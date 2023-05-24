#ifndef EMBREE_H
#define EMBREE_H

#include <embree3/rtcore.h>

#include "Camera.h"
#include "Shape.h"

#include <map>
#include <memory>

using namespace cgCourse;


struct ray_hit : public RTCRayHit
{
	ray_hit(const glm::vec3& origin = glm::vec3(0.0f),
		const glm::vec3& direction = glm::vec3(0.0f),
		float near = 0.0f,
		float far = 100)
	{
		ray.org_x = origin.x;
		ray.org_y = origin.y;
		ray.org_z = origin.z;
		ray.tnear = near;

		ray.dir_x = direction.x;
		ray.dir_y = direction.y;
		ray.dir_z = direction.z;
		ray.time = 0.0f;

		ray.tfar = far;
		ray.mask = 0;
		ray.flags = 0;

		hit.primID = RTC_INVALID_GEOMETRY_ID;
		hit.geomID = RTC_INVALID_GEOMETRY_ID;
	}

	const glm::vec3 org() const
	{
		return { ray.org_x, ray.org_y, ray.org_z };
	}

	const glm::vec3 dir() const
	{
		return { ray.dir_x, ray.dir_y, ray.dir_z };
	}

	const glm::vec3 normal() const
	{
		return { hit.Ng_x, hit.Ng_y, hit.Ng_z };
	}
};


class embree
{
	RTCDevice device;
	RTCScene scene;

	public:
		embree();
		~embree();

		void build_bvh();
		unsigned add_mesh(const Shape & mesh);

		bool intersect(ray_hit & r);
};


#endif // EMBREE_H

