#version 410 core

// Input vertex data
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;

// Output data will be interpolated for each fragment.
// Tip: Try to use the flat modifier to make color associations of a fragment visible for debugging.
out vec3 fragColor;

// Values that stay constant for the whole mesh.
uniform mat4 mvpMatrix;
uniform float offset;

void main()
{
	// Output position of the vertex, in clip space : MVP * vPosition
	gl_Position = mvpMatrix * vec4(vPosition+vNormal*offset, 1);
	// The color of each vertex will be i, 1);
	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	fragColor = vColor;
}

