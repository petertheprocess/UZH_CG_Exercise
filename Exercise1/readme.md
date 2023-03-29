Tasks:

In this exercise, you have to generate a mesh with the shape of a Torus. Specifically, you need to write the code to compute all the vertices, vertex normals and faces of the mesh. Each vertex requires a 3D position value, a RGB color value and a 3D normal vector (for offsetting the positions later). Each face (or triangle) requires a triplet with the indices of its vertices.

After the torus has been created, it should be possible to move the positions along their normal by a specified offset (offset argument of the Torus constructor).

The source code of this project is the viewer from the first exercises extended with some functions to help you visualize the objects. An incomplete Torus class has been created already. Specifically you need to complete code pieces in the following classes:

- GLExample.cpp
- Torus.cpp

Example screenshots of the result can be seen in the doc folder. You are free to color you Torus however you want. The four numbers in the name of the example images describe (1) the number of segments on the torus, (2) the number of segments in the tube surface, (3) the radius of the torus (R) and (4) the radius of the tube (r).  

How to submit:

Once your code compiles and works, create a **.zip** file with the source code (please include only the source files, the **src/** folder) and a screenshot of the rendering window working on your computer. Also add a screenshot each with with an offset factor along the face normal above and below 0. Upload the zip-file to OLAT.

Learning targets:

- You are able to procedurally generate a simple object with triangles

- You know how to enable/disable face culling in OpenGL 

- You know how to change the clockwise and counterclockwise ordering of triangle geometry in OpenGL 

- You know how to change the culling mode (frontface vs. backface culling in OpenGL)

- You know how to render a model in wireframe mode

- You know the difference between vertex shader fragment shader

- You know what vertex normals are and how the movement along them changes the shape of the torus 

