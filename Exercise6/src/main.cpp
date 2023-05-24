#include "GLExample.h"

int main(int argc, char * argv[])
{
	cgCourse::GLExample app(glm::uvec2(1200, 1200), "Exercise 6");
	return app.run();
}

