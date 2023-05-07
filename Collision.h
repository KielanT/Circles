#pragma once
#include <iostream>
#include "Entity.h"
#include "CVector3.h"

namespace Collision
{
	void SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numCircles);

	void SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numCircles);
}
