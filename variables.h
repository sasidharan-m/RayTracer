// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "primitives.h"

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;


#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int w = 500, h = 500 ; // width and height 
float fovy = 90.0 ; // For field of view
#else 
EXTERN vec3 eyeinit ; 
EXTERN vec3 upinit ; 
EXTERN vec3 center ; 
EXTERN int amountinit;
EXTERN int w, h ; 
EXTERN float fovy ; 
#endif 


// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ; 
EXTERN float lightposn [4*numLights] ; // Light Positions
EXTERN float lightcolor[4*numLights] ; // Light Colors

EXTERN int numused ;                     // How many lights are used 

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN float ambient[3] ; 
EXTERN float diffuse[3] ; 
EXTERN float specular[3] ; 
EXTERN float emission[3] ; 
EXTERN float shininess ; 


