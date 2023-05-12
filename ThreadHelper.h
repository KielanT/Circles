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

// Collision work to be sent to the other threads
struct CollisionWork
{
	bool Complete = true;
	uint32_t NumCircles;
	float Time;
	float FrameTime;
	
	QuadTree::QuadTree* Tree;
	Circle* AllCircles;
};



