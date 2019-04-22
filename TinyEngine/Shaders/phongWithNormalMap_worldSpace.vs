#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 uv;
out mat3 TBN;

uniform mat4 localToClip;
uniform mat4 model;
uniform mat3 normalMat;

void main()
{
	gl_Position = localToClip * vec4(aPos, 1.0);
	
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(normalMat * aNormal));
	TBN = mat3(T, B, N);
	
	FragPos = (model * vec4(aPos, 1.0)).xyz;
	uv = aUV;
} 