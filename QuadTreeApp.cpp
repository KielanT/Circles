#include <iostream>
#include <random>
#include <thread>

#include "QuadTreeApp.h"
#include "Collision.h"



QuadTreeApp::~QuadTreeApp()
{
	if (m_QuadTree != nullptr) delete m_QuadTree;

	// Detach the threads
	for (uint32_t i = 0; i < m_NumWorkers; ++i)
	{
		m_CollisionWorkers[i].first.Thread.detach();
	}
}

void QuadTreeApp::Init()
{
	// Create a new quad tree
	m_QuadTree = new QuadTree::QuadTree(QuadTree::AABB(CVector2(0.0f, 0.0f), RANGE_POSITION), 8);

	m_NumWorkers = std::thread::hardware_concurrency(); // Gets the amount of threads the system has (is only a hint may not work)
	if (m_NumWorkers == 0) m_NumWorkers = 8; // If there wasn't any hints force threads count to be 8
	m_NumWorkers -= 1; // Removes 1 worker since the main thread is already running

	// Start the collision threads
	for (uint32_t i = 0; i < m_NumWorkers; ++i)
	{
		m_CollisionWorkers[i].first.Thread = std::thread(&QuadTreeApp::CollisionThread, this, i);
	}

	// Get random values
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> randLoc(-RANGE_POSITION, RANGE_POSITION);
	std::uniform_real_distribution<float> randVel(-RANGE_VELOCITY, RANGE_VELOCITY);
	std::uniform_real_distribution<float> randRad(RADIUS, MAX_RADIUS);

	// Create the block circles (this to set the colour and name)
	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = m_Pool.Get(); // Get/Construct circle from the pool

		// Set up the circles values
		circle->Position = { randLoc(mt), randLoc(mt) };

		// Random radius on or off
#ifdef RAND_RADIUS
		circle->Radius = randRad(mt);
#else 
		circle->Radius = RADIUS;
#endif		
		circle->Velocity = { 0.0f, 0.0f }; // Velocity off for the blockers
		circle->Name = "Block: " + std::to_string(i);
		circle->Colour = { 1, 0, 0 };
		circle->Bounds = QuadTree::AABB(circle->Position, circle->Radius * 2);

		Objects.push_back(circle);
	}

	// Create the moving circles (this to set the colour and name)
	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = m_Pool.Get();
		circle->Position = { randLoc(mt), randLoc(mt) };

#ifdef RAND_RADIUS
		circle->Radius = randRad(mt);
#else 
		circle->Radius = RADIUS;
#endif	
		circle->Velocity = { randVel(mt), randVel(mt) }; // Random Velocity
		circle->Name = "Moving: " + std::to_string(i);
		circle->Colour = { 0, 0, 1 };
		circle->Bounds = QuadTree::AABB(circle->Position, circle->Radius * 2);

		Objects.push_back(circle);
	}
}

void QuadTreeApp::Loop(float time, float frameTime)
{
	// Clear the tree then rebuild
	m_QuadTree->Clear();
	for (auto& obj : Objects)
	{
		obj->Bounds.Centre = obj->Position;
		m_QuadTree->Insert(obj);
	}

	// Run the quad tree on different threads
	RunCollisionThreads(time, frameTime);

	std::cout << "FrameTime: " << frameTime << std::endl;
}

void QuadTreeApp::Shutdown()
{

}

void QuadTreeApp::RunCollisionThreads(float time, float frameTime)
{
	// Splits up the circles and sends them to the threads
	auto AllCircles = Objects.data();
	for (uint32_t i = 0; i < m_NumWorkers; ++i)
	{
		auto& work = m_CollisionWorkers[i].second;
		work.AllCircles = *AllCircles;
		work.NumCircles = NUM_CIRCLES / (m_NumWorkers + 1);
		work.Tree = m_QuadTree;
		work.Time = time;
		work.FrameTime = frameTime;

		auto& workerThread = m_CollisionWorkers[i].first;
		{
			std::unique_lock<std::mutex> l(workerThread.Lock);
			work.Complete = false;
		}

		workerThread.WorkReady.notify_one();

		AllCircles += work.NumCircles;
	}

	// Do collision for the remaining circles
	uint32_t numRemainingCircles = NUM_CIRCLES - static_cast<uint32_t>(AllCircles - Objects.data());
	CollisionQuery(m_QuadTree, *AllCircles, numRemainingCircles, time, frameTime);


	for (uint32_t i = 0; i < m_NumWorkers; ++i)
	{
		auto& workerThread = m_CollisionWorkers[i].first;
		auto& work = m_CollisionWorkers[i].second;

		std::unique_lock<std::mutex> l(workerThread.Lock);
		workerThread.WorkReady.wait(l, [&]() { return work.Complete; });
	}
}

void QuadTreeApp::CollisionThread(uint32_t thread)
{
	// Collision function
	auto& worker = m_CollisionWorkers[thread].first;
	auto& work = m_CollisionWorkers[thread].second;
	
	while (true)
	{
		{
			std::unique_lock<std::mutex> l(worker.Lock);
			worker.WorkReady.wait(l, [&]() { return !work.Complete; });
		}
	
		// RUn the collison query on thread
		CollisionQuery(work.Tree, work.AllCircles, work.NumCircles, work.Time, work.FrameTime);
	
		{
			std::unique_lock<std::mutex> l(worker.Lock);
			work.Complete = true;
			worker.WorkReady.notify_one();
		}
	}
}

void QuadTreeApp::CollisionQuery(QuadTree::QuadTree* tree, Circle* allCircles, uint32_t NUM_CIRCLES, float time, float frameTime)
{

	// Loop through all the circles against the circles in range
	auto criclesEnd = allCircles + NUM_CIRCLES;
	while (allCircles != criclesEnd)
	{
		QuadTree::AABB queryRange(allCircles->Position - CVector2(allCircles->Radius, allCircles->Radius), allCircles->Radius * 2.0f);

		auto InRange = tree->QueryRange(queryRange);

		for (auto& otherCircle : InRange)
		{
			if (allCircles != otherCircle)
			{

				if (QuadTree::Intersects(allCircles->Bounds, otherCircle->Bounds))
				{
					// Circle to circle collision, this is what slows down the collision query
					Collision::CircleToCirlce(allCircles, otherCircle, time, frameTime);
				}
			}
		}

		++allCircles;
	}
}
