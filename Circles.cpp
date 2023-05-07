// Define used for switching between console and visualisation 
//#define Console
#define Visual


#include <TL-Engine.h>	
using namespace tle;

#include <iostream>
#include <random>
#include <vector>

#include "Timer.h"
#include "Entity.h"
#include "Collision.h"



const int NUM_CIRCLES = 100;
const float RANGE_POSITION = 1000.0f;
const float RANGE_VELOCITY = 5.0f;
const float RADIUS = 10.0f;
const float SPEED = 30.0f;

const float CAM_SPEED = 500.0f;

std::vector<Circle> BlockCircles;
std::vector<Circle> MovingCircles;
std::vector<IModel*> BlockCirclesRendered;
std::vector<std::pair<std::string, IModel*>> MovingCirclesRendered; 

Timer gTimer;



void Init();
void Move(Circle* circles, uint32_t numCirlces);
void Colision(Circle* circles, uint32_t numCirlces);
void ControlCamera(I3DEngine* engine, ICamera* camera);

void main()
{
	Init();

	gTimer.Reset();

#ifdef Console

	while (true)
	{
		gTimer.Tick();

		Move(MovingCircles.data(), MovingCircles.size());
		Colision(MovingCircles.data(), MovingCircles.size());

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
		IModel* Model = SphereMesh->CreateModel(movingCircle.Position.x, movingCircle.Position.y, movingCircle.Position.z);
		//Model->Scale(movingCircle.Radius);
		Model->SetSkin("brick1.jpg");
		MovingCirclesRendered.push_back(std::make_pair(movingCircle.Name, Model));
	}


	for (const auto blockCircle : BlockCircles)
	{
		IModel* Model = SphereMesh->CreateModel(blockCircle.Position.x, blockCircle.Position.y, blockCircle.Position.z);
		//Model->Scale(blockCircle.Radius);
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
		Move(MovingCircles.data(), MovingCircles.size());
		//Colision(MovingCircles.data(), MovingCircles.size());
		Collision::SpheresToSpheres(MovingCircles.data(), BlockCircles.data(), NUM_CIRCLES);


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

	for (int i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		Circle circle;
		circle.Position = { randLoc(mt), randLoc(mt), 0.0f };
		circle.Radius = RADIUS;
		circle.Velocity = { 0.0f, 0.0f, 0.0f };
		circle.Name = "Block: " + std::to_string(i);
		circle.Colour = { 1, 0, 0 };

		BlockCircles.push_back(circle);
	}

	for (int i = 0; i < NUM_CIRCLES / 2; ++i)
	{
		Circle circle;
		circle.Position = { randLoc(mt), randLoc(mt), 0.0f };
		circle.Radius = RADIUS;
		circle.Velocity = { randVel(mt), randVel(mt), 0.0f };
		circle.Name = "Moving: " + std::to_string(i);
		circle.Colour = { 0, 0, 1 };

		MovingCircles.push_back(circle);
	}
}

void Move(Circle* circles, uint32_t numCirlces)
{
	auto circlesEnd = circles + numCirlces;

	while (circles != circlesEnd)
	{
		circles->Position += (SPEED * circles->Velocity) * gTimer.GetDeltaTime();


#ifdef Visual
		
		for (auto& movingCircle : MovingCirclesRendered)
		{
			if (movingCircle.first == circles->Name)
			{
				movingCircle.second->SetPosition(circles->Position.x, circles->Position.y, circles->Position.z);
				break;
			}
		}
	
#endif 


		++circles;
	}
}

void Colision(Circle* circles, uint32_t numCirlces)
{
	auto circlesEnd = circles + numCirlces;

	while (circles != circlesEnd)
	{
		Collision::SphereToSpheres(*circles, BlockCircles.data(), NUM_CIRCLES);


		++circles;
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