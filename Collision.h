#pragma once
#include <iostream>
#include "Entity.h"
#include "CVector3.h"

namespace Collision
{
	//*************************** 
	//Basic Collision functions
	//****************************

	// Bute force functions
	void SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numMovingCircles, uint32_t numBlockCircles, float time, float deltaTime);
	void SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numBlockCircles, float time, float deltaTime);

	// Used by the octree or quad tree
	void CircleToCirlce(Circle* circle, Circle* other, float time, float deltaTime);
	void SphereToSphere(Sphere* circle, Sphere* other, float time, float DeltaTime);
}
