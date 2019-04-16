#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 localToClip;

out vec2 uv;

void main()
{
	uv = aUV;
	gl_Position = localToClip * vec4(aPos, 1.0);
}