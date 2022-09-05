#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

void main()
{
	vec2 newUV = floor(TexCoords * 8)/8;

	FragColor = vec4(newUV, 0.0, 1.0);
}