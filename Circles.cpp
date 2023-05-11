// Define used for switching between console and visualisation 
//#define Console
#define Visual

#ifdef Visual
#include <TL-Engine.h>	
using namespace tle;
#endif 

#include <iostream>
#include <random>
#include <vector>
#include <map>

#include "Timer.h"
#include "Entity.h"
#include "Collision.h"
#include "ThreadHelper.h"
#include "PoolAllocator.h"
#include "QuadTree.h"

const uint32_t NUM_CIRCLES = 100;
const float RANGE_POSITION = 1000.0f; // "Wall" around the circles
const float RANGE_VELOCITY = 5.0f;
const float RADIUS = 5.0f;
const float MAX_RADIUS = 20.0f;

const float SPEED = 100.0f;
const float SCALE_FACTOR = 5.0f;

const float CAM_SPEED = 500.0f;

PoolAllocator<Circle> CirclesPool{ NUM_CIRCLES };
std::vector<Circle*> AllObjects;


QuadTree::QuadTree* quadTree = new QuadTree::QuadTree(QuadTree::AABB(CVector2(0.0f, 0.0f), RANGE_POSITION), 8);

#ifdef Visual

	std::map<std::string, IModel*> RenderedCirclesMap;

	void ControlCamera(I3DEngine* engine, ICamera* camera, float frameTime);
#endif 

// Threads
const uint32_t MAX_WORKERS = 31;
std::pair <WorkerThread, CollisionWork> gCollisionWorkers[MAX_WORKERS];
uint32_t NumWorkers;

void Init();
void Move(Circle* circles, uint32_t numCircles, float frameTime);
void Move(Circle* circles, float frameTime);
void CollisionThread(uint32_t thread);
void RunCollisionTheads(float time, float frameTime);
void QuadTreeCollisionQuery(QuadTree::QuadTree* tree, Circle* allCircles, uint32_t numCircles, float time, float frameTime);


void main()
{
	Init();

	Timer gTimer;
	gTimer.Start();

#ifdef Console


	while (true)
	{
		float frameTime = gTimer.GetLapTime();
		quadTree->Clear();


		for (auto& allCircles : AllObjects)
		{
			Move(allCircles, frameTime);
		}

		quadTree->Clear();
		for (auto& allCircles : AllObjects)
		{
			allCircles->Bounds.Centre = allCircles->Position;
			quadTree->Insert(allCircles);
		}

		RunCollisionTheads(gTimer.GetTime(), frameTime);

		
	}
#endif

#ifdef Visual
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "D:\\Software\\Misc\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* Camera;
	Camera = myEngine->CreateCamera(kManual, 0, 0, -3000);

	IMesh* SphereMesh = myEngine->LoadMesh("Sphere.x");

	for (const auto& circles : AllObjects)
	{
		IModel* Model = SphereMesh->CreateModel(circles->Position.x, circles->Position.y, 0);
		Model->Scale(circles->Radius / SCALE_FACTOR);
		RenderedCirclesMap[circles->Name] = Model;

		if (circles->Colour == CVector3( 1, 0, 0 ))
		{
			Model->SetSkin("brick1.jpg");
		}
		else
		{
			Model->SetSkin("CueMetal.jpg");
			
		}
	}


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		float frameTime = gTimer.GetLapTime();

		myEngine->DrawScene();
		
		

		for (auto& allCircles : AllObjects)
		{
			Move(allCircles, frameTime);
		}

		quadTree->Clear();
		for (auto& allCircles : AllObjects)
		{
			allCircles->Bounds.Centre = allCircles->Position;
			quadTree->Insert(allCircles);
		}
	
		RunCollisionTheads(gTimer.GetTime(), frameTime);

		ControlCamera(myEngine, Camera, frameTime);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
#endif

	if (quadTree != nullptr) delete quadTree;

	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		gCollisionWorkers[i].first.Thread.detach();
	}
}

void Init()
{
	NumWorkers = std::thread::hardware_concurrency(); // Gets the amount of threads the system has (is only a hint may not work)
	if (NumWorkers == 0) NumWorkers = 8; // If there wasn't any hints force threads count to be 8
	NumWorkers -= 1; // Removes 1 worker since the main thread is already running

	// Start the collision threads
	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		gCollisionWorkers[i].first.Thread = std::thread(&CollisionThread, i);
	}


	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> randLoc(-RANGE_POSITION, RANGE_POSITION);
	std::uniform_real_distribution<float> randVel(-RANGE_VELOCITY, RANGE_VELOCITY);
	std::uniform_real_distribution<float> randRad(RADIUS, MAX_RADIUS);


	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = CirclesPool.Get();
		circle->Position = { randLoc(mt), randLoc(mt) };
		circle->Radius = randRad(mt);
		circle->Velocity = { 0.0f, 0.0f };
		circle->Name = "Block: " + std::to_string(i);
		circle->Colour = { 1, 0, 0 };
		circle->Bounds = QuadTree::AABB(circle->Position, circle->Radius * 2);

		AllObjects.push_back(circle);
	}

	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = CirclesPool.Get();
		circle->Position = { randLoc(mt), randLoc(mt) };
		circle->Radius = randRad(mt);
		circle->Velocity = { randVel(mt), randVel(mt) };
		circle->Name = "Moving: " + std::to_string(i);
		circle->Colour = { 0, 0, 1 };
		circle->Bounds = QuadTree::AABB(circle->Position, circle->Radius * 2);
		
		AllObjects.push_back(circle);
	}
}


