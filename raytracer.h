// Raytracer header file that declares the ray tracer
// Author: Sasidharan Mahalingam
// Date Created: 2 Dec 2023
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"

class RayTracer
{
public:
	Color trace(const Ray &ray, const Scene &scene, int depth, int i, int j);

	Ray generateRay(const Camera &camera, int i, int j, int height, int width);

	bool getIntersection(const Ray &ray, const Scene &scene, const Primitive *&hit_primitive, vec3 *hit_point);

	Color calcLight(const Light &light, const Primitive *hit_primitive, const Ray &ray, const vec3 &hit_point, const float *attenuation);

	Ray transformRay(const Ray &ray, const Primitive *primitive);

	Ray createReflectRay(const Ray &ray, const vec3 &hit, const vec3 &unit_normal);
};

#endif