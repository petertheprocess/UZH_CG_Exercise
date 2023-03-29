#include "GLExample.h"

int main(int argc, char * argv[])
{
	cgCourse::GLExample app(glm::uvec2(1200, 1000), "HotKey: 'SPACE' Pause/Play the animation | 'N' toggle between right and wrong normals");
	return app.run();
}

