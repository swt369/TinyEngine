#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

void main()
{
	vec4 color = texture(screenTexture, uv);
	float averageGreyscale = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	FragColor = vec4(averageGreyscale, averageGreyscale, averageGreyscale, 1.0);
}