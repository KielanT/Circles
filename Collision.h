#pragma once
#include <iostream>
#include "Entity.h"
#include "CVector3.h"

namespace Collision
{
	void SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numMovingCircles, uint32_t numBlockCircles, float time, float deltaTime);

	void SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numBlockCircles, float time, float deltaTime);

	void CircleToCirlce(Circle* circle, Circle* other, float time, float deltaTime);
}
