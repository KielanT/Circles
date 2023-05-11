#pragma once
#include <vector>

#include "Octree.h"
#include "PoolAllocator.h"

class OctreeApp
{
public:
	OctreeApp(uint32_t poolSize) : m_Pool(poolSize) {}

	~OctreeApp();


	void Init(float rangePos, float velocity, float minRadius, float maxRadius, int numCircles);

	void Loop(float time, float frameTime);

	void Shutdown();

	std::vector<Sphere*> Objects;


private:
	Octree::Node* m_Octree = nullptr;
	
	PoolAllocator<Sphere> m_Pool;
};

