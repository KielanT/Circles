// Define used for switching between console and visualisation 
//#define Console
#define Visual


#include <TL-Engine.h>	
using namespace tle;

#include <iostream>
#include <random>
#include <vector>
#include <map>

#include "Timer.h"
#include "Entity.h"
#include "Collision.h"
#include "ThreadHelper.h"
#include "PoolAllocator.h"



const int NUM_CIRCLES = 1000;
const float RANGE_POSITION = 1000.0f;
const float RANGE_VELOCITY = 5.0f;
const float RADIUS = 10.0f;
const float SPEED = 30.0f;

const float CAM_SPEED = 500.0f;

PoolAllocator<Circle> CirclesPool{ NUM_CIRCLES };
std::vector<Circle*> BlockCircles;
std::vector<Circle*> MovingCircles;


#ifdef Visual
	// Pool allocator requires a constructor but Imodel is an interface
	//PoolAllocator<IModel> ModelPool{ NUM_CIRCLES };
	std::vector<IModel*> BlockCirclesRendered;
	std::map<std::string, IModel*> MovingCirclesRendered;
#endif 



Timer gTimer;

// Theads
const uint32_t MAX_WORKERS = 31; 
std::pair<WorkerThread, CollisionWork> gCollisionWorkers[MAX_WORKERS];
uint32_t NumWorkers; 

void Init();
void Move(Circle* circles, uint32_t numCirlces);
void CollisionThread(uint32_t thread);
void RunCollisionThreads();
void ControlCamera(I3DEngine* engine, ICamera* camera);


void main()
{
	static std::mutex coutMutex;

	Init();

	gTimer.Reset();

#ifdef Console

	while (true)
	{
		gTimer.Tick();
		

		Move(*MovingCircles.data(), MovingCircles.size());
		RunMoveThread();


		RunCollisionThreads();

		std::unique_lock<std::mutex> l(coutMutex);
		{
			// This never gets printed so it is called on collision (not ideal)
			std::cout << "Delta Time: " << gTimer.GetDeltaTime() << std::endl;
		}
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



	for (const auto movingCircle : MovingCircles)
	{
		IModel* Model = SphereMesh->CreateModel(movingCircle->Position.x, movingCircle->Position.y, 0);
		Model->SetSkin("brick1.jpg");
		MovingCirclesRendered[movingCircle->Name] = Model;
	}


	for (const auto blockCircle : BlockCircles)
	{
		IModel* Model = SphereMesh->CreateModel(blockCircle->Position.x, blockCircle->Position.y, 0);
		Model->SetSkin("CueMetal.jpg");
		BlockCirclesRendered.push_back(Model);
	}



	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		gTimer.Tick();
		// Draw the scene

		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		Move(*MovingCircles.data(), MovingCircles.size());

		RunCollisionThreads();

		

		std::lock_guard<std::mutex> lock(coutMutex);
		{
			std::cout << "Delta Time: " << gTimer.GetDeltaTime() << std::endl;
		}

		ControlCamera(myEngine, Camera);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
#endif

	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		gCollisionWorkers[i].first.Thread.detach();
	}


}

void Init()
{
	NumWorkers = std::thread::hardware_concurrency(); // Gets the amount of threads the system has (is only a hint may not work)
	if (NumWorkers == 0) NumWorkers = 8; // If there wasn't any hinds force threads count to be 8
	NumWorkers -= 2; // Removes 1 worker since the main thread is already running

	// Start the collision threads
	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		gCollisionWorkers[i].first.Thread = std::thread(&CollisionThread, i);
	}


	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> randLoc(-RANGE_POSITION, RANGE_POSITION);
	std::uniform_real_distribution<float> randVel(-RANGE_VELOCITY, RANGE_VELOCITY);


	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = CirclesPool.Get();
		circle->Position = { randLoc(mt), randLoc(mt) };
		circle->Radius = RADIUS;
		circle->Velocity = { 0.0f, 0.0f };
		circle->Name = "Block: " + std::to_string(i);
		circle->Colour = { 1, 0, 0 };

		BlockCircles.push_back(circle);
	}

	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = CirclesPool.Get();
		circle->Position = { randLoc(mt), randLoc(mt) };
		circle->Radius = RADIUS;
		circle->Velocity = { randVel(mt), randVel(mt) };
		circle->Name = "Moving: " + std::to_string(i);
		circle->Colour = { 0, 0, 1 };

		MovingCircles.push_back(circle);
	}
}

void Move(Circle* circles, uint32_t numCirlces)
{
	auto circlesEnd = circles + numCirlces;

	while (circles != circlesEnd)
	{
		circles->Position += (SPEED * circles->Velocity) * gTimer.GetDeltaTime();

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
		MovingCirclesRendered[circles->Name]->SetPosition(circles->Position.x, circles->Position.y, 0);
	
#endif 

		++circles;
	}
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

		Collision::SpheresToSpheres(work.MovingCircles, work.BlockingCircles, work.NumMovingCircles, work.NumBlockCircles, work.Time, gTimer.GetDeltaTime());

		{
			std::unique_lock<std::mutex> l(worker.Lock);
			work.Complete = true;
			worker.WorkReady.notify_one();
		}
	}
}

void RunCollisionThreads()
{
	auto movingCircles = MovingCircles.data();
	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		auto& work = gCollisionWorkers[i].second;
		work.BlockingCircles = *BlockCircles.data();
		work.MovingCircles = *movingCircles;
		work.NumMovingCircles = (NUM_CIRCLES / 2) / (NumWorkers + 1);
		work.NumBlockCircles = NUM_CIRCLES / 2;
		work.Time = gTimer.GetTime();

		auto& workerThread = gCollisionWorkers[i].first;
		{
			std::unique_lock<std::mutex> l(workerThread.Lock);
			work.Complete = false;
		}

		workerThread.WorkReady.notify_one();

		movingCircles += work.NumMovingCircles;
	}

	// Do collision for the remaining circles
	uint32_t numRemainingCircles = (NUM_CIRCLES / 2) - static_cast<uint32_t>(movingCircles - MovingCircles.data());
	Collision::SpheresToSpheres(*movingCircles, *BlockCircles.data(), numRemainingCircles, NUM_CIRCLES / 2, gTimer.GetTime(), gTimer.GetDeltaTime());


	for (uint32_t i = 0; i < NumWorkers; ++i)
	{
		auto& workerThread = gCollisionWorkers[i].first;
		auto& work = gCollisionWorkers[i].second;

		std::unique_lock<std::mutex> l(workerThread.Lock);
		workerThread.WorkReady.wait(l, [&]() { return work.Complete; });
	}
}


void ControlCamera(I3DEngine* engine, ICamera* camera)
{


	if (engine->KeyHeld(Key_W))
	{
		camera->MoveZ(CAM_SPEED * gTimer.GetDeltaTime());
	}
	if (engine->KeyHeld(Key_S))
	{
		camera->MoveZ(-CAM_SPEED * gTimer.GetDeltaTime());
	}
	if (engine->KeyHeld(Key_A))
	{
		camera->MoveX(-CAM_SPEED * gTimer.GetDeltaTime());
	}
	if (engine->KeyHeld(Key_D))
	{
		camera->MoveX(CAM_SPEED * gTimer.GetDeltaTime());
	}
}