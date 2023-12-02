// Primitives cpp file that defines the primitives supported in the ray tracer
// Author: Sasidharan Mahalingam
// Date Created: 1 Dec 2023

#include "primitives.h"
#include <iostream>

const float eps = 1e-6;

int sgn(float x) { return x > eps? 1 : x < -eps? -1 : 0; }

bool isSameVector(const vec3& a, const vec3& b) {
    return glm::dot(b-a, b-a) < eps;
}

bool Color::operator == (const Color& otherColor) const
{
    return r == otherColor.r && g == otherColor.g && b == otherColor.b;
}

Color Color::operator * (const Color& otherColor) const
{
    return Color(r * otherColor.r, g * otherColor.g, b * otherColor.b); 
}

Color Color::operator + (const Color& otherColor) const
{
    return Color(r + otherColor.r, g + otherColor.g, b + otherColor.b);
}

Color Color::operator * (const float scale) const
{
    return Color(r * scale, g * scale, b * scale);
}

bool Color::isZero() const { return Rbyte() == 0 && Gbyte() == 0 && Bbyte() == 0; }

Materials::Materials() : shininess(0.0) {}

Sphere::Sphere(const vec3& o_, const float& r_): o(o_), r(r_)
{
    type = sphere;
}

bool Sphere::intersect(const Ray& ray, float* dis_to_ray) const
{
    const vec3& dir = ray.direction;
    const vec3& p = ray.o;
    float c2 = glm::dot(dir, dir);
    float c1 = 2 * glm::dot(dir, p - o)
    float c0 = glm::dot(p - o, p - o) - r * r;
    float delta = c1 * c1 - 4 * c2 * c0;
    if(delta < -eps)
    {
        return false;
    }
    delta = fabs(delta);
    float x = std::min((-c1 - sqrt(delta))/(2 * c2), (-c1 + sqrt(delta))/(2 * c2));
    if(x < 1e-2)
    {
        return false;
    }
    else
    {
        *dis_to_ray = x;
        return true;
    }
}

vec3 Sphere::interpolatePointNormal(const vec3& point) const
{
    vec4 p_hom = vec4(point, 1.0f) * this->inversed_transform;
    vec3 p_dehom = vec3(p_hom.x / p_hom.w, p_hom.y / p_hom.w, p_hom.z / p_hom.w);
    return vec3(vec4(p_dehom - o, 0.0f) * glm::transpose(this->inversed_transform));
}

Triangle::Triangle(const vec3& a_, const vec3& b_, const vec3& c_, vec3 na_, vec3 nb_, vec3 nc_) : a(vertices[0]), b(vertices[1]), c(vertices[2]), na(vertexNormals[0]), nb(vertexNormals[1]), nc(vertexNormals[2])
{
    a = a_;
    b = b_;
    c = c_;
    type = triangle;
    if(na_ == vec3(0, 0, 0))
    {
        na = nb = nc = glm::cross(b-a, c-a);
    }
    else
    {
        na = na_;
        nb = nb_;
        nc = nc_;
    }
}

Triangle::intersect(const Ray& ray, float *dist_to_ray) const
{
    vec3 n = glm::cross(b-a, c-a);
    const vec3& p = ray.o;
    const vec3& dir = ray.direction;

    // Check if ray is parallel to plane
    if(sgn(glm::dot(dir, n)) == 0)
    {
        return false;
    }
    // Find plane trianlge interection
    float t = (glm::dot(a,n) - glm::dot(p,n)) / glm::dot(dir, n);
    vec3 p0 = p + (dir * t);

    if(t < 1e-2)
    {
        return false;
    }

    vec3 tmp_nb = glm::cross(c - p0, a - p0);
    vec3 tmp_nc = glm::cross(a - p0, b - p0);

    float beta = glm::dot(n, tmp_nb) / glm::dot(n, n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n, n);
    float alpha = 1 - beta - gamma;

    if((alpha > -eps) && (alpha < 1 + eps) && (beta > -eps) && (beta < 1 + eps) && (gamma > -eps) && (gamma < 1 + eps))
    {
        *dist_to_ray = t;
    }
    else
    {
        return false;
    }
}

Triangle::interpolatePointNormal(const vec3& point) const
{
    vec4 p_hom = vec4(point, 1.0f) * this->inversed_transform;
    vec3 p_dehom = vec3(p_hom.x / p_hom.w, p_hom.y / p_hom.w, p_hom.z / p_hom.w);
    vec3 n = glm::cross(b-a, c-a);
    vec3 tmp_nb = glm::cross(c - p_dehom, a - p_dehom);
    vec3 tmp_nc = glm::cross(a - p_dehom, b - p_dehom);

    float beta = glm::dot(n, tmp_nb) / glm::dot(n, n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n, n);
    float alpha = 1 - beta - gamma;

    vec3 ret = (na * alpha) + (nb * beta) + (nc * gamma);
    return vec3(vec4(ret, 0.0f) * glm::transpose(this->inversed_transform));
}

Object::~Object() {}

Triangle::~Triangle() {}

Sphere::~Sphere() {}