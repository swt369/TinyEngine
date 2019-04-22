#version 330 core

#define MAX_DIRECTIONAL_LIGHT_COUNT 2
#define MAX_POINT_LIGHT_COUNT 2
#define MAX_SPOT_LIGHT_COUNT 2

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

struct DirectionalLightExtended
{
	vec3 direction_tangent;
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

struct PointLightExtended
{
	vec3 position_tangent;
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

struct SpotLightExtended
{
	vec3 position_tangent;
	vec3 direction_tangent;
};

in vec3 fragPos_tangent;
in vec3 fragPos;
in vec3 viewPos_tangent;
in vec2 uv;
in DirectionalLightExtended directionalLightExtends[MAX_DIRECTIONAL_LIGHT_COUNT];
in PointLightExtended pointLightExtends[MAX_POINT_LIGHT_COUNT];
in SpotLightExtended spotLightExtends[MAX_SPOT_LIGHT_COUNT];

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

vec3 calcDirLight(int index, vec3 normal_tangent, vec3 viewDir_tangent);
vec3 calcPointLight(int index, vec3 normal_tangent, vec3 viewDir_tangent);
vec3 calcSpotLight(int index, vec3 normal_tangent, vec3 viewDir_tangent);
float calcDirectionalShadow(int index);
float calcOmniDirectionalShadow(int index);

void main()
{
	vec3 viewDir_tangent = normalize(viewPos_tangent - fragPos_tangent);
	
	vec3 normal_tangent = texture(normalMap, uv).rgb;
	normal_tangent = normalize(normal_tangent * 2.0 - 1.0);
	
	vec3 resultColor = vec3(0.0);
	for (int i = 0; i < directionalLightCount; i++)
		resultColor += calcDirLight(i, normal_tangent, viewDir_tangent);
		
	for (int i = 0; i < pointLightCount; i++)
		resultColor += calcPointLight(i, normal_tangent, viewDir_tangent);
		
	for (int i = 0; i < spotLightCount; i++)
		resultColor += calcSpotLight(i, normal_tangent, viewDir_tangent);
		
	FragColor = vec4(resultColor, 1.0);
}

vec3 calcDirLight(int index, vec3 normal_tangent, vec3 viewDir_tangent)
{
	DirectionalLight light = directionalLights[index];
	DirectionalLightExtended lightExtended = directionalLightExtends[index];

	vec3 lightDir_tangent = normalize(-lightExtended.direction_tangent);
	vec3 reflectDir_tangent = reflect(-lightDir_tangent, normal_tangent);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir_tangent, normal_tangent), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse * texture(texture_diffuse_0, uv).xyz;
	
	float specStrength = pow(max(dot(reflectDir_tangent, viewDir_tangent), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	float shadowResult = calcDirectionalShadow(index);
	return ambient + (1.0 - shadowResult) * (diffuse + specular);
}

vec3 calcPointLight(int index, vec3 normal_tangent, vec3 viewDir_tangent)
{
	PointLight light = pointLights[index];
	PointLightExtended lightExtended = pointLightExtends[index];

	vec3 lightDir_tangent = normalize(lightExtended.position_tangent - fragPos_tangent);
	float distance = length(lightExtended.position_tangent - fragPos_tangent);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	vec3 reflectDir_tangent = reflect(-lightDir_tangent, normal_tangent);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir_tangent, normal_tangent), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse * texture(texture_diffuse_0, uv).xyz;
	
	float specStrength = pow(max(dot(reflectDir_tangent, viewDir_tangent), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	float shadowResult = calcOmniDirectionalShadow(index);
	return attenuation * (ambient + (1.0 - shadowResult) * (diffuse + specular));
}

vec3 calcSpotLight(int index, vec3 normal_tangent, vec3 viewDir_tangent)
{
	SpotLight light = spotLights[index];
	SpotLightExtended lightExtended = spotLightExtends[index];

	vec3 lightDir_tangent = normalize(lightExtended.position_tangent - fragPos_tangent);
	vec3 reflectDir_tangent = reflect(-lightDir_tangent, normal_tangent);
	float theta = dot(lightDir_tangent, normalize(-lightExtended.direction_tangent));
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir_tangent, normal_tangent), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse;
	
	float specStrength = pow(max(dot(reflectDir_tangent, viewDir_tangent), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	return intensity * (ambient + diffuse + specular);
}

float calcDirectionalShadow(int index)
{
	DirectionalLight light = directionalLights[index];

	vec3 lightDir = normalize(-light.direction);
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // float closestDepth = texture(directionalShadowMaps[index], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	
    // check whether current frag pos is in shadow
	float bias = 0.01;	
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
	vec3 lightToFrag = fragPos - light.position;
	float viewDistance = length(viewPos - fragPos);
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