#ifndef TORUS_H
#define TORUS_H

#include "Shape.h"
#include "inputModule.h"

namespace cgCourse
{
    class Torus : public Shape
    {
    public:
        RenderSettings *settingPtr;
        Torus();
        Torus(float offset);
        Torus(float offset,RenderSettings *setting);
        void draw();
    };
}

#endif //  TORUS_H