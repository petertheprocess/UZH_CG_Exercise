#include "Cube.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
namespace cgCourse
{ 
	Cube::Cube() : Shape()
	{
		// set geometry with respect to local origin
		positions = {
			{ -1.0f, -1.0f, -1.0f },	// index 0
			{ -1.0f, -1.0f, 1.0f },
			{ -1.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, -1.0f },

			{ 1.0f, 1.0f, 1.0f },		// index 4
			{ 1.0f, -1.0f, 1.0f },
			{ 1.0f, -1.0f, -1.0f },
			{ 1.0f, 1.0f, -1.0f },

			{ -1.0f, -1.0f, -1.0f },	// index 8
			{ 1.0f, -1.0f, -1.0f },
			{ 1.0f, -1.0f, 1.0f },
			{ -1.0f, -1.0f, 1.0f },

			{ 1.0f, 1.0f, 1.0f },		// index 12
			{ 1.0f, 1.0f, -1.0f },
			{ -1.0f, 1.0f, -1.0f },
			{ -1.0f, 1.0f, 1.0f },

			{ -1.0f, -1.0f, -1.0f },	// index 16
			{ -1.0f, 1.0f, -1.0f },
			{ 1.0f, 1.0f, -1.0f },
			{ 1.0f, -1.0f, -1.0f },

			{ 1.0f, 1.0f, 1.0f },		// index 20
			{ -1.0f, 1.0f, 1.0f },
			{ -1.0f, -1.0f, 1.0f },
			{ 1.0f, -1.0f, 1.0f }
		};

		colors = {
			{ 0.8f, 0.8f, 0.0f },
			{ 0.8f, 0.8f, 0.0f },
			{ 0.8f, 0.8f, 0.0f },
			{ 0.8f, 0.8f, 0.0f },

			{ 0.8f, 0.0f, 0.0f },
			{ 0.8f, 0.0f, 0.0f },
			{ 0.8f, 0.0f, 0.0f },
			{ 0.8f, 0.0f, 0.0f },

			{ 0.0f, 0.8f, 0.0f },
			{ 0.0f, 0.8f, 0.0f },
			{ 0.0f, 0.8f, 0.0f },
			{ 0.0f, 0.8f, 0.0f },

			{ 0.0f, 0.0f, 0.8f },
			{ 0.0f, 0.0f, 0.8f },
			{ 0.0f, 0.0f, 0.8f },
			{ 0.0f, 0.0f, 0.8f },

			{ 0.0f, 0.8f, 0.8f },
			{ 0.0f, 0.8f, 0.8f },
			{ 0.0f, 0.8f, 0.8f },
			{ 0.0f, 0.8f, 0.8f },

			{ 0.8f, 0.0f, 0.8f },
			{ 0.8f, 0.0f, 0.8f },
			{ 0.8f, 0.0f, 0.8f },
			{ 0.8f, 0.0f, 0.8f }
		};

		normals = {
			{ -1.0f, 0.0f, 0.0f },
			{ -1.0f, 0.0f, 0.0f },
			{ -1.0f, 0.0f, 0.0f },
			{ -1.0f, 0.0f, 0.0f },

			{ 1.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },

			{ 0.0f, -1.0f, 0.0f },
			{ 0.0f, -1.0f, 0.0f },
			{ 0.0f, -1.0f, 0.0f },
			{ 0.0f, -1.0f, 0.0f },

			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },

			{ 0.0f, 0.0f, -1.0f },
			{ 0.0f, 0.0f, -1.0f },
			{ 0.0f, 0.0f, -1.0f },
			{ 0.0f, 0.0f, -1.0f },

			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f }
		};

		for(int i = 0; i < positions.size(); ++i)
			texCoords.push_back({(i >> 1) & 1, ((i & 2) >> 1) ^ (i & 1)});

		faces = {
			{ 0, 1, 2 }, { 2, 3, 0 },
			{ 4, 5, 6 }, { 6, 7, 4 },
			{ 8, 9, 10 }, { 10, 11, 8 },
			{ 12, 13, 14 }, { 14, 15, 12 },
			{ 16, 17, 18 }, { 18, 19, 16 },
			{ 20, 21, 22 }, { 22, 23, 20 }
		};

		for(const glm::vec3 & n: normals)
			tangents.push_back(glm::vec3(abs(n.x) < 0.9, abs(n.x) > 0.9, 0));
	}
}

