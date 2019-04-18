#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadowMap;

void main()
{             
    float depthValue = texture(shadowMap, TexCoords).r;
	//FragColor = vec4(0.5, 0.5, 0.5, 1.0);
    FragColor = vec4(depthValue, depthValue, depthValue, 1.0);
}

