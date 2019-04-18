#version 330 core

#define MAX_DIRECTIONAL_LIGHT_COUNT 4
#define MAX_POINT_LIGHT_COUNT 16
#define MAX_SPOT_LIGHT_COUNT 16

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
	mat4 lightSpaceMatrix;

	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	mat4 lightSpaceMatrix;
	
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;
};

in vec3 FragPos;
in vec3 normal;
in vec2 uv;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];
uniform int directionalLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];
uniform int spotLightCount;

uniform sampler2D texture_diffuse_0;
uniform sampler2D shadowMap;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos);
float calcShadow(vec4 fragPosLightSpace, vec3 lightDir);

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(normal);
	
	vec3 resultColor;
	for (int i = 0; i < directionalLightCount; i++)
		resultColor += calcDirLight(directionalLights[i], norm, viewDir);
		
	for (int i = 0; i < pointLightCount; i++)
		resultColor += calcPointLight(pointLights[i], norm, viewDir, FragPos);
		
	for (int i = 0; i < spotLightCount; i++)
		resultColor += calcSpotLight(spotLights[i], norm, viewDir, FragPos);
		
	FragColor = vec4(resultColor, 1.0);
}

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse * texture(texture_diffuse_0, uv).xyz;
	
	float specStrength = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	float shadowResult = calcShadow(light.lightSpaceMatrix * vec4(FragPos, 1.0), lightDir);
	return ambient + (1.0 - shadowResult) * (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse;
	
	float specStrength = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = specStrength * light.specular * material.specular;

	return attenuation * (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
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

float calcShadow(vec4 fragPosLightSpace, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
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