#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

void main()
{
	FragColor = vec4(texture(screenTexture, uv).rgb, 1.0);
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness <= 1.5)
		discard;
}