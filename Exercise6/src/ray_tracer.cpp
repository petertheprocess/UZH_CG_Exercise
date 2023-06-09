#include "ray_tracer.h"
#include "stdlib.h"
#include "iostream"
#define EPSILON 0.00000001f
#define OFFSET_SHADOW 0.1f
// TODO: play with the samples number, add a input control in the UI (user interface, ImGui window)
int ray_tracer::samples = 1;

void ray_tracer::raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam, const Cube &lightbox)
{
	glm::mat4 inv_proj_view = glm::inverse(cam.getViewProjectionMatrix());

#pragma omp parallel for
	for (int i = 0; i < window_size.x; ++i)
		for (int j = 0; j < window_size.y; ++j)
		{
			glm::vec3 color = glm::vec3(0.0f); 
			glm::vec3 normal = glm::vec3(0.0f); 
			glm::vec3 shade_color = glm::vec3(0.0f); 
			float depth = 0;
			// row major
			// ind index = (i * window_size.x + window_size.y - j - 1) * 3;
			float &colorR = buffer[(window_size.y - j - 1) * window_size.x + i] = 0;
			float &colorG = buffer[window_size.x*window_size.y + (window_size.y - j - 1) * window_size.x + i] = 0;
			float &colorB = buffer[2*(window_size.x*window_size.y) + (window_size.y - j - 1) * window_size.x + i] = 0;

			glm::vec3 dir = ray_view_dir({i, j}, window_size, inv_proj_view, cam.getPosition());
			

			for (int s = 0; s < samples; ++s){
				glm::vec3 sample_dir = ray_view_dir({i, j}, window_size, inv_proj_view, cam.getPosition());
				glm::vec3 color_sample = glm::vec3(0);
				glm::vec3 normal_sample = glm::vec3(0);
				depth += intersect_depth_color(cam.getPosition(), sample_dir, color_sample, normal_sample );
				color += color_sample;
				normal += normal_sample;
			}

			color /= samples;
			depth /= samples;
			normal /= samples;
			normal = glm::normalize(normal);
			// 对于每个hit到目标的像素，我们存下他的depth, normal 和 color
			// 然后如果之后物体和相机位置不变，我们可以直接取这些数据进行阴影和着色计算

			if (depth > 0)
			{
				// add shadow ray
				glm::vec3 p_world = depth*dir+cam.getPosition();
				glm::vec3 shadow_dir = glm::normalize(p_world-lightbox.getPosition());
				float dis_to_light = glm::distance(p_world,lightbox.getPosition());
				float depth_shadow = intersect_depth(lightbox.getPosition(), shadow_dir);
				bool shadow = (fabsf(depth_shadow-dis_to_light)>OFFSET_SHADOW);

				// apply illumination
				shade_color = shade(normal,color,-shadow_dir,glm::vec3(1.0f),-dir,shadow);
			}
			colorR = shade_color.x;
			colorG = shade_color.y;
			colorB = shade_color.z;
		}
}


glm::vec3 ray_tracer::shade(glm::vec3 normal, glm::vec3 color, glm::vec3 light_dir, glm::vec3 light_color, glm::vec3 view_dir, bool shadow){
	glm::vec3 ambient = color * light_color * 0.3f;
	glm::vec3 diffuse = float(std::max(glm::dot(normal,light_dir),0.0f)) * light_color * color;
	glm::vec3 half_vec = glm::normalize(light_dir + view_dir);
	float specDot = float(std::max(glm::dot(half_vec,normal),0.0f));
	glm::vec3 specular = float(std::pow(specDot, 32)) * light_color * color;
	float shadow_factor = shadow? 0.0f:1.0f;
	
	return ambient+shadow_factor*(diffuse+specular);
	// return color;
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
	float rand_factor = 0.0001;
	glm::vec3 random_offset = {rand() / double(RAND_MAX) * rand_factor, rand() / double(RAND_MAX) * rand_factor, rand() / double(RAND_MAX) * rand_factor};

	glm::vec3 ray_view_dir = glm::normalize(world_space - cam_pos + random_offset);
	// TODO: implement this function to compute the direction of primary rays with a random offset.
	// HINT: do this first, and test it with the embree tracer, then complete the rt_simple class.
	return ray_view_dir;
}

