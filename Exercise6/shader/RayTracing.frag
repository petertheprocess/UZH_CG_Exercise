#version 410 core

in vec3 objectColor;
in vec2 texCoord;

out vec3 color;
uniform sampler2D texMap;

void main()
{
	color = texture(texMap, texCoord).rgb;
	// color = objectColor;

}

