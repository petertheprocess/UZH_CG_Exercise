#include "Square.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
namespace cgCourse
{
	Square::Square() : Shape()
	{
		// set geometry with respect to local origin
		positions = {
			{ 1.0f, 1.0f, 0.0f },	// index 0
			{ 1.0f, -1.0f, 0.0f },
			{ -1.0f, -1.0f, 0.0f },
			{ -1.0f, 1.0f, 0.0f }
		};

		colors = {
			{ 0.8f, 0.8f, 0.0f },
			{ 0.8f, 0.8f, 0.0f },
			{ 0.8f, 0.8f, 0.0f },
			{ 0.8f, 0.8f, 0.0f }
		};

		normals = {
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f }
		};

        texCoords = {
            {1.0f,1.0f},{1.0f,0.0f},
            {0.0f,0.0f},{0.0f,1.0f}
        };

		faces = {
			{ 0, 1, 2 }, { 2, 3, 0 }
		};

		for(const glm::vec3 & n: normals)
			tangents.push_back(glm::vec3(abs(n.x) < 0.9, abs(n.x) > 0.9, 0));
	}
}

