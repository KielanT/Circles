#pragma once

#include <vector>

#include "QuadTree.h"
#include "PoolAllocator.h"
#include "ThreadHelper.h"
#include "Globals.h"

//****************************************
// Quad tree app for running the quad tree
// ***************************************

class QuadTreeApp
{
public:
	QuadTreeApp(uint32_t poolSize) : m_Pool(poolSize) {}

	~QuadTreeApp();

	// Initlise the app
	void Init();

	// Run the app loop
	void Loop(float time, float frameTime);

	// Shutdown the app
	void Shutdown();

	// Objects
	std::vector<Circle*> Objects;

private:
	// Multi threading functions 
	void RunCollisionThreads(float time, float frameTime);
	void CollisionThread(uint32_t thread);
	void CollisionQuery(QuadTree::QuadTree* tree, Circle* allCircles, uint32_t numCircles, float time, float frameTime);

private:
	// Multithreading data
	static const uint32_t MAX_WORKERS = 31;
	uint32_t m_NumWorkers = 0;
	std::pair<WorkerThread, CollisionWork> m_CollisionWorkers[MAX_WORKERS];
	
	// Pool allocator 
	PoolAllocator<Circle> m_Pool;

	// The quad tree
	QuadTree::QuadTree* m_QuadTree = nullptr;


};


