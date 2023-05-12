#include <iostream>
#include <random>
#include "OctreeApp.h"
#include "Globals.h"


OctreeApp::~OctreeApp()
{
}


void OctreeApp::Init()
{
	m_Octree = Octree::BuildOctree(CVector3(0.0f, 0.0f, 0.0f), RANGE_POSITION, 5);

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> randLoc(-RANGE_POSITION, RANGE_POSITION);
	std::uniform_real_distribution<float> randVel(-RANGE_VELOCITY, RANGE_VELOCITY);
	std::uniform_real_distribution<float> randRad(RADIUS, MAX_RADIUS);

	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto sphere = m_Pool.Get();
		sphere->Position = { randLoc(mt), randLoc(mt), 0.0f };
#ifdef RAND_RADIUS
		sphere->Radius = randRad(mt);
#else 
		sphere->Radius = RADIUS;
#endif	
		sphere->Velocity = { 0.0f, 0.0f, 0.0f };
		sphere->Name = "Block: " + std::to_string(i);
		sphere->Colour = { 1, 0, 0 };

		Octree::InsertObject(m_Octree, sphere);
		Objects.push_back(sphere);
	}

	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto sphere = m_Pool.Get();
		sphere->Position = { randLoc(mt), randLoc(mt), 0.0f };
#ifdef RAND_RADIUS
		sphere->Radius = randRad(mt);
#else 
		sphere->Radius = RADIUS;
#endif	
		sphere->Velocity = { randVel(mt), randVel(mt), 0.0f };
		sphere->Name = "Moving: " + std::to_string(i);
		sphere->Colour = { 0, 0, 1 };

		Octree::InsertObject(m_Octree, sphere);
		Objects.push_back(sphere);
	}

}



void OctreeApp::Loop(float time, float frameTime)
{
	Octree::TestCollisions(m_Octree, time, frameTime);
	
	

	std::cout << "FrameTime: " << frameTime << std::endl;
}

void OctreeApp::Shutdown()
{
}

