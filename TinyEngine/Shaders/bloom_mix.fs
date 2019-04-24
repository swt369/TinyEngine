#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

void main()
{
	vec3 hdrColor = texture(screenTexture, uv).rgb;
	vec3 bloomColor = texture(bloomTexture, uv).rgb;
	FragColor = vec4(hdrColor + bloomColor, 1.0);
}