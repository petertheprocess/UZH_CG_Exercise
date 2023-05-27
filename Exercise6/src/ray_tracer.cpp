#include "ray_tracer.h"
#include "stdlib.h"
#include "iostream"
#define EPSILON 0.00000001f
// TODO: play with the samples number, add a input control in the UI (user interface, ImGui window)
int ray_tracer::samples = 4;

void ray_tracer::raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam)
{
	glm::mat4 inv_proj_view = glm::inverse(cam.getViewProjectionMatrix());

#pragma omp parallel for
	for (int i = 0; i < window_size.x; ++i)
		for (int j = 0; j < window_size.y; ++j)
		{
			// row major
			float &depth = buffer[(window_size.y - j - 1) * window_size.x + i] = 0;
			glm::vec3 dir = ray_view_dir({i, j}, window_size, inv_proj_view, cam.getPosition());

			for (int s = 0; s < samples; ++s)
				depth += intersect_depth(cam.getPosition(), dir);

			depth /= samples;
		}
}

glm::vec3 ray_tracer::ray_view_dir(const glm::ivec2 &pos, const glm::ivec2 &window_size, const glm::mat4 &inv_proj_view, const glm::vec3 &cam_pos)
{
	glm::vec2 ndc;
	ndc.x = (2.0f * pos.x) / window_size.x - 1.0f;
	ndc.y = (2.0f * pos.y) / window_size.y - 1.0f;
	glm::vec4 clip_space(ndc.x, ndc.y, -1.0f, 1.0f);
	glm::vec4 view_space = inv_proj_view * clip_space;
	glm::vec3 world_space(view_space.x / view_space.w, view_space.y / view_space.w, view_space.z / view_space.w);
	srand(1);
	glm::vec3 random_offset = {rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX)};

	glm::vec3 ray_view_dir = glm::normalize(world_space - cam_pos);
	// TODO: implement this function to compute the direction of primary rays with a random offset.
	// HINT: do this first, and test it with the embree tracer, then complete the rt_simple class.
	return ray_view_dir;
}

// TODO: add the definition of the methods here.
float rt_simple::intersect_triangle(const Triangle &tri, const glm::vec3 &org, const glm::vec3 &dir)
{
	float depth = -1;
	glm::vec3 normal = cross(tri.p1 - tri.p0, tri.p2 - tri.p0);
	float d = -dot(normal, tri.p1);
	// std::cout<<"d:"<<d<<std::endl;
	if (fabsf(dot(normal, dir)) < EPSILON)
		return depth;

	float t = (-d - dot(normal, org)) / dot(normal, dir);
	// std::cout<<"t:"<<t<<std::endl;
	// t<0 means opposite direction of dir
	if (t < 0)
		return depth;
	glm::vec3 p_intersect = org + t * dir;
	float dis = distance(p_intersect, org);
	
	// if (depth != NULL && dis > depth)
	// 	return depth;
	// std::cout<<"dis:"<<dis<<std::endl;
	if (dot(cross(tri.p1 - tri.p0, p_intersect - tri.p0), normal) < 0)
		return depth;
	if (dot(cross(tri.p2 - tri.p1, p_intersect - tri.p1), normal) < 0)
		return depth;
	if (dot(cross(tri.p0 - tri.p2, p_intersect - tri.p2), normal) < 0)
		return depth;
	// std::cout<<"depth:"<<depth<<std::endl;
	return depth = dis;
}

float rt_simple::intersect_bvh(BVHNode *node, const glm::vec3 &org, const glm::vec3 &dir)
{
	float depth = -1;
	if (node == nullptr)
		return depth;

	float t_min[3];
	float t_max[3];

	for (int i = 0; i < 3; i++)
	{
		t_min[i] = (node->boundsMin[i] - org[i]) / dir[i];
		t_max[i] = (node->boundsMax[i] - org[i]) / dir[i];
		if (t_min[i] > t_max[i])
			std::swap(t_max[i], t_min[i]);
	}

	float t_enter = std::max(std::max(t_min[0], t_min[1]), t_min[2]);
	float t_exit = std::min(std::min(t_max[0], t_max[1]), t_max[2]);
	// std::cout<<"t_enter"<<t_enter<<"t_exit"<<t_exit<<std::endl;
	
	if ( t_exit < t_enter || t_exit < 0)
	// 不能用== 因为最后的子节点包围盒实际上是一个很薄的平面，因为只包了一个三角形
		return depth;
	else if (node->triangleId >= 0) // 如果包围盒里有三角形
	{
		Triangle tri = triangles[node->triangleId];
		// std::cout<<node->triangleId<<","<<tri.p1.x<<std::endl;
		float self_depth = intersect_triangle(tri, org, dir);
		if (node->leftChild != nullptr)
		{
			float left_depth = intersect_bvh(node->leftChild, org, dir);
			if (left_depth > 0 && self_depth > 0)
			{
				depth = std::min(left_depth, self_depth);
			}
			else if (left_depth < 0 && self_depth > 0)
			{
				depth = self_depth;
			}
			else
			{
				depth = left_depth;
			}
		}else{
			depth = self_depth;
		}
	}
	else
	{
		float left_depth = intersect_bvh(node->leftChild, org, dir);
		float right_depth = intersect_bvh(node->rightChild, org, dir);
		if (left_depth > 0 && right_depth > 0)
		{
			depth = std::min(left_depth, right_depth);
		}
		else if (left_depth < 0 && right_depth > 0)
		{
			depth = right_depth;
		}
		else
		{
			depth = left_depth;
		}
	}

	return depth;
}

float rt_simple::intersect_depth(const glm::vec3 &org, const glm::vec3 &dir)
{
	float depth = intersect_bvh(rootNode, org, dir);
	// std::cout<<"depth:"<<depth<<std::endl;
	return depth > 0 ? depth : 0;
};

unsigned rt_simple::add_mesh(const Shape &mesh)
{
	model_matrix = mesh.getModelMatrix();
	unsigned count = 0;
	for (auto face : mesh.faces)
	{
		Triangle tri;
		tri.p0 = glm::vec3(model_matrix * glm::vec4(mesh.positions[face[0]], 1.0f));
		tri.p1 = glm::vec3(model_matrix * glm::vec4(mesh.positions[face[1]], 1.0f));
		tri.p2 = glm::vec3(model_matrix * glm::vec4(mesh.positions[face[2]], 1.0f));
		triangles.push_back(tri);
		// std::cout<<triangles[count].p0.x<<std::endl;
		count++;
	}
	return count;
}
// TODO: add the definition of the methods here.
