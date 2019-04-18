#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 FragPos;
out vec3 normal;
out vec2 uv;

uniform mat4 localToClip;
uniform mat4 model;
uniform mat3 normalMat;

void main()
{
	gl_Position = localToClip * vec4(aPos, 1.0);
	
	FragPos = (model * vec4(aPos, 1.0)).xyz;
	normal = normalMat * aNormal;
	uv = aUV;
} 