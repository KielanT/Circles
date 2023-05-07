#pragma once

#include <string>
#include "CVector3.h"

struct Circle
{
	CVector3 Position;
	float Radius;
	CVector3 Velocity;
	std::string Name;
	int HP;
	CVector3 Colour;
};