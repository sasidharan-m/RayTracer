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


using namespace std;
 
// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h"

int main(int argc, char* argv[]) {

  if (argc < 2) {
    cerr << "Usage: raytrace scenefile \n"; 
    exit(-1); 
  }

  FreeImage_Initialise();

  readfile(argv[1]) ; 

  FreeImage_DeInitialise();

  return 0;
}
