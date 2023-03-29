#ifndef GLEXAMPLE_H
#define GLEXAMPLE_H

#include "GLApp.h"
#include "Cube.h"
#include "Torus.h"
#include "ShaderProgram.h"
#include "MultiLine.h"
#include "Camera.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <memory>

namespace cgCourse
{
	class GLExample : public GLApp
	{
	public:
		GLExample(glm::uvec2 _windowSize, std::string _title);

		bool init() override;
		bool update() override;
		bool render() override;
		bool end() override;
		glm::mat3 calculateNormalMatrix(glm::mat4 modelViewMatrix);

	private:
		void renderCubes();
		void renderTorus();

		std::shared_ptr<ShaderProgram> programForShape;
		std::shared_ptr<ShaderProgram> programForTorusNormals;

		std::shared_ptr<Cube> cube;
		std::shared_ptr<Torus> torus;
		std::shared_ptr<MultiLine> normalsTorus;

		glm::mat4 mvpMatrix = glm::mat4(1);
		glm::mat3 normalMatrix = glm::mat3(1);

		Camera cam;
		bool drawTorusNormals = true;
	};
}

#endif // GLEXAMPLE_H

