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


const int NUM_CIRCLES = 1000;
const float RANGE_POSITION = 1000.0f;
const float RANGE_VELOCITY = 5.0f;
const float RADIUS = 10.0f;
const float SPEED = 30.0f;

const float CAM_SPEED = 500.0f;

PoolAllocator<Circle> CirclesPool{ NUM_CIRCLES };
std::vector<Circle*> AllObjects;
std::vector<Circle*> MovingCircles;
std::vector<Circle*> BlockCircles;
QuadTree* quadTree = new QuadTree(0, Rectangle(-RANGE_POSITION, -RANGE_POSITION, RANGE_POSITION, RANGE_POSITION));

#ifdef Visual
	// Pool allocator requires a constructor but Imodel is an interface
	//PoolAllocator<IModel> ModelPool{ NUM_CIRCLES };
	std::vector<IModel*> BlockCirclesRendered;
	std::map<std::string, IModel*> MovingCirclesRendered;

	void ControlCamera(I3DEngine* engine, ICamera* camera);
#endif 



Timer gTimer;

void Init();
void Move(Circle* circles, uint32_t numCircles);




void main()
{
	Init();

	gTimer.Reset();

#ifdef Console


	while (true)
	{
		
		gTimer.Tick();
		
		Move(*MovingCircles.data(), NUM_CIRCLES / 2);

		quadTree->Clear();
		for (int i = 0; i < NUM_CIRCLES; i++)
		{
			quadTree->Insert(AllObjects[i]);
		}
		
		std::vector<Circle*> returnObjects;
		for (int i = 0; i < AllObjects.size(); i++)
		{
			returnObjects.clear();
			quadTree->Retrieve(returnObjects, AllObjects[i]->Bounds);
			for (int x = 0; x < returnObjects.size(); x++)
			{
				if(AllObjects[i] != AllObjects[x])
					Collision::CircleToCirlce(AllObjects[i], AllObjects[x]);
			}
		}

		std::cout << "Delta Time: " << gTimer.GetDeltaTime() << std::endl;
		
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

		Move(*MovingCircles.data(), NUM_CIRCLES / 2);

		quadTree->Clear();
		for (int i = 0; i < NUM_CIRCLES; i++)
		{
			quadTree->Insert(AllObjects[i]);
		}

		std::vector<Circle*> returnObjects;
		for (int i = 0; i < AllObjects.size(); i++)
		{
			returnObjects.clear();
			quadTree->Retrieve(returnObjects, AllObjects[i]->Bounds);
			for (int x = 0; x < returnObjects.size(); x++)
			{
				if (AllObjects[i] != AllObjects[x])
					Collision::CircleToCirlce(AllObjects[i], AllObjects[x]);
			}
		}

		std::cout << "Delta Time: " << gTimer.GetDeltaTime() << std::endl;

		ControlCamera(myEngine, Camera);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
#endif


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
		circle->Bounds = Rectangle(circle->Position.x - circle->Radius, circle->Position.y - circle->Radius, circle->Radius * 2, circle->Radius * 2);


		quadTree->Insert(circle);
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
		circle->Bounds = Rectangle(circle->Position.x - circle->Radius, circle->Position.y - circle->Radius, circle->Radius * 2, circle->Radius * 2);
		
		quadTree->Insert(circle);
		MovingCircles.push_back(circle);
		AllObjects.push_back(circle);
	}
}


void Move(Circle* circles, uint32_t numCircles)
{
	auto circlesEnd = circles + numCircles;

	while (circles != circlesEnd)
	{
		circles->Position += (SPEED * circles->Velocity) * gTimer.GetDeltaTime();
		circles->Bounds.X = circles->Position.x - circles->Radius;
		circles->Bounds.Y = circles->Position.y - circles->Radius;

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

#ifdef Visual
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
#endif