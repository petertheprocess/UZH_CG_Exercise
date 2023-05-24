#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "embree.h"


class ray_tracer
{
	public:
		static int samples;

	public:
		ray_tracer() = default;
		~ray_tracer() = default;

		virtual float intersect_depth(const glm::vec3 & org, const glm::vec3 & dir) = 0;

		void raycasting(float * buffer, const glm::ivec2 & window_size, const Camera & cam);
		glm::vec3 ray_view_dir(const glm::ivec2 & pos, const glm::ivec2 & window_size, const glm::mat4 & inv_proj_view, const glm::vec3 & cam_pos);
};


// TODO: this ray tracer implementation compute the ray triangle intersection with all the triangles in the scene.
class rt_simple: public ray_tracer
{
	// TODO: add possible data members and class methods.
	private:
	const std::vector<glm::vec3> & positions;
	const std::vector<glm::uvec3> & faces;
	unsigned add_mesh(const Shape & mesh);

	public:
		// TODO: complete the definition of this method.
		rt_simple(const std::vector<Shape *> & shapes){
			for(auto & mesh: shapes)
				add_mesh(*mesh);
		}
		float intersect_depth(const glm::vec3 & org, const glm::vec3 & dir);
};



class rt_embree: public ray_tracer, public embree
{
	public:
		rt_embree(const std::vector<Shape *> & shapes)
		{
			for(auto & mesh: shapes)
				add_mesh(*mesh);

			build_bvh();
		}

		float intersect_depth(const glm::vec3 & org, const glm::vec3 & dir)
		{
			ray_hit r(org, dir);
			return intersect(r) ? r.ray.tfar : 0.f;
		}
};


#endif // RAY_TRACER_H

