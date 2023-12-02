// Scene header file that declares the scene of the ray tracer
// Author: Sasidharan Mahalingam
// Date Created: 2 Dec 2023
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <stack>
#include <sstream>
#include "primitives.h"

using namespace std;

void rightMultiply(const mat4 &M, stack<mat4> &transform_stack);

struct Camera
{
	vec3 eye;
	vec3 center;
	vec3 up;
	float fovy;

	Camera(const vec3 &eye_, const vec3 &center_, const vec3 &up_, const float &fovy_) : eye(eye_), center(center_), up(up_), fovy(fovy_) {}

	Camera() : eye(0.0, 0.0, 5.0), center(0.0, 0.0, 0.0), up(0.0, 1.0, 0.0) {}
};

struct Light
{
	vec3 pos_or_dir;
	vec3 transform;
	Color color;

	enum Type {directional, point};

	Type type;

	const vec3 &position() const;
	const vec3 &direction() const;
};

struct Scene
{
private:
	bool readvals (stringstream &s, const int numvals, float *values);

public:
	Scene();

	void readFile(const string &filename);
	string outputfile;

	Camera camera;

	int max_depth;

	int width, height;

	vector<Light> lights;

	Materials materials;
	float attenuation[3];

	vector<Primitive*> primitives;

	vector<vec3> vertex_buffer, vertex_buffer_with_normal, vertex_normal_buffer;
};

#endif