#version 330 core

out vec4 color;

in vec2 TexCoords;
in vec4 ParticleColor;

uniform sampler2D image;

void main()
{
	color = texture(image, TexCoords) * ParticleColor;
}
