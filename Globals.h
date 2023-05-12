#pragma once
#include <map>
#include <Model.h>

//#define DESTROY


const uint32_t NUM_CIRCLES = 1000;
const float RANGE_POSITION = 1000.0f; // "Wall" around the circles
const float RANGE_VELOCITY = 5.0f;
const float RADIUS = 5.0f;
const float MAX_RADIUS = 20.0f;

const float SPEED = 100.0f;

extern std::map<std::string, tle::IModel*> gModelsMap;