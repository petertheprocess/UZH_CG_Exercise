## OpengGL - Exercise 3

### Subject / Goals:

In the first task, apply the Gouraud and Phong lighting model using ambient, diffuse, and specular terms to the Cube and Torus. In the second task, apply texture mapping (including normal mapping) to deepen your knowledge further. It is advisable to first implement the first task and then the second task without normal mapping before starting with the normals.

### Tasks:

Note that the *todos* in the code are labeled according to the relevant task.

#### First Task

- Implement the Gouraud and Phong lighting model for the BasicCube.
- Combine screenshots of each term (ambient, diffuse, specular) for each lightning model, so eight screenshots in total.
- [optional] Apply Blinn-Phong illumination, make a screenshot and compare it to the Phong illumination.

Files with todos: BasicShape.frag, BasicShape.vert, GLExample.cpp

#### Second Task

- Apply texture mapping to the cube and the torus.
- Apply a light map to the cube and the torus object.
- Combine the texture mapping with the previously implemented (Blinn-)Phong illumination model.
- Apply normal mapping and use the normal texture provided
- Make screenshots of all the results during the texturing process (texture coordinates, texture only, texture and light map applied).
- [optional] Find a texture, specular map and normal map on the internet which you can use for this exercise - or create them on your own

Files with todos: TexturedShape.frag, TexturedShape.vert, Cube.cpp, Torus.cpp, GLExample.cpp, Texture.cpp, TexturedNormals.vert

### How to Submit:

Create a **.zip** file with the source code (please include only the source files, the **src/** and **shader/** folders), and upload it to OLAT with the screenshots requested.

### Learning Targets:

- You know how to apply basic Phong lighting to a scene in the shader
- You know the difference between Gouraud shading and Phong shading.
- You know how shading models and lighting models differ.
- You know how to set uniforms to a shader
- You know how to load a texture with OpenGL 
- You know how to apply texture mapping in a shader program
- You know how to apply a light map to the (Blinn-)Phong model
- You know how to set uniforms for samplers in GLSL shaders
- You know how to load normal mapping with OpenGL
- You know how to generate tangents, bitangents, and the TBN matrix

### Setup

To load images, we will use cimg in this exercise. The setup script should handle everything correctly. However, on MacOS and Windows, you have to install imagemagick additionally. 

You can do so on MacOS with *brew* on by running: 

	brew install imagemagick

On Windows you can download imagemagick from the imagemagick website. The recommended version is fine. When installing, select also "Install development headers and libraries for C and C++".

If you are a Linux user, you may have to install imagemagick or graphicsmagick using your package manager. For Debian/Ubuntu users:

	sudo apt install graphicsmagick

### Additional Information:

This exercise renders two objects (a Torus and a Cube). Depending on the shader and uniforms, your task is implementing different lighting models. In the running application, you can select which technique should use for shading. Additionally, you can show the Torus' normals. Note that showing the normals when using the textures has to be completed by you. In the running application, you can also change the color of the light.

#### Task 1

The starting point for this task are the BasicShape shaders. Here you should implement the Gouraud (lighting in the vertex shader) and Phong shading (lighting in the fragment shader). As you cannot set breakpoints in the shader, consider using the color output of the fragment shader for debugging purposes. Example screenshots can be found in the doc folder. For more information, we recommend the chapter on Basic Lighting on learnopengl.com and the chapter on Shaders.

#### Task 2

After you implement the Gouraud and Phong illumination, it is time to move to the second task. In this exercise, you will learn how to use textures to give the Torus and Cube a more detailed and interesting appearance. You can find two times three textures in the res folder: color, specular, and normal texture.

We suggest that you start with the texture mapping of the color and the specular one and ignore the normal map for a start; further, the Cube (Cube.cpp) is simpler to create the texture coordinates for. We used one texture per side for the cube and for the torus (Torus.cpp) we wrapped the texture four times around it. To then use the texture, first, you have to load the texture onto the GPU (Texture.cpp). To use the texture in the shaders, you have to bind the texture to the shader (GLExample.cpp). Then you can sample the value in the shader (TexturedShape.frag, TexturedShape.vert). On this topic, advisable resources can be found in the chapter of learnopengl.com, namely "Textures" and "Lighting maps."

The process is the same for the normal mapping except that you have to additionally compute the tangents for the Cube and Torus (Cube.cpp and Torus.cpp). Those tangents are used to map the object's normals to point in the right direction. For this, the generation of a TBN (tangent, bitangent, normal) is required. The learnopengl.com chapter Normal Mapping has a nice introduction to this topic. To check your solution you should complete the MultiLine rendering for the normals using the normal map. THis can be done in the TexturedNormals shaders. You might check the BasicNormals shader for inspiration.

### Links

- [Download Imagemagick on Windows](https://imagemagick.org/script/download.php)
- [Basic Lighting on learnopengl.com](https://learnopengl.com/Lighting/Basic-Lighting)
- [Shaders on learnopengl.com](https://learnopengl.com/Getting-started/Shaders)
- [Textures on learnopengl.com](https://learnopengl.com/Getting-started/Textures)
- [Lighting Maps on learnopengl.com](https://learnopengl.com/Lighting/Lighting-maps)
- [Normal Mapping on learnopengl.com](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
