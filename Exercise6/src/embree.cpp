#include "embree.h"

#include <math.h>
#include <iostream>
#include <random>

void embree_error(void * ptr, RTCError error, const char * str)
{
	fprintf(stderr, "EMBREE ERROR: %s\n", str);
}

embree::embree()
{
	device = rtcNewDevice(NULL);
	rtcSetDeviceErrorFunction(device, embree_error, NULL);

	scene = rtcNewScene(device);
}

embree::~embree()
{
	rtcReleaseScene(scene);
	rtcReleaseDevice(device);
}

void embree::build_bvh()
{
	rtcCommitScene(scene);
}

unsigned embree::add_mesh(const Shape & mesh)
{
	const glm::mat4 & model_matrix = mesh.getModelMatrix();

	RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

	face_num.push_back(mesh.faces.size());
	vertex_num.push_back(mesh.positions.size());

	// positions = mesh.positions;
	glm::vec3 * vertices = (glm::vec3 *) rtcSetNewGeometryBuffer(geom,
		RTC_BUFFER_TYPE_VERTEX, 0,
		RTC_FORMAT_FLOAT3, 3 * sizeof(float),
		mesh.positions.size());

	// faces = mesh.faces;
	glm::uvec3 * tri_idxs = (glm::uvec3 *) rtcSetNewGeometryBuffer(geom,
		RTC_BUFFER_TYPE_INDEX, 0,
		RTC_FORMAT_UINT3, 3 * sizeof(int),
		mesh.faces.size());

	// colors = mesh.colors;
	
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));

	// for (unsigned i = 0; i < normals.size(); i++)
	// 	normals.push_back(normal_matrix * mesh.normals[i]);

	for (unsigned i = 0; i < mesh.positions.size(); i++){
		vertices[i] = glm::vec3(model_matrix * glm::vec4(mesh.positions[i], 1.f));
		normals.push_back(glm::normalize(normal_matrix * mesh.normals[i]));
		colors.push_back(mesh.colors[i]);
		positions.push_back(vertices[i]);
	}


	for (unsigned i = 0; i < mesh.faces.size(); i++){
		tri_idxs[i] = mesh.faces[i];
		faces.push_back( mesh.faces[i]);
	}
	

	rtcCommitGeometry(geom);

	unsigned geom_id = rtcAttachGeometry(scene, geom);
	rtcReleaseGeometry(geom);

	return geom_id;
}

bool embree::intersect(ray_hit & r)
{
	RTCIntersectContext context;
	rtcInitIntersectContext(&context);
	rtcIntersect1(scene, &context, &r);

	return r.hit.primID != -1;
}

