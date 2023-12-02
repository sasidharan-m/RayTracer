// Scene cpp file that defines the scene of the ray tracer
// Author: Sasidharan Mahalingam
// Date Created: 2 Dec 2023

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include "scene.h"

const vec3& Light::position() const
{
	return pos_or_dir;
}

const vec3& Light::direction() const
{
	return pos_or_dir;
}

void rightMultiply(const mat4 &m, stack<mat4> &transform_stack)
{
	mat4 &t = transform_stack.top();
	t = m * t;
}

bool Scene::readvals(stringstream &s, const int numvals, float* values) 
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

void Scene::readfile(const string &filename)
{
	string str, cmd;
	ifstream in;
	in.open(filename.c_str());
	if (in.is_open()) 
	{
		stack<mat4> transform_stack;
		transform_stack.push(mat4(1.0));

		getline(in, str);
		while(in)
		{
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) 
			{
		        // Ruled out comment and blank lines 
		        stringstream s(str);
		        s >> cmd; 
		        int i; 
		        float values[10]; // Position and color for light, colors for others
		        // Up to 10 params for cameras.  
		        bool validinput; // Validity of input

		        if(cmd == "output")
		        {
		        	s >> outputfile;
		        } 
		        // Process the light, add it to database.
        		// Lighting Command
		        else if((cmd == "directional") || (cmd == "point")) 
		        {
		        	validinput = readvals(s, 6, values);
		        	if(validinput)
		        	{
		        		Light light;
		        		light.pos_or_dir = vec3(values[0], values[1], values[2]);
		        		light.color = Color(values[3], values[4], values[5]);
		        		if(cmd == "directional")
		        		{
		        			light.type = Light::directional;
		        		}
		        		else if(cmd == "point")
		        		{
		        			light.type = Light::point;
		        		}
		        		lights.push_back(light);
		        	}
				}
				else if(cmd == "attenuation")
				{
					validinput = readvals(s, 3, values);
					if(validinput)
					{
						for(unsigned int i = 0; i < 3; ++i)
						{
							attenuation[i] = values[i];
						}
					}
				}
				// Material Commands 
		        // Ambient, diffuse, specular, shininess properties for each object.
		        // Filling this in is pretty straightforward, so I've left it in 
		        // the skeleton, also as a hint of how to do the more complex ones.
		        // Note that no transforms/stacks are applied to the colors. 

		        else if(cmd == "ambient") 
		        {
					validinput = readvals(s, 3, values); // colors 
					if(validinput) 
					{
						materials.ambient = Color(values[0], values[1], values[2]); 
					}
		        } 
		        else if(cmd == "diffuse") 
		        {
					validinput = readvals(s, 3, values); 
					if(validinput) 
					{
						materials.diffuse = Color(values[0], values[1], values[2]);
					}
		        } 
		        else if(cmd == "specular") 
		        {
					validinput = readvals(s, 3, values); 
					if(validinput) 
					{
						materials.specular = Color(values[0], values[1], values[2]);
					}
		        } 
		        else if(cmd == "emission") 
		        {
					validinput = readvals(s, 3, values); 
					if(validinput) 
					{
						materials.emission = Color(values[0], values[1], values[2]);
					}
		        } 
		        else if(cmd == "shininess") 
		        {
					validinput = readvals(s, 1, values); 
					if (validinput) 
					{
						materials.shininess = values[0];
					}
		        } 
		        else if(cmd == "size") 
		        {
					validinput = readvals(s,2,values); 
					if(validinput) 
					{ 
						width = (int) values[0]; 
						height = (int) values[1]; 
					} 
		        } 
		        else if(cmd == "camera") 
		        {
					validinput = readvals(s, 10, values); // 10 values eye cen up fov
					if(validinput) 
					{
						camera.eye = vec3(values[0], values[1], values[2]);
						camera.center = vec3(values[3], values[4], values[5]);
						camera.up = vec3(values[6], values[7], values[8]);
						camera.fovy = values[9];
					}
		        }
		        else if(cmd == "maxdepth")
		        {
		        	validinput = readvals(s, 1, values);
		        	if(validinput)
		        	{
		        		max_depth = values[0];
		        	}
		        }
		        else if(cmd == "vertex")
		        {
		        	validinput = readvals(s, 3, values);
		        	if(validinput)
		        	{
		        		vertex_buffer.push_back(vec3(values[0], values[1], values[2]));
		        	}
		        }
		        else if(cmd == "vertexnormal")
		        {
		        	validinput = readvals(s, 6, values);
		        	if(validinput)
		        	{
		        		vertex_buffer_with_normal.push_back(vec3(values[0], values[1], values[2]));
		        		vertex_normal_buffer.push_back(vec3(values[3], values[4], values[5]));
		        	}
		        }
		        else if(cmd == "tri")
		        {
		        	validinput = readvals(s, 3, values);
		        	if(validinput)
		        	{
		        		Triangle *triangle = new Triangle(vertex_buffer[values[0]], vertex_buffer[values[1]], vertex_buffer[values[2]]);
		        		triangle->index = primitives.size();
		        		triangle->materials = materials;
		        		triangle->transform = transform_stack.top();
		        		triangle->inversed_transform = glm::inverse(transform_stack.top());
		        		primitives.push_back(triangle);
		        	}
		        }
		        else if(cmd == "trinormal")
		        {
		        	validinput = readvals(s, 6, values);
		        	if(validinput)
		        	{
		        		Triangle *triangle = new Triangle(vertex_buffer_with_normal[values[0]], vertex_buffer_with_normal[values[1]], vertex_buffer_with_normal[values[2]], vertex_normal_buffer[values[3]], vertex_normal_buffer[values[4]], vertex_normal_buffer[values[5]]);
		        		triangle->index = primitives.size();
		        		triangle->materials = materials;
		        		triangle->transform = transform_stack.top();
		        		triangle->inversed_transform = glm::inverse(transform_stack.top());
		        		primitives.push_back(triangle);
		        	}
		        }
		        else if(cmd == "sphere")
		        {
		        	validinput = readvals(s, 4, values);
		        	if(validinput)
		        	{
		        		Sphere *sphere = new Sphere(values[0], values[1], values[2], values[3]);
		        		sphere->index = primitives.size();
		        		sphere->materials = materials;
		        		sphere->transform = transform_stack.top();
		        		triangle->inversed_transform = glm::inverse(transform_stack.top());
		        		primitives.push_back(sphere);
		        	}
		        }
		        else if((cmd == "maxverts") || (cmd == "maxvertnorms"))
		        {

		        }
		        else if(cmd == "translate")
		        {
		        	validinput = readvals(s, 3, values);
		        	if(validinput)
		        	{
		        		rightMultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
		        	}
		        }
		        else if(cmd == "scale")
		        {
		        	validinput = readvals(s, 3, values);
		        	if(validinput)
		        	{
		        		rightMultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
		        	}
		        }
		        else if(cmd == "rotate")
		        {
		        	validinput = readvals(s, 4, values);
		        	if(validinput)
		        	{
		        		rightMultiply(glm::mat4(Transform::rotate(-values[3], vec3(values[0], values[1], values[2]))), transfstack);
		        	}
		        }
		        // I include the basic push/pop code for matrix stacks
				else if(cmd == "pushTransform") 
				{
					transfstack.push(transfstack.top()); 
				} 
				else if(cmd == "popTransform") 
				{
					if(transfstack.size() <= 1) 
					{
						cerr << "Stack has no elements.  Cannot Pop\n"; 
					} 
					else 
					{
						transfstack.pop(); 
					}
				}
				else 
				{
          			cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        		}
			}
			getline(in, str);
		}
	}
	else 
	{
    	cerr << "Unable to Open Input Data File " << filename << "\n"; 
    	throw 2; 
  	}
}

Scene::Scene()
{
	attenuation[0] = 1.0;
	attenuation[1] = 0.0;
	attenuation[2] = 0.0;
	max_depth = 5;
}

Scene::~Scene() {}
