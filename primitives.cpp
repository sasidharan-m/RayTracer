#include "primitives.h"
#include <iostream>

const float eps = 1e-6;
int sgn(float x) { return x > eps? 1 : x < -eps? -1 : 0; }
bool IsSameVector(const vec3& a, const vec3& b) {
    return glm::dot(b-a, b-a) < eps;
}
