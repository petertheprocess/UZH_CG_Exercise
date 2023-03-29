#include "Shape.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cgCourse
{
	Shape::~Shape()
	{
		glDeleteBuffers(1, &posBufferID);
		glDeleteBuffers(1, &colorBufferID);
		glDeleteBuffers(1, &normalBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteVertexArrays(1, &vaoID);
	}

	bool Shape::createVertexArray(GLuint posAttribLoc, GLuint colAttribLoc, GLuint normAttribLoc)
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
		// for positions
		glGenBuffers(1, &posBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttribLoc);

		// for colors
		glGenBuffers(1, &colorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
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

		initIndexBuffer();

		// Reset state
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

	void Shape::draw() const
	{
		glBindVertexArray(vaoID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// TODO: complete the following methods
	
	
	void Shape::setPosition(glm::vec3 _pos)
	{
		this->objectPosition = _pos;
		this->translationMatrix = glm::translate(glm::mat4(1.0f),_pos);

		// glm::mat4() constructor read params by column!
		// this->translationMatrix = glm::transpose(glm::mat4(1.0f, 0.0f, 0.0f, _pos.x,
		// 									0.0f, 1.0f, 0.0f, _pos.y,
		// 									0.0f, 0.0f, 1.0f, _pos.z,
		// 									0.0f, 0.0f, 0.0f, 1.0f));
	}

	void Shape::setRotation(float _angle, glm::vec3 _rot)
	{
		this->rotationMatrix = glm::rotate(glm::mat4(1.0f),_angle,_rot);

		// // normalize
		// glm::vec3 axis = glm::normalize(_rot);

		// // constract rotationMatrix
		// float c = cos(_angle);
		// float s = sin(_angle);
		// float t = 1.0f - c;
		// float x = axis.x, y = axis.y, z = axis.z;
		// this->rotationMatrix = glm::transpose(glm::mat4(
		// 	t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0.0f,
		// 	t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0.0f,
		// 	t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0.0f,
		// 	0.0f, 0.0f, 0.0f, 1.0f));
	}

	void Shape::setScaling(glm::vec3 _scale)
	{
		this->scalingMatrix = glm::scale(glm::mat4(1.0f),_scale);
		// this->scalingMatrix = glm::transpose(glm::mat4(_scale.x, 0.0f, 0.0f, 0.0f,
		// 								0.0f, _scale.y, 0.0f, 0.0f,
		// 								0.0f, 0.0f, _scale.z, 0.0f,
		// 								0.0f, 0.0f, 0.0f, 1.0f));
	}

	/**
	 * @brief When input parameter is true, calculate the modelMatrix 
	 * by scall-rotate-translate(SRT) order, otherwise follow the
	 * scall-translate-rotate(STR) order.
	 * 
	 * @param isSRT 
	 */
	void Shape::calculateModelMatrix(bool isSRT)
	{
		if(isSRT)
			this->modelMatrix = translationMatrix*rotationMatrix*scalingMatrix;
		else //STR
			this->modelMatrix = rotationMatrix*translationMatrix*scalingMatrix;
	}


	// end TODO

	void Shape::initIndexBuffer()
	{
		glGenBuffers(1, &indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::uvec3), faces.data(), GL_STATIC_DRAW);
	}

	const glm::mat4 &Shape::getModelMatrix() const
	{
		return modelMatrix;
	}

	const glm::vec3 &Shape::getPosition() const
	{
		return objectPosition;
	}
}
