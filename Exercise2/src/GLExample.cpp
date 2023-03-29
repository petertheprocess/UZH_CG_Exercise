#include "GLExample.h"
#include "Cube.h"
#include <iostream>

namespace cgCourse
{
	GLExample::GLExample(glm::uvec2 _windowSize, std::string _title) : GLApp(_windowSize, _title, false) {}

	bool GLExample::init()
	{
		// Framebuffer size and window size may be different in high-DPI displays
		// setup camera with standard view (static for our case)
		cam.create(getFramebufferSize(),
				   glm::vec3(3, 3, 7),
				   glm::vec3(0, 0, 0),
				   glm::vec3(0, 1, 0));

		programForShape = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Shape");
		programForTorusNormals = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Normals");

		// Init models
		cube = std::make_shared<Cube>();
		if (!cube->createVertexArray(0, 1, 2))
			return false;

		// transform the cube
		// cube->setPosition(glm::vec3(1, 0, 0));
		// cube->calculateModelMatrix(1);
		// cube->setScaling(glm::vec3(1.5, 1.5, 1.5));

		torus = std::make_shared<Torus>();
		if (!torus->createVertexArray(0, 1, 2))
			return false;

		// TODO: transform the torus //done
		// torus->setPosition(glm::vec3(-1, 0, 0));
		// torus->calculateModelMatrix();
		torus->setScaling(glm::vec3(3, 0.8, 1));

		// Init multiline field for normals of objects
		normalsTorus = std::make_shared<MultiLine>(torus->positions, torus->normals);
		if (!normalsTorus->createVertexArray(0, 1, 2))
			return false;

		return true;
	}

	bool GLExample::update()
	{
		// TODO: spin animation for the cube and the torus
		torus->setPosition(glm::vec3(-3, 0, 0));
		torus->setRotation(glfwGetTime()-timeOffset, glm::vec3(3, 3, 7));
		torus->calculateModelMatrix(0);

		cube->setPosition(glm::vec3(-3, 0, 0));
		cube->setRotation(glfwGetTime()-timeOffset, glm::vec3(1, 1, 1));
		cube->calculateModelMatrix(1);
		return true;
	}

	bool GLExample::render()
	{
		glEnable(GL_DEPTH_TEST);

		// Clear the color and depth buffers
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		if(settings.action)
			update();

		renderCubes();
		renderTorus();

		return true;
	}

	void GLExample::renderCubes()
	{
		programForShape->bind();

		mvpMatrix = cam.getViewProjectionMatrix() * cube->getModelMatrix();
		glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &cube->getModelMatrix()[0][0]);
		glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
		cube->draw();

		programForShape->unbind();
	}

	void GLExample::renderTorus()
	{
		programForShape->bind();

		mvpMatrix = cam.getViewProjectionMatrix() * torus->getModelMatrix();
		normalMatrix = calculateNormalMatrix(mvpMatrix);

		glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &torus->getModelMatrix()[0][0]);
		glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
		glUniformMatrix3fv(programForShape->getUniformLocation("normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

		// TODO: compute normal matrix and add it as uniform variable

		torus->draw();

		programForShape->unbind();

		if (!drawTorusNormals)
			return;

		programForTorusNormals->bind();
		mvpMatrix = cam.getViewProjectionMatrix() * torus->getModelMatrix();
		glUniformMatrix4fv(programForTorusNormals->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
		glUniformMatrix3fv(programForShape->getUniformLocation("normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

		normalsTorus->updateNormalDisplay(normalMatrix,settings);
		normalsTorus->draw();

		programForTorusNormals->unbind();
	}

	bool GLExample::end()
	{
		programForShape->deleteShaderProgramFromGPU();
		programForTorusNormals->deleteShaderProgramFromGPU();
		return true;
	}
	glm::mat3 GLExample::calculateNormalMatrix(glm::mat4 modelViewMatrix)
	{
		return glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
	}
}
