#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse_0;

in vec2 uv;

void main()
{
	FragColor = texture(texture_diffuse_0, uv);
}