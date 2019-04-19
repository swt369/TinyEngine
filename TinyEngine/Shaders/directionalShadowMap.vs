#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 localToClip;

void main()
{
	gl_Position = localToClip * vec4(aPos, 1.0);
}