#include "Cube.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
namespace cgCourse
{
	Cube::Cube() : Shape()
	{
		// set geometry with respect to local origin
		positions = {
			{-1.0f, -1.0f, -1.0f}, // index 0
			{-1.0f, -1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, -1.0f},

			{1.0f, 1.0f, 1.0f}, // index 4
			{1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f},

			{-1.0f, -1.0f, -1.0f}, // index 8
			{1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, 1.0f},
			{-1.0f, -1.0f, 1.0f},

			{1.0f, 1.0f, 1.0f}, // index 12
			{1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f, 1.0f},

			{-1.0f, -1.0f, -1.0f}, // index 16
			{-1.0f, 1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f},

			{1.0f, 1.0f, 1.0f}, // index 20
			{-1.0f, 1.0f, 1.0f},
			{-1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f}};

		colors = {
			{0.8f, 0.0f, 0.0f},
			{0.8f, 0.0f, 0.0f},
			{0.8f, 0.0f, 0.0f},
			{0.8f, 0.0f, 0.0f},

			{0.8f, 0.8f, 0.0f},
			{0.8f, 0.8f, 0.0f},
			{0.8f, 0.8f, 0.0f},
			{0.8f, 0.8f, 0.0f},

			{0.0f, 0.8f, 0.0f},
			{0.0f, 0.8f, 0.0f},
			{0.0f, 0.8f, 0.0f},
			{0.0f, 0.8f, 0.0f},

			{0.0f, 0.0f, 0.8f},
			{0.0f, 0.0f, 0.8f},
			{0.0f, 0.0f, 0.8f},
			{0.0f, 0.0f, 0.8f},

			{0.0f, 0.8f, 0.8f},
			{0.0f, 0.8f, 0.8f},
			{0.0f, 0.8f, 0.8f},
			{0.0f, 0.8f, 0.8f},

			{0.8f, 0.0f, 0.8f},
			{0.8f, 0.0f, 0.8f},
			{0.8f, 0.0f, 0.8f},
			{0.8f, 0.0f, 0.8f}};

		normals = {
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},

			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},

			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},

			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},

			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},

			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f}};

		/* TODO Task 2 set the texture coordinates for the cube similar
		 *       to the positions array. After you set the texture coordinates
		 *       for this object try to access them in the shader and
		 *       check by color output if they are ok. You can also
		 *       compare your results in the end with the final image
		 *       so that the texture orientation is done right.
		 *
		 */
		// texCoords = {
		// 	{0.0f, 0.0f},
		// 	{0.0f, 1.0f},
		// 	{1.0f, 1.0f},
		// 	{1.0f, 0.0f},

		// 	{0.0f, 0.0f},
		// 	{0.0f, 1.0f},
		// 	{1.0f, 1.0f},
		// 	{1.0f, 0.0f},

		// 	{0.0f, 0.0f},
		// 	{0.0f, 1.0f},
		// 	{1.0f, 1.0f},
		// 	{1.0f, 0.0f},

		// 	{0.0f, 0.0f},
		// 	{0.0f, 1.0f},
		// 	{1.0f, 1.0f},
		// 	{1.0f, 0.0f},

		// 	{0.0f, 0.0f},
		// 	{0.0f, 1.0f},
		// 	{1.0f, 1.0f},
		// 	{1.0f, 0.0f},

		// 	{0.0f, 0.0f},
		// 	{0.0f, 1.0f},
		// 	{1.0f, 1.0f},
		// 	{1.0f, 0.0f},};
		for (int i = 0; i < positions.size(); i += 4)
		{
			texCoords.push_back(glm::vec2(0.0, 0.0));
			texCoords.push_back(glm::vec2(0.0, 1.0));
			texCoords.push_back(glm::vec2(1.0, 1.0));
			texCoords.push_back(glm::vec2(1.0, 0.0));
		}

		// texCoords->push_back(...

		// for(int i = 0; i < positions.size(); i++) {
		//     texCoords.push_back(glm::vec2(0.0,0.0));
		// }

		// TODO END

		faces = {
			{0, 1, 2}, {2, 3, 0}, {4, 5, 6}, {6, 7, 4}, {8, 9, 10}, {10, 11, 8}, {12, 13, 14}, {14, 15, 12}, {16, 17, 18}, {18, 19, 16}, {20, 21, 22}, {22, 23, 20}};

		/* TODO Task 2 add the tangent vectors for the cube here to
		 *       to the tangent arrays
		 */
		tangents.resize(positions.size());

		for (int i = 0; i < faces.size(); i++)
		{
			glm::vec3 pos1 = positions[faces[i].x];
			glm::vec3 pos2 = positions[faces[i].y];
			glm::vec3 pos3 = positions[faces[i].z];
			glm::vec2 uv1 = texCoords[faces[i].x];
			glm::vec2 uv2 = texCoords[faces[i].y];
			glm::vec2 uv3 = texCoords[faces[i].z];

			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;

			GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			glm::vec3 tangent;
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = glm::normalize(tangent);

			tangents[faces[i].x]+=tangent;
			tangents[faces[i].y]+=tangent;
			tangents[faces[i].z]+=tangent;
		}

		for(auto & t: tangents)
			t = glm::normalize(t);
		// tangents...

		// TODO END
	}
}