// TODO: add the definition of the methods here.
float rt_simple::intersect_triangle(const Triangle &tri, const glm::vec3 &org, const glm::vec3 &dir)
{
	float depth = -1;
	glm::vec3 normal = cross(tri.v1.position - tri.v0.position, tri.v2.position - tri.v0.position);
	float d = -dot(normal, tri.v1.position);
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
	if (dot(cross(tri.v1.position - tri.v0.position, p_intersect - tri.v0.position), normal) < 0)
		return depth;
	if (dot(cross(tri.v2.position - tri.v1.position, p_intersect - tri.v1.position), normal) < 0)
		return depth;
	if (dot(cross(tri.v0.position - tri.v2.position, p_intersect - tri.v2.position), normal) < 0)
		return depth;
	// std::cout<<"depth:"<<depth<<std::endl;
	return depth = dis;
}

float rt_simple::intersect_bvh(BVHNode *node, const glm::vec3 &org, const glm::vec3 &dir, int &intersect_triID)
{
	float depth = -1;
	intersect_triID = -1;
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

	if (t_exit <= t_enter || t_exit < 0)
		return depth;
	if (node->triIds.size() > 0)
	{
		for (auto triId : node->triIds)
		{
			float tri_depth = intersect_triangle(triangles[triId], org, dir);
			if (tri_depth > 0 && (tri_depth < depth || depth < 0))
			{
				depth = tri_depth;
				intersect_triID = triId;
			}
		}
	}
	else
	{	
		int left_triID = -1;
		int right_triID = -1;
		float left_depth = intersect_bvh(node->leftChild, org, dir, left_triID);
		float right_depth = intersect_bvh(node->rightChild, org, dir, right_triID);
		if (left_depth > 0 && right_depth > 0)
		{
			depth = std::min(left_depth, right_depth);
			if(left_depth > right_depth){
				depth = right_depth;
				intersect_triID = right_triID;
			}else{
				depth = left_depth;
				intersect_triID = left_triID;				
			}
		}
		else if (left_depth < 0 && right_depth > 0)
		{
			depth = right_depth;
			intersect_triID = right_triID;
		}
		else
		{
			depth = left_depth;
			intersect_triID = left_triID;
		}
	}

	return depth;
}

float rt_simple::intersect_depth(const glm::vec3 &org, const glm::vec3 &dir)
{
	int triID = -1;
	float depth = intersect_bvh(rootNode, org, dir, triID);
	// std::cout<<"depth:"<<depth<<std::endl;
	return depth > 0 ? depth : 0;
};

float rt_simple::intersect_depth_color(const glm::vec3 &org, const glm::vec3 &dir, glm::vec3 &color, glm::vec3 &normal)
{
	int triID = -1;
	float depth = intersect_bvh(rootNode, org, dir, triID);
	if(depth>0){
		Triangle tri = triangles[triID];
		glm::vec3 pt = org + depth * dir;
		color = barycentricInterpolation(pt,tri.v0.position,tri.v1.position,tri.v2.position,tri.v0.color,tri.v1.color,tri.v2.color);
		normal = barycentricInterpolation(pt,tri.v0.position,tri.v1.position,tri.v2.position,tri.v0.normal,tri.v1.normal,tri.v2.normal);
		return depth;
	}else{
		return -1;
	}
}

