// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include "Transform.h" 

using namespace std;
using namespace glm;

#include "variables.h" 
#include "readfile.h"


// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, float* values) 
{
  mat4 transform = transfstack.top(); 
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
  vec4 newval = transform * valvec; 
  for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
  mat4 &T = transfstack.top(); 
  T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

void readfile(const char* filename) 
{
  string str, cmd; 
  ifstream in;
  in.open(filename); 
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.  
    // This is done using standard STL Templates 
    stack <mat4> transfstack; 
    transfstack.push(mat4(1.0));  // identity

    getline (in, str); 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        stringstream s(str);
        s >> cmd; 
        int i; 
        float values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.  
        bool validinput; // Validity of input 

        // Process the light, add it to database.
        // Lighting Command
        if ((cmd == "directional") || (cmd == "point")) {
          if (numused == numLights) { // No more Lights 
            cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
          } else {
            validinput = readvals(s, 8, values); // Position/color for lts.
            if (validinput) {

              // YOUR CODE FOR HW 2 HERE. 
              // Note that values[0...7] shows the read in values 
              // Make use of lightposn[] and lightcolor[] arrays in variables.h
              // Those arrays can then be used in display too.  
              for(unsigned int i = 0; i < 3; ++i)
              {
                lightposn[4*numused + i] = values[i];
              }
              lightposn[4*numused+3] = (cmd == "directional" ? 0.0 : 1.0);
              for(unsigned int i = 4; i < 7; ++i)
              {
                lightcolor[4*numused + (i - 4)] = values[i];
              }
              lightcolor[3] = 1.0;
              ++numused; 
            }
          }
        }

        // Material Commands 
        // Ambient, diffuse, specular, shininess properties for each object.
        // Filling this in is pretty straightforward, so I've left it in 
        // the skeleton, also as a hint of how to do the more complex ones.
        // Note that no transforms/stacks are applied to the colors. 

        else if (cmd == "ambient") {
          validinput = readvals(s, 3, values); // colors 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              ambient[i] = values[i]; 
            }
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              diffuse[i] = values[i]; 
            }
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              specular[i] = values[i]; 
            }
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 4; i++) {
              emission[i] = values[i]; 
            }
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values); 
          if (validinput) {
            shininess = values[0]; 
          }
        } else if (cmd == "size") {
          validinput = readvals(s,2,values); 
          if (validinput) { 
            w = (int) values[0]; h = (int) values[1]; 
          } 
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            eyeinit[0] = values[0];
            eyeinit[1] = values[1];
            eyeinit[2] = values[2];

            center[0] = values[3];
            center[1] = values[4];
            center[2] = values[5];

            upinit[0] = values[6];
            upinit[1] = values[7];
            upinit[2] = values[8];

            fovy = values[9];

          }
        }
        else if (cmd == "translate") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file. 
            // Also keep in mind what order your matrix is!
            rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.  
            // Also keep in mind what order your matrix is!
            rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE. 
            // values[0..2] are the axis, values[3] is the angle.  
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.  
            // Note that rotate returns a mat3. 
            // Also keep in mind what order your matrix is!
            rightmultiply(glm::mat4(Transform::rotate(-values[3], vec3(values[0], values[1], values[2]))), transfstack);
          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top()); 
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
          }
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        }
      }
      getline (in, str); 
    }

  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }
}
