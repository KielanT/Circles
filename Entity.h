#pragma once

#include <string>
#include "CVector3.h"

struct Circle
{
	CVector3 Position;
	float Radius;
	CVector3 Velocity;
	std::string Name;
	int HP = 100;
	CVector3 Colour;
};