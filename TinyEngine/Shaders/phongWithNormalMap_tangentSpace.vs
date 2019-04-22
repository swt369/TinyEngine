#version 330 core

#define MAX_DIRECTIONAL_LIGHT_COUNT 2
#define MAX_POINT_LIGHT_COUNT 2
#define MAX_SPOT_LIGHT_COUNT 2

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

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

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];
uniform int directionalLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];
uniform int spotLightCount;

out vec3 fragPos_tangent;
out vec3 fragPos;
out vec3 viewPos_tangent;
out vec2 uv;
out DirectionalLightExtended directionalLightExtends[MAX_DIRECTIONAL_LIGHT_COUNT];
out PointLightExtended pointLightExtends[MAX_POINT_LIGHT_COUNT];
out SpotLightExtended spotLightExtends[MAX_SPOT_LIGHT_COUNT];

uniform mat4 localToClip;
uniform mat4 model;
uniform mat3 normalMat;
uniform vec3 viewPos;

void DirectionalLightToTangentSpace(mat3 TBN_inverse);
void PointLightToTangentSpace(mat3 TBN_inverse);
void SpotLightToTangentSpace(mat3 TBN_inverse);

void main()
{
	gl_Position = localToClip * vec4(aPos, 1.0);
	uv = aUV;
	
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(normalMat * aNormal));
	mat3 TBN = mat3(T, B, N);
	mat3 TBN_inverse = transpose(TBN);
	
	fragPos = vec3(model * vec4(aPos, 1.0));
	fragPos_tangent = TBN_inverse * fragPos;
	viewPos_tangent = TBN_inverse * viewPos;
	
	DirectionalLightToTangentSpace(TBN_inverse);
	PointLightToTangentSpace(TBN_inverse);	
	SpotLightToTangentSpace(TBN_inverse);
} 

void DirectionalLightToTangentSpace(mat3 TBN_inverse)
{
	for (int i = 0; i < directionalLightCount; i++)
	{
		DirectionalLight light = directionalLights[i];
		
		directionalLightExtends[i].direction_tangent = TBN_inverse * light.direction;
	}
}

void PointLightToTangentSpace(mat3 TBN_inverse)
{
	for (int i = 0; i < pointLightCount; i++)
	{
		PointLight light = pointLights[i];
		
		pointLightExtends[i].position_tangent = TBN_inverse * light.position;
	}
}

void SpotLightToTangentSpace(mat3 TBN_inverse)
{
	for (int i = 0; i < spotLightCount; i++)
	{
		SpotLight light = spotLights[i];
		
		spotLightExtends[i].position_tangent = TBN_inverse * light.position;
		spotLightExtends[i].direction_tangent = TBN_inverse * light.direction;
	}
}