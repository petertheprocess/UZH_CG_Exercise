//
//  inputModule.h
//  CubeViewer
//
//  Created by Renato Pajarola.
//
//

#ifndef INPUTMODULE_H
#define INPUTMODULE_H

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// set initial viewer position and model transformation parameters
#define OFFSET -5.0
#define ANGLE_X  25
#define ANGLE_Y -25

// this must be packed/padded accordingly for a uniform shader block
typedef struct {
  GLuint wire;
  GLuint cull;
} RenderSettings;


#endif /* defined(INPUTMODULE_H) */









