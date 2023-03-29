#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;

out vec3 fragColor;

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

void main()
{
	// TODO: complete with the necessary code
	gl_Position = mvpMatrix * vec4(vPosition, 1);
	fragColor = vColor;
}

