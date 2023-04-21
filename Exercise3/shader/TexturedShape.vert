#version 410 core

/* TODO Task 2 update your vertex shader so that...
 *      ...it emits all necessary information to the fragment shader texturing
 *
 */

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;
layout(location = 4) in vec3 vTangent;

out vec3 objectColor;
// TODO Task 2 Add the output data required in the fragment shader
out vec2 texCoord;
// out vec3 normal;
out vec3 worldPos;
out mat3 TBN;
// TODO END

// matrices that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(vPosition, 1);
	

	/* TODO Task 2 set texture coordinates for fragment shader here here and the output required for the nomal mapping */
	// normal = vNormal;
	texCoord = vTexCoords;
	worldPos=vec3(modelMatrix*vec4(vPosition,1));

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 T = normalize(normalMatrix * vTangent);
	vec3 N = normalize(normalMatrix * vNormal);
	vec3 B = cross(T, N);
	
	TBN = mat3(T,B,N);
	// objectColor = TBN*vec3(1,1,1);
	// End TODO
}

