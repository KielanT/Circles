// Define used for switching between console and visualisation 
#define Console
//#define Visual

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

const int NUM_CIRCLES = 1000;
const float RANGE_POSITION = 1000.0f;
const float RANGE_VELOCITY = 5.0f;
const float RADIUS = 10.0f;
const float SPEED = 100.0f;

const float CAM_SPEED = 500.0f;

PoolAllocator<Circle> CirclesPool{ NUM_CIRCLES };
std::vector<Circle*> AllObjects;
std::vector<Circle*> MovingCircles;
std::vector<Circle*> BlockCircles;

QuadTree::QuadTree* quadTree = new QuadTree::QuadTree(QuadTree::AABB(CVector2(0.0f, 0.0f), RANGE_POSITION), 8);

#ifdef Visual
	// Pool allocator requires a constructor but Imodel is an interface
	//PoolAllocator<IModel> ModelPool{ NUM_CIRCLES };
	std::vector<IModel*> BlockCirclesRendered;
	std::map<std::string, IModel*> MovingCirclesRendered;

	void ControlCamera(I3DEngine* engine, ICamera* camera, float frameTime);
#endif 





void Init();
void Move(Circle* circles, uint32_t numCircles, float frameTime);
void Move(Circle* circles, float frameTime);



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


		for (auto& allCircles : MovingCircles)
		{
			Move(allCircles, frameTime);
		}

		quadTree->Clear();
		for (auto& allCircles : AllObjects)
		{
			allCircles->Bounds.Centre = allCircles->Position;
			quadTree->Insert(allCircles);
		}

		for (auto& allCircles : AllObjects)
		{
			QuadTree::AABB queryRange(allCircles->Position - CVector2(allCircles->Radius, allCircles->Radius), allCircles->Radius * 2.0f);

			auto InRange = quadTree->QueryRange(queryRange);


			for (auto& otherCircle : InRange)
			{
				if (allCircles != otherCircle)
				{

					if (QuadTree::Intersects(allCircles->Bounds, otherCircle->Bounds))
					{
						Collision::CircleToCirlce(allCircles, otherCircle, gTimer.GetTime(), frameTime);
					}
				}
			}
		}


		std::cout << "Frame Time: " << frameTime << std::endl;
		
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
		// Draw the scene
		float frameTime = gTimer.GetLapTime();

		myEngine->DrawScene();
		
		

		for (auto& allCircles : MovingCircles)
		{
			Move(allCircles, frameTime);
		}

		quadTree->Clear();
		for (auto& allCircles : AllObjects)
		{
			allCircles->Bounds.Centre = allCircles->Position;
			quadTree->Insert(allCircles);
		}

		for (auto& allCircles : AllObjects)
		{
			QuadTree::AABB queryRange(allCircles->Position - CVector2(allCircles->Radius, allCircles->Radius), allCircles->Radius * 2.0f);

			auto InRange = quadTree->QueryRange(queryRange);


			for (auto& otherCircle : InRange)
			{
				if (allCircles != otherCircle) 
				{
					
					if (QuadTree::Intersects(allCircles->Bounds, otherCircle->Bounds))
					{
						Collision::CircleToCirlce(allCircles, otherCircle, gTimer.GetTime(), frameTime);
					}
				}
			}
		}
		

		std::cout << "Frame Time: " << frameTime << std::endl;

		ControlCamera(myEngine, Camera, frameTime);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
#endif

	if (quadTree != nullptr) delete quadTree;

}

void Init()
{

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
		circle->Bounds = QuadTree::AABB(circle->Position, circle->Radius * 2);


		
		BlockCircles.push_back(circle);
		AllObjects.push_back(circle);
	}

	for (uint32_t i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		auto circle = CirclesPool.Get();
		circle->Position = { randLoc(mt), randLoc(mt) };
		circle->Radius = RADIUS;
		circle->Velocity = { randVel(mt), randVel(mt) };
		circle->Name = "Moving: " + std::to_string(i);
		circle->Colour = { 0, 0, 1 };
		circle->Bounds = QuadTree::AABB(circle->Position, circle->Radius * 2);
		
		MovingCircles.push_back(circle);
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
		MovingCirclesRendered[circles->Name]->SetPosition(circles->Position.x, circles->Position.y, 0);

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

	MovingCirclesRendered[circle->Name]->SetPosition(circle->Position.x, circle->Position.y, 0);
#endif 
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