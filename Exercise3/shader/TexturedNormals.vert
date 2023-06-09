#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;
layout(location = 4) in vec3 vTangent;

out vec3 fragColor;
uniform sampler2D normalMap;
uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
/* TODO Task 2 Add the normal sampler */

// TODO END
void main()
{
	if((gl_VertexID & 1) == 1)
	{
		gl_Position = mvpMatrix * vec4(vPosition, 1);
	}
	else
	{

		/* TODO Task 2 Calcualte the gl_position of the enpoint fo the normal */
		mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
		vec3 T = normalize(normalMatrix * vTangent);
		vec3 N = normalize(normalMatrix * vNormal);
		vec3 B = cross(T, N);
		mat3 TBN = mat3(T,B,N);
		vec3 normal = texture(normalMap,vTexCoords).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = TBN * normal;
		gl_Position = mvpMatrix * vec4(vPosition + normal, 1);
		/* TODO END */
	}

	fragColor = vColor;
}

