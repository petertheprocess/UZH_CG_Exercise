#include "Torus.h"
#include <iostream>

// this define and include will enable M_PI from math.h.
#define _USE_MATH_DEFINES
#include <math.h>

#define PI 3.1415926535f

namespace cgCourse
{
    Torus::Torus() : Torus(0.0) {}

    Torus::Torus(float offset) : Torus(offset, {0}) {}

    Torus::Torus(float offset, RenderSettings *settingPtr)
    {
        this->settingPtr = settingPtr;
        /* TODO: Add your code here
         * First create the torus mesh
         * Then offset the vertex positions along the calculated face normals by offset
         */
        // double R_torus = 10;
        // double R_tube = 2;
        double R_torus = 10;
        double R_tube = 3;
        uint16_t seg_tube = 25;
        uint16_t seg_torus = 25;

        for (int i = 0; i < seg_torus; i++)
        {
            for (int j = 0; j < seg_tube; j++)
            {
                double px = (R_torus + R_tube * cos(2.0 * PI * j / double(seg_tube))) * cos(2.0 * PI * i / double(seg_torus));
                double py = (R_torus + R_tube * cos(2.0 * PI * j / double(seg_tube))) * sin(2.0 * PI * i / double(seg_torus));
                double pz = R_tube * sin(2.0 * PI * j / double(seg_tube));

                positions.push_back(glm::vec3(px, py, pz));
                // colors.push_back(glm::vec3( 1.0f , j*1.0/seg_tube,  j*1.0/seg_tube));
                colors.push_back(glm::vec3(i % 2, j % 5 / 5, j % 10 / 10));
                normals.push_back(glm::vec3(cos(2.0 * PI * j / double(seg_tube)) * cos(2.0 * PI * i / double(seg_torus)),
                                            cos(2.0 * PI * j / double(seg_tube)) * sin(2.0 * PI * i / double(seg_torus)),
                                            sin(2.0 * PI * j / double(seg_tube))));

                uint32_t id_v0 = (i * seg_tube) + j;
                uint32_t id_v1 = (i * seg_tube) + (j + 1) % seg_tube;
                uint32_t id_v2 = ((i + 1) % seg_torus * seg_tube) + j;
                uint32_t id_v3 = ((i + 1) % seg_torus * seg_tube) + (j + 1) % seg_tube;

                faces.push_back(glm::uvec3(id_v0, id_v1, id_v2));
                faces.push_back(glm::uvec3(id_v2, id_v1, id_v3));
            }
        }

        // for (int i = 0; i < positions.size(); i++)
        // {
        //     positions[i] = positions[i] +  normals[i] * float(offset);
        // }

    }

    void Torus::draw()
    {
        if (settingPtr->wire)
        {
            // glEnable(GL_BLEND);
            // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
            // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            // set wire-frame rendering mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(1.0);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // glPointSize(7.0);
        }

        glBindVertexArray(vaoID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

        glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        if (settingPtr->wire)
        {
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }
    }
}
