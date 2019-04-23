#version 330 core

#define MAX_DIRECTIONAL_LIGHT_COUNT 1
#define MAX_POINT_LIGHT_COUNT 4
#define MAX_SPOT_LIGHT_COUNT 1

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
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

in vec3 FragPos;
in vec3 normal;
in vec2 uv;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform int pointLightCount;

uniform sampler2D texture_diffuse_0;

vec3 calcPointLight(int index, vec3 normal, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(normal);
	
	vec3 resultColor = vec3(0.0);
		
	for (int i = 0; i < pointLightCount; i++)
		resultColor += calcPointLight(i, norm, viewDir);
		
	FragColor = vec4(resultColor, 1.0);
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

	return attenuation * (ambient + diffuse);
}