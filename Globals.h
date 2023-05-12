#pragma once
#include <map>
#include <Model.h>

//****************************************
//    Preprocessor defines are
//    used to switch between options
//	  and turn options on and off
//	  
//    Enable by commenting them in or out 
//	 
//	  - Switch between console and visual
//	  - Switch between octree and quadtree
//    - turn on and off if the circles should be destroyed
//    - turn on and of random radius
//*****************************************

#define CONSOLE
//#define VISUAL 

//#define OCTREE
#define QUADTREE

//#define DESTROY
//#define RAND_RADIUS

//****************************************
//   Globals used through out the program
//*****************************************

// Uses constexpr so they get initlised at compile time not run time

constexpr const uint32_t NUM_CIRCLES = 1000; // number of circles in the environments
constexpr const float RANGE_POSITION = 1000.0f; // "Wall" around the circles and the max and min (negative value of this) of the world bounds
constexpr const float RANGE_VELOCITY = 5.0f; // Min(negative of this value) and max velocity of the movement 
constexpr const float RADIUS = 5.0f; // Min radius 
constexpr const float MAX_RADIUS = 20.0f; // Max Radius

constexpr const float SPEED = 100.0f; // Speed for the movement of the circles

// Extern to be initlised else where and allow for the use of it being global
extern std::map<std::string, tle::IModel*> gModelsMap;