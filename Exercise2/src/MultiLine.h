#ifndef MULTILINE_H
#define MULTILINE_H

#include "Shape.h"

namespace cgCourse
{
	class MultiLine : public Shape
	{
	public:
		MultiLine(	const std::vector<glm::vec3> & _vertices,
					const std::vector<glm::vec3> & _normals
					);
		bool createVertexArray( GLuint posAttribLoc,
								GLuint colAttribLoc,
								GLuint normAttribLoc
								) override;
		void draw() const override;
		void updateNormalDisplay(glm::mat3 normalMatrix, Settings setting);
		bool isRealNormal = false;
	};
}

#endif // MULTILINE_H

