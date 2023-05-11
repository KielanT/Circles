#include <iostream>
#include <random>
#include "OctreeApp.h"


OctreeApp::~OctreeApp()
{
}


void OctreeApp::Init(float rangePos, float velocity, float minRadius, float maxRadius ,int numCircles)
{
	m_Octree = Octree::BuildOctree(CVector3(0.0f, 0.0f, 0.0f), rangePos, 5);

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> randLoc(-rangePos, rangePos);
	std::uniform_real_distribution<float> randVel(-velocity, velocity);
	std::uniform_real_distribution<float> randRad(minRadius, maxRadius);

	for (uint32_t i = 0; i < numCircles / 2; ++i)
	{
		auto sphere = m_Pool.Get();
		sphere->Position = { randLoc(mt), randLoc(mt), 0.0f };
		sphere->Radius = randRad(mt);
		sphere->Velocity = { 0.0f, 0.0f, 0.0f };
		sphere->Name = "Block: " + std::to_string(i);
		sphere->Colour = { 1, 0, 0 };

		Octree::InsertObject(m_Octree, sphere);
		Objects.push_back(sphere);
	}

	for (uint32_t i = 0; i < numCircles / 2; ++i)
	{
		auto sphere = m_Pool.Get();
		sphere->Position = { randLoc(mt), randLoc(mt), 0.0f };
		sphere->Radius = randRad(mt);
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

