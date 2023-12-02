// Main program for simple Ray Tracing
// Author: Sasidharan Mahalingam
// Date Created: 1 Dec 2023
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include "Transform.h"
#include <FreeImage.h>
#include <stdio.h>
#include <cassert>

#include "scene.h"

using namespace std;
 

int main(int argc, char* argv[]) {

  if (argc < 2) {
    cerr << "Usage: raytrace scenefile \n"; 
    exit(-1); 
  }

  FreeImage_Initialise();

  Scene scene;
  scene.outputfile = "result.png";
  scene.readfile(argv[1]); 

  FreeImage_DeInitialise();

  return 0;
}
