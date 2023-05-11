#pragma once

#include <vector>

#include "QuadTree.h"
#include "PoolAllocator.h"
#include "ThreadHelper.h"




class QuadTreeApp
{
public:
	QuadTreeApp(uint32_t poolSize) : m_Pool(poolSize) {}

	~QuadTreeApp();

	void Init(float rangePos, float velocity, float minRadius, float maxRadius, int numCircles);

	void Loop(float time, float frameTime);

	void Shutdown();

	std::vector<Circle*> Objects;

private:
	void RunCollisionThreads(float time, float frameTime);
	void CollisionThread(uint32_t thread);
	void CollisionQuery(QuadTree::QuadTree* tree, Circle* allCircles, uint32_t numCircles, float time, float frameTime);

private:
	QuadTree::QuadTree* m_QuadTree = nullptr;

	PoolAllocator<Circle> m_Pool;

private:
	static const uint32_t MAX_WORKERS = 31;
	std::pair<WorkerThread, CollisionWork> m_CollisionWorkers[MAX_WORKERS];

	uint32_t m_NumWorkers;

	int m_NumCircles;


};


