#pragma once
#include <iostream>
#include "Entity.h"
#include "CVector3.h"

namespace Collision
{
	void SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numMovingCircles, uint32_t numBlockCircles, float time);

	void SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numBlockCircles, float time);
}
