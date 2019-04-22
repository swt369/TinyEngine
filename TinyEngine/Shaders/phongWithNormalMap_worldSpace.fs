#version 330 core

#define MAX_DIRECTIONAL_LIGHT_COUNT 4
#define MAX_POINT_LIGHT_COUNT 1
#define MAX_SPOT_LIGHT_COUNT 1

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	mat4 lightSpaceMatrix;
	vec3 direction;
};

struct PointLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	float constant;
	float linear;
	float quadratic;
	float range;
};

struct SpotLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;
};

in mat3 TBN;
in vec3 FragPos;
in vec2 uv;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];
uniform sampler2D directionalShadowMaps[MAX_DIRECTIONAL_LIGHT_COUNT];
uniform int directionalLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform samplerCube omniDirectionalShadowMaps[MAX_POINT_LIGHT_COUNT];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];
uniform int spotLightCount;

uniform sampler2D texture_diffuse_0;
uniform sampler2D normalMap;

vec3 calcDirLight(int index, vec3 normal, vec3 viewDir);
vec3 calcPointLight(int index, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(int index, vec3 normal, vec3 viewDir);
float calcDirectionalShadow(int index, vec3 normal);
float calcOmniDirectionalShadow(int index);

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 normal = texture(normalMap, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);
	
	vec3 resultColor = vec3(0.0);
	for (int i = 0; i < directionalLightCount; i++)
		resultColor += calcDirLight(i, normal, viewDir);
		
	for (int i = 0; i < pointLightCount; i++)
		resultColor += calcPointLight(i, normal, viewDir);
		
	for (int i = 0; i < spotLightCount; i++)
		resultColor += calcSpotLight(i, normal, viewDir);
		
	FragColor = vec4(resultColor, 1.0);
}

vec3 calcDirLight(int index, vec3 normal, vec3 viewDir)
{
	DirectionalLight light = directionalLights[index];

	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse * texture(texture_diffuse_0, uv).xyz;
	
	float specStrength = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	float shadowResult = calcDirectionalShadow(index, normal);
	return ambient + (1.0 - shadowResult) * (diffuse + specular);
}

vec3 calcPointLight(int index, vec3 normal, vec3 viewDir)
{
	PointLight light = pointLights[index];

	vec3 lightDir = normalize(light.position - FragPos);
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse * texture(texture_diffuse_0, uv).xyz;
	
	float specStrength = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	float shadowResult = calcOmniDirectionalShadow(index);
	return attenuation * (ambient + (1.0 - shadowResult) * (diffuse + specular));
}

vec3 calcSpotLight(int index, vec3 normal, vec3 viewDir)
{
	SpotLight light = spotLights[index];

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse;
	
	float specStrength = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	return intensity * (ambient + diffuse + specular);
}

float calcDirectionalShadow(int index, vec3 normal)
{
	DirectionalLight light = directionalLights[index];

	vec3 lightDir = normalize(-light.direction);
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(FragPos, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // float closestDepth = texture(directionalShadowMaps[index], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowMaps[index], 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMaps[index], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	
	if (projCoords.z > 1.0)
	{
		shadow = 0.0;
	}
	
    return shadow;
}

float calcOmniDirectionalShadow(int index)
{
	PointLight light = pointLights[index];

	vec3 sampleOffsetDirections[20] = vec3[]
	(
		vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
		vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
		vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
		vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
		vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
	);
	
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	vec3 lightToFrag = FragPos - light.position;
	float viewDistance = length(viewPos - FragPos);
	float currentDepth = length(lightToFrag);
	float diskRadius = 0.05;
	for (int i = 0; i < samples; i++)
	{
		float closestDepth = texture(omniDirectionalShadowMaps[index], lightToFrag + sampleOffsetDirections[i] * diskRadius).r * light.range;
		shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
	}
	shadow /= float(samples);
	
	return shadow;
}