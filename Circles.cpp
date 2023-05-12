// Define used for switching between console and visualisation 


#include <iostream>
#include <map>


#include "Timer.h"
#include "Entity.h"
#include "Collision.h"
#include "Movement.h"
#include "Globals.h"


#ifdef VISUAL
#include <TL-Engine.h>	
using namespace tle;
#endif 

#ifdef QUADTREE
#include "QuadTreeApp.h"

#endif // QUADTREE

#ifdef OCTREE
#include "OctreeApp.h"
#endif // OCTREE


// Scale factor used for making sure the radius are synces or at least close
constexpr const float SCALE_FACTOR = 5.0f;

// Used for controlling the camera speed
constexpr const float CAM_SPEED = 500.0f;

#ifdef VISUAL
std::map<std::string, IModel*> gModelsMap;
#endif 

// Static polymorphism for switching between the quad tree and octree
#ifdef QUADTREE
QuadTreeApp app(NUM_CIRCLES);
#endif // QUADTREE

#ifdef OCTREE
OctreeApp app(NUM_CIRCLES);
#endif // OCTREE

#ifdef VISUAL
void ControlCamera(I3DEngine* engine, ICamera* camera, float frameTime);
#endif 

void main()
{
	app.Init(); // App init function

	// Timer
	Timer timer;
	timer.Start();

#ifdef CONSOLE


	while (true)
	{
		float frameTime = timer.GetLapTime(); // Frame time
		
		// Move objects 
		Movement::Move(*app.Objects.data(), frameTime);
		
		// Run the loop
		app.Loop(timer.GetTime(), frameTime);
	}
#endif

#ifdef VISUAL
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "D:\\Software\\Misc\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* Camera;
	Camera = myEngine->CreateCamera(kManual, 0, 0, -3000);

	IMesh* SphereMesh = myEngine->LoadMesh("Sphere.x");

	// Init the models
	for (const auto& object : app.Objects)
	{
		IModel* Model = SphereMesh->CreateModel(object->Position.x, object->Position.y, 0);
		Model->Scale(object->Radius / SCALE_FACTOR);
		gModelsMap[object->Name] = Model;
	
		if (object->Colour == CVector3( 1, 0, 0 ))
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
		float frameTime = timer.GetLapTime();

		myEngine->DrawScene();
		
		Movement::Move(*app.Objects.data(), gModelsMap, frameTime);
		app.Loop(timer.GetTime(), frameTime);
		

		ControlCamera(myEngine, Camera, frameTime);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
#endif
	app.Shutdown(); // Shutdown
}


#ifdef VISUAL
void ControlCamera(I3DEngine* engine, ICamera* camera, float frameTime)
{
	// Control the camera

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