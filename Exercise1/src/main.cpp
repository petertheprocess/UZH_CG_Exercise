#include "GLExample.h"

int main(int argc, char * argv[])
{
	// create the app
	cgCourse::GLExample app(glm::uvec2(1280, 1024), "hotkey: W for wireframe mode | C for toggle culling face | '[' ']' to tune the offset");

	// run the app
	return app.run();
}

