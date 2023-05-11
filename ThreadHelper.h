#pragma once
#include <thread>
#include <condition_variable>

#include "Entity.h"
#include "QuadTree.h"

struct WorkerThread
{
	std::thread Thread;
	std::condition_variable WorkReady;
	std::mutex Lock;
};

struct CollisionWork
{
	bool Complete = true;
	Circle* AllCircles;
	uint32_t NumCircles;
	QuadTree::QuadTree* Tree;
	float Time;
	float FrameTime;
};

