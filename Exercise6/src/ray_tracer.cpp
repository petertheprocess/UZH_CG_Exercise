#include "ray_tracer.h"
#include "stdlib.h"
#define EPSILON 0.00000001f
// TODO: play with the samples number, add a input control in the UI (user interface, ImGui window)
int ray_tracer::samples = 4;


void ray_tracer::raycasting(float * buffer, const glm::ivec2 & window_size, const Camera & cam)
{
	glm::mat4 inv_proj_view = glm::inverse(cam.getViewProjectionMatrix());

	#pragma omp parallel for
	for(int i = 0; i < window_size.x; ++i)
	for(int j = 0; j < window_size.y; ++j)
	{
		//row major
		float & depth = buffer[(window_size.y - j - 1) * window_size.x + i] = 0;
		glm::vec3 dir = ray_view_dir({i, j}, window_size, inv_proj_view, cam.getPosition());

		for(int s = 0; s < samples; ++s)
			depth += intersect_depth(cam.getPosition(), dir);

		depth /= samples;
	}
}


glm::vec3 ray_tracer::ray_view_dir(const glm::ivec2 & pos, const glm::ivec2 & window_size, const glm::mat4 & inv_proj_view, const glm::vec3 & cam_pos)
{
	glm::vec2 ndc;
	ndc.x = (2.0f * pos.x) / window_size.x - 1.0f;
	ndc.y =(2.0f * pos.y) / window_size.y - 1.0f;
	glm::vec4 clip_space(ndc.x, ndc.y, -1.0f, 1.0f);
	glm::vec4 view_space = inv_proj_view * clip_space;
	glm::vec3 world_space(view_space.x / view_space.w, view_space.y / view_space.w, view_space.z / view_space.w);
	srand(1);
	glm::vec3 random_offset = {rand() / double(RAND_MAX),rand() / double(RAND_MAX),rand() / double(RAND_MAX)};

	
	glm::vec3 ray_view_dir = world_space - cam_pos;
	// TODO: implement this function to compute the direction of primary rays with a random offset.
	// HINT: do this first, and test it with the embree tracer, then complete the rt_simple class.
	return ray_view_dir;
}

// TODO: add the definition of the methods here.
float rt_simple::intersect_depth(const glm::vec3 & org, const glm::vec3 & dir){
	float depth = NULL;
	for(auto face:faces){
		glm::vec3 p0 = glm::mat3(model_matrix)*positions[face.x];
		glm::vec3 p1 = glm::mat3(model_matrix)*positions[face.y];
		glm::vec3 p2 = glm::mat3(model_matrix)*positions[face.z];

		glm::vec3 normal = cross(p1-p0,p2-p0);
		float d = -dot(normal,p1);
		if(fabsf(dot(normal,dir))<EPSILON) continue;
		
		float t = (-d - dot(normal,org) )/dot(normal,dir);
		// t<0 means opposite direction of dir
		if(t<0) continue;
		glm::vec3 p_intersect = org+t*dir;
		float dis = distance(p_intersect,org);
		if(depth!=NULL && dis>depth) continue;

		if (dot(cross(p1-p0, p_intersect-p0),normal) < 0 ) continue;
  		if (dot(cross(p2-p1, p_intersect-p1),normal) < 0 ) continue;
  		if (dot(cross(p0-p2,p_intersect-p2),normal) < 0 ) continue;

		depth = dis;
	}

	return depth;
};

unsigned rt_simple::add_mesh(const Shape & mesh){
	model_matrix = mesh.getModelMatrix();
	for(auto position:mesh.positions){
		positions.push_back(glm::mat3(model_matrix)*position);
	}
	faces.insert(faces.end(),mesh.faces.begin(),mesh.faces.end());
}
// TODO: add the definition of the methods here.
