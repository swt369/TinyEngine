#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D hdrBuffer;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrBuffer, uv).rgb;
	// Exposure tone mapping
	float exposure = 0.5f;
	vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
	// Gamma correction
	mapped = pow(mapped, vec3(1.0 / gamma));
	FragColor = vec4(mapped, 1.0);
}