void Move(Circle* circles, uint32_t numCircles, float frameTime)
{
	auto circlesEnd = circles + numCircles;

	while (circles != circlesEnd)
	{
		circles->Position += (SPEED * circles->Velocity) * frameTime;

		if (circles->Position.x < -RANGE_POSITION || circles->Position.x > RANGE_POSITION)
		{
			circles->Velocity.x = -circles->Velocity.x;
		}

		if (circles->Position.y < -RANGE_POSITION || circles->Position.y > RANGE_POSITION)
		{
			circles->Velocity.y = -circles->Velocity.y;
		}

#ifdef Visual

		//or (auto& movingCircle : MovingCirclesRendered)
		//
		//	if (movingCircle.first == circles->Name)
		//	{
		//		movingCircle.second->SetPosition(circles->Position.x, circles->Position.y, circles->Position.z);
		//		break;
		//	}
		//
		// A map is quicker than doing the above code
		RenderedCirclesMap[circles->Name]->SetPosition(circles->Position.x, circles->Position.y, 0);

#endif 

		++circles;
	}
}

void Move(Circle* circle, float frameTime)
{
	circle->Position += (SPEED * circle->Velocity) * frameTime;


	if (circle->Position.x < -RANGE_POSITION || circle->Position.x > RANGE_POSITION)
	{
		circle->Velocity.x = -circle->Velocity.x;
	}

	if (circle->Position.y < -RANGE_POSITION || circle->Position.y > RANGE_POSITION)
	{
		circle->Velocity.y = -circle->Velocity.y;
	}

#ifdef Visual

	RenderedCirclesMap[circle->Name]->SetPosition(circle->Position.x, circle->Position.y, 0);
#endif 
}

void CollisionThread(uint32_t thread)
{
	auto& worker = gCollisionWorkers[thread].first;
	auto& work = gCollisionWorkers[thread].second;
	while (true)
	{
		{
			std::unique_lock<std::mutex> l(worker.Lock);
			worker.WorkReady.wait(l, [&]() { return !work.Complete; });
		}

		QuadTreeCollisionQuery(work.Tree, work.AllCircles, work.NumCircles, work.Time, work.FrameTime);

		{
			std::unique_lock<std::mutex> l(worker.Lock);
			work.Complete = true;
			worker.WorkReady.notify_one();
		}
	}
}

void RunCollisionTheads(float time, float frameTime)
{
	auto AllCircles = AllObjects.data();
	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		auto& work = gCollisionWorkers[i].second;
		work.AllCircles = *AllCircles;
		work.NumCircles = NUM_CIRCLES / (NumWorkers + 1);
		work.Tree = quadTree;
		work.Time = time;
		work.FrameTime = frameTime;

		auto& workerThread = gCollisionWorkers[i].first;
		{
			std::unique_lock<std::mutex> l(workerThread.Lock);
			work.Complete = false;
		}

		workerThread.WorkReady.notify_one();

		AllCircles += work.NumCircles;
	}

	// Do collision for the remaining circles
	uint32_t numRemainingCircles = NUM_CIRCLES  - static_cast<uint32_t>(AllCircles - AllObjects.data());
	QuadTreeCollisionQuery(quadTree, *AllCircles, numRemainingCircles, time, frameTime);


	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		auto& workerThread = gCollisionWorkers[i].first;
		auto& work = gCollisionWorkers[i].second;

		std::unique_lock<std::mutex> l(workerThread.Lock);
		workerThread.WorkReady.wait(l, [&]() { return work.Complete; });
	}
}

void QuadTreeCollisionQuery(QuadTree::QuadTree* tree, Circle* allCircles, uint32_t numCircles, float time, float frameTime)
{

	auto criclesEnd = allCircles + numCircles;
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
					Collision::CircleToCirlce(allCircles, otherCircle, time, frameTime);
				}
			}
		}

		++allCircles;
	}
}


#ifdef Visual
void ControlCamera(I3DEngine* engine, ICamera* camera, float frameTime)
{


	if (engine->KeyHeld(Key_W))
	{
		camera->MoveZ(CAM_SPEED * frameTime);
	}
	if (engine->KeyHeld(Key_S))
	{
		camera->MoveZ(-CAM_SPEED * frameTime);
	}
	if (engine->KeyHeld(Key_A))
	{
		camera->MoveX(-CAM_SPEED * frameTime);
	}
	if (engine->KeyHeld(Key_D))
	{
		camera->MoveX(CAM_SPEED * frameTime);
	}
}
#endif