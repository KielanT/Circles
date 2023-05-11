#pragma once
#include <map>

#include "CVector2.h"
#include "CVector3.h"
#include "Entity.h"
#include <Model.h>



namespace Movement
{
	void Move(Circle* circles, const int& numCircles, const float& speed, const float& range, const float& frameTime);
	void Move(Sphere* spheres, const int& numSphere, const float& speed, const float& range, const float& frameTime);

	void Move(Circle* circles, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime);
	void Move(Sphere* spheres, std::map<std::string, tle::IModel*>& modelsMap, const int& numSphere, const float& speed, const float& range, const float& frameTime);
}

