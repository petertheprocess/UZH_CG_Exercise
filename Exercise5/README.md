## Physically Based Rendering - Exercise 5

### Subject / Goals:

In this exercise, you will implement the bidirectional reflective distribution functions (BRDF) for physically based rendering of a revolver. The Fresnel reflectance is already provided, the remaining terms have to be implemented by you.

### Task:

This exercise only requires changes in the fragment shader (pbr.frag). Before starting the exercise that a look at the variables and values that are available in this shader. The input to the fragment shader is sufficient to finish the exercise. In the doc folder you can find a video of how the finished version should look like.

In the fragment shader, you should implement the BRDFs for the specular and diffuse term. The Fresnel reflectance for the specular BRDF is already provided e.g. the open parts are: normal distribution function (NDF) and the joint masking-shadowing function for the specular BRDF. You also have to assemble the terms to get the specular BRDF. Further you have to implement the diffuse BRDF.

Note that the tone as well as gamma correction are also provided. Additionally a small ambient term is added at the start. To keep the code understandable you should implement the different parts of the BRDF in their own methods in the shader. Do not forget to maintain energy conservation between the two BRDFs.

When finished, create a screenshot of the result.

Files with todos: pbr.frag

### How to Submit:

Create a **.zip** file with the source code (please include only the source files, the **src/** and **shader/** folders), and upload it to OLAT with the screenshots requested.

### Learning Targets:

- You know the fundamentals of physically based rendering (PBR)
- You know the different parts of the BRDFs for the specular and the diffuse term
- You know how to implement the NDF and the joint shadow function for the specular BRDF
- You know how to maintain energy conservation in PBR
- You can write readable complex shader using functions

### Setup

To load textures, we will use cimg in this exercise. The setup script should handle everything correctly. However, on MacOS and Windows, you have to install imagemagick additionally. 

You can do so on MacOS with *brew* on by running: 

	brew install imagemagick

On Windows you can download imagemagick from the imagemagick website. The recommended version is fine. When installing, select also "Install development headers and libraries for C and C++".

If you are a Linux user, you may have to install imagemagick or graphicsmagick using your package manager. For Debian/Ubuntu users:

	sudo apt install graphicsmagick

As some students reported issues witch cimg when using xcode, we encourage mac users to either use CLion or Unix Makefiles.

If you have any problems building the project do not hesitate to post in the forum with your operating system, IDE and error message.

### Additional Information:

Differently to the previous exercises, in this one you can change the camera. Use this to explore the model and the rendering. In the doc folder a video is provided illustrating how the final rendered revolver should look like.

### Links

- [Download Imagemagick on Windows](https://imagemagick.org/script/download.php)
- [PBR on learnopengl.com](https://learnopengl.com/PBR/Theory)
