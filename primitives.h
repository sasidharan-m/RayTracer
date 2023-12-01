#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace glm;
typedef unsigned char BYTE;

extern const float eps;
int sgn(float x);
bool IsSameVector(const vec3& a, const vec3& b);

struct Ray
{
	vec3 o;
	vec3 direction;
	Ray(const vec3& o_, const vec3& direction_) : o(o_), direction(direction_) {}
};

struct Color
{
	float r, g, b;
	Color() : r(0), g(0), b(0) {}
	Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}
	BYTE Rbyte() const { return std::min(r, 1.0f) * 255; }
	BYTE Gbyte() const { return std::min(g, 1.0f) * 255; }
	BYTE Bbyte() const { return std::min(b, 1.0f) * 255; }
	bool operator == (const Color& otherColor) const;
	Color operator * (const Color& otherColor) const;
	Color operator + (const Color& otherColor) const;
	Color operator * (const float scale) const;
	bool isZero() const;
};

const Color BLACK(0, 0, 0);
const Color WHITE(1.0, 1.0, 1.0);

struct Materials
{	
	Color diffuse; 
	Color specular;
	Color emission; 
	Color ambient;
	float shininess;
	Materials();
};

class Primitive
{
public:
	mat4 transform; 
  	mat4 InversedTransform;
  	Materials materials;	
    
    int index; // Identify the object for debugging.
    
    enum shape {triangle, sphere} ;
    shape type; 
    
	Object();
	virtual ~Object();
	virtual bool Intersect(const Ray& ray, float* dis_to_ray) const = 0;
	virtual vec3 InterpolatePointNormal(const vec3& point) const = 0;

};


class Sphere : public Primitive 
{
public:
	Svec3 o;
	float r;
	Sphere(const vec3& _o, const float& _r);
	
	virtual ~Sphere();
	virtual bool Intersect(const Ray& ray, float* dis_to_ray) const;
	virtual vec3 InterpolatePointNormal(const vec3& point) const;
}

class Triangle : public Primitive
{
public:
	vec3 vertexes[3], vertexNormals[3];
    vec3 &a, &b, &c;
    vec3 &na, &nb, &nc;
    
	Triangle(const vec3& _a, const vec3& _b, const vec3& _c, vec3 na = vec3(0,0,0), vec3 nb = vec3(0,0,0), vec3 nc = vec3(0,0,0));
    
    virtual ~Triangle();
    virtual bool Intersect(const Ray& ray, float* dis_to_ray) const;
    virtual vec3 InterpolatePointNormal(const vec3& point) const;
}

#endif