## Exercise 6: Ray Tracing

### Subject / Goals:

Learn the ray tracing basics and implement a simple ray tracer for a set of triangular meshes.

### Tasks:

#### Basic (3 points)
- Complete the rt_simple class; this class implements a simple tracer that computes the ray intersection operation with all the triangles. The result is saved as a depth image, the distances to the closest hit; see the expected result image in the doc folder.
- Extend both rt_simple and rt_embree classes to render the scene with shadows and without textures, like a screenshot. For this, you must launch secondary rays toward the light source, called shadow rays.

### Display (3 points)
- Instead of rendering to an image, do the necessary modifications to switch from OpenGL to a Ray Tracer rendering in the window. It means you must display the buffer and render the scene per frame

### How to Submit:

Create a **.zip** file with the source code (please include only the source files, the **src/** and **shader/** folders), and upload it to OLAT with a screenshot of the rendering window working on your computer. 

### Learning Targets:

- Implement the ray triangle intersection.
- Implement a simple ray tracer and compare it with the Intel Embree implementation.
- Render a scene with primary and shadow rays.
- Learn about the complexity of the operations and the advantage of using a tracer with an acceleration structure for the ray intersection operations.

### Additional Information:

[https://www.embree.org/](https://www.embree.org/).

