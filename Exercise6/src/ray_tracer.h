#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "embree.h"
#include "Cube.h"
#include <iostream>

class ray_tracer
{
public:
	static int samples;
	std::vector<glm::vec3> colors;

public:
	ray_tracer() = default;
	~ray_tracer() = default;

	virtual float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) = 0;
	// virtual glm::vec3 shade() = 0;
	virtual float intersect_depth_color(const glm::vec3 &org, const glm::vec3 &dir, glm::vec3 &color, glm::vec3 &normal) = 0;

	void raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam, const Cube &lightbox);
	glm::vec3 shade(glm::vec3 normal, glm::vec3 color, glm::vec3 light_dir, glm::vec3 light_color, glm::vec3 view_dir, bool shadow);
	glm::vec3 ray_view_dir(const glm::ivec2 &pos, const glm::ivec2 &window_size, const glm::mat4 &inv_proj_view, const glm::vec3 &cam_pos);
	glm::vec3 barycentricInterpolation(const glm::vec3 &p, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &color0, const glm::vec3 &color1, const glm::vec3 &color2)
	{
		glm::vec3 e0 = v2 - v1;
		glm::vec3 e1 = v2 - v0;

		glm::vec3 p2 = p - v2;

		float denom = glm::length(glm::cross(e0, e1));

		float factor1 = glm::length(glm::cross(e1, p2)) / denom;
		float factor0 = glm::length(glm::cross(e0, p2)) / denom;
		float factor2 = 1 - factor0 - factor1;

		return factor0 * color0 + factor1 * color1 + factor2 * color2;
	}
};

// TODO: this ray tracer implementation compute the ray triangle intersection with all the triangles in the scene.
class rt_simple : public ray_tracer
{
	// TODO: add possible data members and class methods.
private:
	// glm::mat4 model_matrix;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
	};

	struct Triangle
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
	};

	std::vector<Triangle> triangles;

	struct BVHNode
	{
		glm::vec3 boundsMin;
		glm::vec3 boundsMax;
		std::vector<int> triIds;
		BVHNode *leftChild;
		BVHNode *rightChild;
	};

	BVHNode *rootNode;

	unsigned add_mesh(const Shape &mesh);
	float intersect_bvh(BVHNode *node, const glm::vec3 &org, const glm::vec3 &dir, int &intersect_triID);
	float intersect_triangle(const Triangle &tri, const glm::vec3 &org, const glm::vec3 &dir);
	float intersect_depth_color(const glm::vec3 &org, const glm::vec3 &dir, glm::vec3 &color, glm::vec3 &normal);
	void computeBoundingBox(const std::vector<int> &triIds, glm::vec3 &boundsMin, glm::vec3 &boundsMax);
	BVHNode *buildBVH(std::vector<Triangle> &triangles, int start, int end, int axis);

public:
	// TODO: complete the definition of this method.
	rt_simple(const std::vector<Shape *> &shapes)
	{
		for (auto &mesh : shapes)
			add_mesh(*mesh);
		std::cout << "-----------add_mesh" << std::endl;
		std::cout << "triNum:" << triangles.size() << std::endl;
		rootNode = buildBVH(triangles, 0, triangles.size() - 1, 0);

		// preorderTraversal(rootNode);
		std::cout << "buildBVH" << std::endl;
	}
	float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir);
};

class rt_embree : public ray_tracer, public embree
{

public:
	rt_embree(const std::vector<Shape *> &shapes)
	{
		for (auto &mesh : shapes)
			add_mesh(*mesh);
		ray_tracer::colors = embree::colors;
		build_bvh();
	}

	float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir)
	{
		ray_hit r(org, dir);
		return intersect(r) ? r.ray.tfar : 0.f;
	}

	float intersect_depth_color(const glm::vec3 &org, const glm::vec3 &dir, glm::vec3 &color, glm::vec3 &normal)
	{
		ray_hit r(org, dir);
		if (intersect(r))
		{
			float depth = r.ray.tfar;
			glm::vec3 pt = org + r.ray.tfar * dir;
			unsigned tri_id = r.hit.primID;
			unsigned geom_id = r.hit.geomID;
			unsigned vertex_offset = 0;
			for (int i = 0; i < geom_id; i++)
			{
				tri_id += face_num[i];
				vertex_offset += vertex_num[i];
			}

			glm::vec3 p0 = embree::positions[vertex_offset + embree::faces[tri_id][0]];
			glm::vec3 p1 = embree::positions[vertex_offset + embree::faces[tri_id][1]];
			glm::vec3 p2 = embree::positions[vertex_offset + embree::faces[tri_id][2]];
			glm::vec3 color0 = embree::colors[vertex_offset + embree::faces[tri_id][0]];
			glm::vec3 color1 = embree::colors[vertex_offset + embree::faces[tri_id][1]];
			glm::vec3 color2 = embree::colors[vertex_offset + embree::faces[tri_id][2]];
			glm::vec3 n0 = embree::normals[vertex_offset + embree::faces[tri_id][0]];
			glm::vec3 n1 = embree::normals[vertex_offset + embree::faces[tri_id][1]];
			glm::vec3 n2 = embree::normals[vertex_offset + embree::faces[tri_id][2]];
			// barycentric Interpolation
			color = barycentricInterpolation(pt, p0, p1, p2, color0, color1, color2);
			normal = barycentricInterpolation(pt, p0, p1, p2, n0, n1, n2);
			return depth;
		}
		else
		{
			return 0.0f;
		}
	}
};

#endif // RAY_TRACER_H
