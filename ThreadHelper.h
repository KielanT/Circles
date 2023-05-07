#pragma once
#include <thread>
#include <condition_variable>

#include "Entity.h"

struct WorkerThread
{
	std::thread Thread;
	std::condition_variable WorkReady;
	std::mutex Lock;
};

struct CollisionWork
{
	bool Complete = true;
	Circle* MovingCircles;
	Circle* BlockingCircles;
	uint32_t NumMovingCircles;
	uint32_t NumBlockCircles;
	float Time;
};

struct MoveWork
{
	bool Complete = true;
	Circle* MovingCircles;
	float Speed;
	float DeltaTime;
};
