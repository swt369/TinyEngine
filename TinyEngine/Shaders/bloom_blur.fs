#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;
uniform bool horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 texel_size = 1.0 / textureSize(screenTexture, 0);
	vec3 result = texture(screenTexture, uv).rgb * weight[0];
	if (horizontal)
	{
		for (int i = 1; i < 5; i++)
		{
			result += texture(screenTexture, uv + vec2(texel_size.x * i, 0.0)).rgb * weight[i];
			result += texture(screenTexture, uv - vec2(texel_size.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; i++)
		{
			result += texture(screenTexture, uv + vec2(0.0, texel_size.y * i)).rgb * weight[i];
			result += texture(screenTexture, uv - vec2(0.0, texel_size.y * i)).rgb * weight[i];
		}
	}
	
	FragColor = vec4(result, 1.0);
}