#ifndef GLAPP_H
#define GLAPP_H

#include "GLIncludes.h"
#include <glm/vec2.hpp>
#include <string>

namespace cgCourse
{
	
	class GLApp
	{
	public:
		GLApp(glm::uvec2 _windowSize, std::string _title, bool resizeable = true);
		virtual ~GLApp();

		bool run();

		// Default implementation of event handlers
		virtual void keyPressHandler(int key, int action, int mods);
		virtual void mouseClickHandler(int button, int action);
		virtual void mouseMoveHandler(glm::dvec2 position);

		Settings settings={false,true};
		double pauseTime=0;
		double timeOffset=0;

	protected:
		void stop();

		// Virtual methods for initialization, rendering and termination
		virtual bool init() = 0;
		virtual bool update() = 0;
		virtual bool render() = 0;
		virtual bool end() = 0;

		// Utility methods
		glm::uvec2 getFramebufferSize() const;
		bool checkGlError(const char * message) const;

		GLFWwindow * window_ = nullptr;
		std::string const title;
	};
}


#endif // GLAPP_H

