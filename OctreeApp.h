#pragma once
#include <vector>

#include "Octree.h"
#include "PoolAllocator.h"

class OctreeApp
{
public:
	OctreeApp(uint32_t poolSize) : m_Pool(poolSize) {}

	~OctreeApp();

	// Initiliser
	void Init();

	// Loop function
	void Loop(float time, float frameTime);

	// Shutdown
	void Shutdown();

	// Objects
	std::vector<Sphere*> Objects;


private:

	// Quad tree
	Octree::Node* m_Octree = nullptr;
	
	// Pool allocator
	PoolAllocator<Sphere> m_Pool;
};

