#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 localToClip;

void main()
{
	gl_Position = localToClip * vec4(aPos, 1.0);
}