#pragma once
#include <map>
#include <Model.h>

#define CONSOLE
//#define VISUAL 

//#define OCTREE
#define QUADTREE

//#define DESTROY
#define RAND_RADIUS

constexpr const uint32_t NUM_CIRCLES = 1000;
constexpr const float RANGE_POSITION = 1000.0f; // "Wall" around the circles
constexpr const float RANGE_VELOCITY = 5.0f;
constexpr const float RADIUS = 5.0f;
constexpr const float MAX_RADIUS = 20.0f;

constexpr const float SPEED = 100.0f;

extern std::map<std::string, tle::IModel*> gModelsMap;