unsigned rt_simple::add_mesh(const Shape &mesh)
{
	glm::mat4 model_matrix = mesh.getModelMatrix();
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
	unsigned count = 0;
	for (auto face : mesh.faces)
	{
		Triangle tri;
		tri.v0.position = glm::vec3(model_matrix * glm::vec4(mesh.positions[face[0]], 1.0f));
		tri.v1.position = glm::vec3(model_matrix * glm::vec4(mesh.positions[face[1]], 1.0f));
		tri.v2.position = glm::vec3(model_matrix * glm::vec4(mesh.positions[face[2]], 1.0f));
		tri.v0.color = mesh.colors[face[0]];
		tri.v1.color = mesh.colors[face[1]];
		tri.v2.color = mesh.colors[face[2]];
		tri.v0.normal = normal_matrix*mesh.normals[face[0]];
		tri.v1.normal = normal_matrix*mesh.normals[face[1]];
		tri.v2.normal = normal_matrix*mesh.normals[face[2]];
		triangles.push_back(tri);
		// std::cout<<triangles[count].v0.position.x<<std::endl;
		count++;
	}
	return count;
}

void rt_simple::computeBoundingBox(const std::vector<int> &triIds, glm::vec3 &boundsMin, glm::vec3 &boundsMax)
{
	if (triIds.size() == 0)
	{
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		boundsMin[i] = std::min(std::min(triangles[triIds[0]].v0.position[i], triangles[triIds[0]].v1.position[i]), triangles[triIds[0]].v2.position[i]);
		boundsMax[i] = std::max(std::max(triangles[triIds[0]].v0.position[i], triangles[triIds[0]].v1.position[i]), triangles[triIds[0]].v2.position[i]);
		for (auto triId : triIds)
		{
			float min_tmp = std::min(std::min(triangles[triId].v0.position[i], triangles[triId].v1.position[i]), triangles[triId].v2.position[i]);
			float max_tmp = std::max(std::max(triangles[triId].v0.position[i], triangles[triId].v1.position[i]), triangles[triId].v2.position[i]);
			boundsMin[i] = std::min(min_tmp, boundsMin[i]);
			boundsMax[i] = std::max(max_tmp, boundsMax[i]);
		}

		if (boundsMax[i] - boundsMin[i] < 0.1)
		{
			boundsMax[i] += 0.1;
			boundsMin[i] -= 0.1;
		}
	}
}

rt_simple::BVHNode *rt_simple::buildBVH(std::vector<Triangle> &triangles, int start, int end, int axis)
{

	if (start > end)
	{
		return nullptr;
	}
	BVHNode *node = new BVHNode;
	if (end - start < 8)
	{
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		for (int ind = start; ind <= end; ind++)
		{
			node->triIds.push_back(ind);
		}
		computeBoundingBox(node->triIds, node->boundsMin, node->boundsMax);
	}
	else
	{
		// compute the center of BB
		float boundsCenter = 0.0f;
		for (int i = start; i <= end; ++i)
		{
			float centroid = (triangles[i].v0.position[axis] + triangles[i].v1.position[axis] + triangles[i].v2.position[axis]) / 3.0f;
			boundsCenter += centroid;
		}
		boundsCenter /= (end - start + 1);

		// split the triangles
		int l_ind = start;
		int r_ind = end;

		while (l_ind < r_ind)
		{
			float l_centroid = (triangles[l_ind].v0.position[axis] + triangles[l_ind].v1.position[axis] + triangles[l_ind].v2.position[axis]) / 3.0f;
			float r_centroid = (triangles[r_ind].v0.position[axis] + triangles[r_ind].v1.position[axis] + triangles[r_ind].v2.position[axis]) / 3.0f;
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
		// build child leaves
		node->leftChild = buildBVH(triangles, start, mid, (axis + 1) % 3);
		node->rightChild = buildBVH(triangles, mid + 1, end, (axis + 1) % 3);

		// compute AABB of the node
		for (int i = 0; i < 3; ++i)
		{
			// std::cout<<"bounds"<<std::endl;
			node->boundsMin[i] = std::min(node->leftChild->boundsMin[i], node->rightChild->boundsMin[i]);
			node->boundsMax[i] = std::max(node->leftChild->boundsMax[i], node->rightChild->boundsMax[i]);
		}
	}

	return node;
}
