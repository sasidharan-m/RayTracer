// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include <cmath>

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  mat3 ret;
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.  
  glm::mat3 k ( 0.0f ), k2 ( 0.0f );
  glm::mat3 i ( 1.0f );
  glm::vec3 v = glm::normalize( axis ); 
  const float radians = degrees * pi / 180;
  k[1][0] = v.z;
  k[2][0] = -v.y;
  k[0][1] = -v.z;
  k[2][1] = v.x;
  k[0][2] = v.y;
  k[1][2] = -v.x;
  k2[0][0] = v.x * v.x;
  k2[1][0] = v.x * v.y;
  k2[2][0] = v.x * v.z;
  k2[0][1] = v.x * v.y;
  k2[1][1] = v.y * v.y;
  k2[2][1] = v.y * v.z;
  k2[0][2] = v.x * v.z;
  k2[1][2] = v.y * v.z;
  k2[2][2] = v.z * v.z;
  ret = (std::cos(radians) * i) + ((1 - std::cos(radians)) * k2) + (std::sin(radians) * k);
  return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1. 
  glm::vec3 axis = glm::vec3( glm::normalize( up ) );
  glm::mat3 r ( 1.0f );
  r = rotate (-degrees, axis);
  eye = r * eye;
  up = r * up; 
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.
  glm::vec3 axis = glm::vec3( glm::normalize( glm::cross( eye,up ) ) ) ;
  glm::mat3 r ( 1.0f );
  r = rotate (-degrees, axis);
  eye = r * eye;
  up = r * up;   
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
  glm::vec3 w = glm::normalize( eye );
  glm::vec3 u = glm::normalize( glm::cross( up, w ) );
  glm::vec3 v = glm::normalize( glm::cross( w, u ) );
  glm::mat4 orientation ( 1.0f );
  orientation[0] = vec4(u.x, v.x, w.x, 0);
  orientation[1] = vec4(u.y, v.y, w.y, 0);
  orientation[2] = vec4(u.z, v.z, w.z, 0);
  glm::mat4 translation( 1.0f );
  translation[3][0] = -eye.x;
  translation[3][1] = -eye.y;
  translation[3][2] = -eye.z;
  translation[3][3] = 1;
  // You will change this return call
  return (orientation * translation); 
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.
  float theta = fovy/2.0;
  const float radians = theta * pi / 180;
  float d =  std::cos(radians) / std::sin(radians);
  float A = -((zFar + zNear) / (zFar - zNear));
  float B = -((2 * zFar * zNear) / (zFar - zNear));
  ret[0][0] = d / aspect;
  ret[1][0] = 0;
  ret[2][0] = 0;
  ret[3][0] = 0;
  ret[0][1] = 0;
  ret[1][1] = d;
  ret[2][1] = 0;
  ret[3][1] = 0;
  ret[0][2] = 0;
  ret[1][2] = 0;
  ret[2][2] = A;
  ret[3][2] = B;
  ret[0][3] = 0;
  ret[1][3] = 0;
  ret[2][3] = -1;
  ret[3][3] = 0;
  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret = glm::mat4( 1.0 );
  // YOUR CODE FOR HW2 HERE
  // Implement scaling
  ret[0][0] = sx;
  ret[1][1] = sy;
  ret[2][2] = sz;
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret = glm::mat4( 1.0 );
  // YOUR CODE FOR HW2 HERE
  // Implement translation
  ret[3][0] = tx;
  ret[3][1] = ty;
  ret[3][2] = tz;
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
