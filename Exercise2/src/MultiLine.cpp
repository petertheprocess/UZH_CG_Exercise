#include "MultiLine.h"

#include <iostream>
#include <cmath>

namespace cgCourse
{
	MultiLine::MultiLine(const std::vector<glm::vec3> &_vertices,
						 const std::vector<glm::vec3> &_normals) : Shape()
	{
		// TODO: initialize positions, colors, and normals

		for (int i = 0; i < _vertices.size(); i++)
		{
			positions.push_back(_vertices[i]);
			positions.push_back(_vertices[i] + _normals[i] * 0.3f);
			normals.push_back(_normals[i]);
			normals.push_back(_normals[i]);
		}
		colors.resize(positions.size());
		colors.assign(positions.size(), glm::vec3(1, 1, 0.3));
	}

	bool MultiLine::createVertexArray(GLuint posAttribLoc, GLuint colAttribLoc, GLuint normAttribLoc)
	{
		// check if all buffer locations are somehow defined
		if ((posAttribLoc == GLuint(-1)) ||
			(colAttribLoc == GLuint(-1)) ||
			(normAttribLoc == GLuint(-1)))
		{
			return false;
		}

		// Initialize Vertex Array Object
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		// Initialize buffer objects with geometry data
		// for positions (dynamic_draw mode)
		glGenBuffers(1, &posBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttribLoc);

		// for colors
		glGenBuffers(1, &colorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colAttribLoc);

		// for normals
		if (normals.size() != 0)
		{
			glGenBuffers(1, &normalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(normAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(normAttribLoc);
		}

		// Reset state
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

	void MultiLine::updateNormalDisplay(glm::mat3 normalMatrix, Settings setting)
	{
		if (setting.normalMatrixOn)
		{
			for (int i = 1; i < normals.size(); i += 2)
			{
				// since normal' = normalMat * normal
				// we wanna endpint after transformation to be
				// end' = original' + normal' = mvpMat * original + normalMat * normal
				// in the shader, all the points will be transformed by multiply mvpMat
				// so if we wanna end' to be like that endpoint before transform should be
				// end = mvpMat_inverse * end'= original + mvpMat_inverse * normalMat * normal
				// given that normalMat = transpose(inverse(mvpMat_upleft))
				// so end = original + transpose(normalMat) * normalMat * normal
				positions[i] = positions[i - 1] + glm::normalize(glm::transpose(normalMatrix) * normalMatrix * normals[i]) * 0.3f;
			}

			glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());
			colors.assign(positions.size(), glm::vec3(1, 0, 1));
			glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(glm::vec3), colors.data());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			isRealNormal = true;
		}
		else if (isRealNormal && !setting.normalMatrixOn)
		{
			for (int i = 1; i < normals.size(); i += 2)
			{
				positions[i] = positions[i - 1] + glm::normalize(normals[i]) * 0.3f;
			}
			glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());
			colors.assign(positions.size(), glm::vec3(1, 1, 0.3));
			glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(glm::vec3), colors.data());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			isRealNormal = false;
		}
	}

	void MultiLine::draw() const
	{
		// TODO: complete code to draw the normals as lines
		glBindVertexArray(this->vaoID);
		glDrawArrays(GL_LINES, 0, positions.size());
		glPointSize(2.0f);
		glBindVertexArray(0);
	}
}
