#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "embree.h"
#include <iostream>

class ray_tracer
{
public:
	static int samples;

public:
	ray_tracer() = default;
	~ray_tracer() = default;

	virtual float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) = 0;

	void raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam);
	glm::vec3 ray_view_dir(const glm::ivec2 &pos, const glm::ivec2 &window_size, const glm::mat4 &inv_proj_view, const glm::vec3 &cam_pos);
};

// TODO: this ray tracer implementation compute the ray triangle intersection with all the triangles in the scene.
class rt_simple : public ray_tracer
{
	// TODO: add possible data members and class methods.
private:
	glm::mat4 model_matrix;

	struct Triangle
	{
		glm::vec3 p0;
		glm::vec3 p1;
		glm::vec3 p2;
	};

	std::vector<Triangle> triangles;

	struct BVHNode
	{
		glm::vec3 boundsMin;
		glm::vec3 boundsMax;
		int triangleId;
		BVHNode *leftChild;
		BVHNode *rightChild;
	};

	BVHNode *rootNode;

	unsigned add_mesh(const Shape &mesh);
	float intersect_bvh(BVHNode *node, const glm::vec3 &org, const glm::vec3 &dir);
	float intersect_triangle(const Triangle &tri, const glm::vec3 &org, const glm::vec3 &dir);

	void computeBoundingBox(const Triangle &triangle, glm::vec3 &boundsMin, glm::vec3 &boundsMax)
	{
		boundsMin.x = std::min(std::min(triangle.p0.x, triangle.p1.x), triangle.p2.x);
		boundsMin.y = std::min(std::min(triangle.p0.y, triangle.p1.y), triangle.p2.y);
		boundsMin.z = std::min(std::min(triangle.p0.z, triangle.p1.z), triangle.p2.z);

		boundsMax.x = std::max(std::max(triangle.p0.x, triangle.p1.x), triangle.p2.x);
		boundsMax.y = std::max(std::max(triangle.p0.y, triangle.p1.y), triangle.p2.y);
		boundsMax.z = std::max(std::max(triangle.p0.z, triangle.p1.z), triangle.p2.z);
		// std::cout << "boundsBox" << std::endl;
	}

	BVHNode *buildBVH(std::vector<Triangle> &triangles, int start, int end, int axis)
	{

		if (start > end)
		{
			return nullptr;
		}
		BVHNode *node = new BVHNode;
		node->triangleId = -1;

		if (start == end)
		{
			// std::cout<<start<<":s=e:"<<end<<std::endl;
			// 叶节点，包含一个三角形
			node->leftChild = nullptr;
			node->rightChild = nullptr;
			node->triangleId = start;
			// std::cout << "make triId:" << node->triangleId << std::endl;
			computeBoundingBox(triangles[start], node->boundsMin, node->boundsMax);
		}
		else if (start + 1 == end)
		{
			// 叶节点，包含两个三角形
			node->leftChild = new BVHNode;
			node->rightChild = nullptr;
			node->triangleId = end;
			// std::cout << "make triId:" << node->triangleId << std::endl;
			node->leftChild->triangleId = start;
			node->leftChild->leftChild = nullptr;
			node->leftChild->rightChild = nullptr;
			// std::cout << "make triId:" << node->leftChild->triangleId << std::endl;
			computeBoundingBox(triangles[start], node->leftChild->boundsMin, node->leftChild->boundsMax);
			computeBoundingBox(triangles[end], node->boundsMin, node->boundsMax);
		}
		else
		{
			// 计算包围盒的中心点

			float boundsCenter = 0.0f;
			for (int i = start; i <= end; ++i)
			{
				float centroid = (triangles[i].p0[axis] + triangles[i].p1[axis] + triangles[i].p2[axis]) / 3.0f;
				boundsCenter += centroid;
			}
			boundsCenter /= (end - start + 1);

			// 根据中心点划分三角形
			int l_ind = start;
			int r_ind = end;

			while (l_ind < r_ind)
			{
				float l_centroid = (triangles[l_ind].p0[axis] + triangles[l_ind].p1[axis] + triangles[l_ind].p2[axis]) / 3.0f;
				float r_centroid = (triangles[r_ind].p0[axis] + triangles[r_ind].p1[axis] + triangles[r_ind].p2[axis]) / 3.0f;
				if (l_centroid > boundsCenter)
				{
					std::swap(triangles[l_ind], triangles[r_ind - 1]);
				}
				l_ind++;
				if (r_centroid <= boundsCenter)
				{
					std::swap(triangles[r_ind], triangles[l_ind + 1]);
				}
				r_ind--;
			}
			int mid = r_ind;
			// std::cout<<start<<"::"<<mid<<"::"<<end<<std::endl;
			// 递归构建左右子树
			node->leftChild = buildBVH(triangles, start, mid, (axis + 1) % 3);
			node->rightChild = buildBVH(triangles, mid + 1, end, (axis + 1) % 3);

			// 计算节点的包围盒
			
			for (int i = 0; i < 3; ++i)
			{
				// std::cout<<"bounds"<<std::endl;
				node->boundsMin[i] = std::min(node->leftChild->boundsMin[i], node->rightChild->boundsMin[i]);
				node->boundsMax[i] = std::max(node->leftChild->boundsMax[i], node->rightChild->boundsMax[i]);
			}

		}

		return node;
	}
	void preorderTraversal(BVHNode *root)
	{
		// std::cout << "root->triangleId"
		// 		  << " "<< std::endl;
		if (root == nullptr )
		{
			return;
		}
		std::cout<< root->triangleId<<std::endl;					 // 打印当前节点的值
		preorderTraversal(root->leftChild);	 // 遍历左子树
		preorderTraversal(root->rightChild); // 遍历右子树
	};

public:
	// TODO: complete the definition of this method.
	rt_simple(const std::vector<Shape *> &shapes)
	{
		for (auto &mesh : shapes)
			add_mesh(*mesh);
		std::cout << "-----------add_mesh" << std::endl;
		std::cout << "triNum:" << triangles.size() << std::endl;
		rootNode = buildBVH(triangles, 0, triangles.size() - 1, 0);
		
		preorderTraversal(rootNode);
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

		build_bvh();
	}

	float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir)
	{
		ray_hit r(org, dir);
		return intersect(r) ? r.ray.tfar : 0.f;
	}
};

#endif // RAY_TRACER_H
