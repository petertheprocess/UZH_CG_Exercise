#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;

out vec3 objectColor;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(vPosition, 1);
    texCoord = vTexCoords;
    objectColor = vColor;
}

