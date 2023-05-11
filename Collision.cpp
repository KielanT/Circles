#include "Collision.h"
#include <mutex>



void Collision::SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numMovingCircles, uint32_t numBlockCircles, float time, float deltaTime)
{
	auto MovingCirclesEnd = MovingCircles + numMovingCircles;
	auto BlockCirclesEnd = BlockCircles + numBlockCircles;
	
	while (MovingCircles != MovingCirclesEnd)
	{
		//auto blocker = BlockCircles;
		//while (blocker != BlockCirclesEnd)
		//{
		//	const float distance = sqrt((MovingCircles->Position.x - BlockCircles->Position.x) * (MovingCircles->Position.x - BlockCircles->Position.x) +
		//								(MovingCircles->Position.y - BlockCircles->Position.y) * (MovingCircles->Position.y - BlockCircles->Position.y) +
		//								(MovingCircles->Position.z - BlockCircles->Position.z) * (MovingCircles->Position.z - BlockCircles->Position.z));
		//
		//	if (distance < MovingCircles->Radius + BlockCircles->Radius)
		//	{
		//		MovingCircles->Velocity = -MovingCircles->Velocity;
		//		std::cout << "Collision Between: " << MovingCircles->Name << " and " << BlockCircles->Name << std::endl;
		//	}
		//
		//	++blocker;
		//}

		// Unsure to why the commented outcode does not work

		SphereToSpheres(*MovingCircles, BlockCircles, numBlockCircles, time, deltaTime);

		++MovingCircles;
	}
}

void Collision::SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numBlockCircles, float time, float deltaTime)
{
	static std::mutex coutMutex;

	auto BlockCirclesEnd = BlockCircles + numBlockCircles;
	while (BlockCircles != BlockCirclesEnd)
	{
		const float distance = sqrt((movingCirlce.Position.x - BlockCircles->Position.x) * (movingCirlce.Position.x - BlockCircles->Position.x) +
									(movingCirlce.Position.y - BlockCircles->Position.y) * (movingCirlce.Position.y - BlockCircles->Position.y) /*+
									(movingCirlce.Position.z - BlockCircles->Position.z) * (movingCirlce.Position.z - BlockCircles->Position.z)*/);

		if (distance < movingCirlce.Radius + BlockCircles->Radius)
		{
			CVector2 normal = BlockCircles->Position - movingCirlce.Position;
			movingCirlce.Velocity = Reflect(movingCirlce.Velocity, Normalise(normal));
			
			movingCirlce.HP -= 20;
			BlockCircles->HP -= 20;

			// Stops the couts from getting jumbled with the other threads
			std::unique_lock<std::mutex> lock(coutMutex);
			{
				std::cout << std::endl;
				std::cout << "Collision Between: " << movingCirlce.Name << " and " << BlockCircles->Name << std::endl;
				std::cout << "Time of collision: " << time << " seconds since start of program" << std::endl;
				std::cout << movingCirlce.Name << " HP:" << std::to_string(movingCirlce.HP) << std::endl;
				std::cout << BlockCircles->Name << " HP:" << std::to_string(BlockCircles->HP) << std::endl;
				std::cout << std::endl;
				std::cout << "Delta Time: " << deltaTime << std::endl;
			}
		}

		++BlockCircles;
	}
}

void Collision::CircleToCirlce(Circle* circle, Circle* other, float time, float DeltaTime)
{
	const float distance = sqrt((circle->Position.x - other->Position.x) * (circle->Position.x - other->Position.x) +
								(circle->Position.y - other->Position.y) * (circle->Position.y - other->Position.y));
	static std::mutex coutMutex;
	

	if (distance < circle->Radius + other->Radius)
	{
		CVector2 normal = circle->Position - other->Position;
		circle->Velocity = Reflect(circle->Velocity, Normalise(normal));
		other->Velocity = Reflect(other->Velocity, Normalise(normal));
	
		
		circle->HP -= 20;
		other->HP -= 20;
	
		std::unique_lock<std::mutex> lock(coutMutex);
		{
			std::cout << std::endl;
			std::cout << "Collision Between: " << circle->Name << " and " << other->Name << std::endl;
			std::cout << circle->Name << ": " << ToString(circle->Colour) << other->Name << ": " << ToString(other->Colour) << std::endl;
			std::cout << "Time of collision: " << time << " seconds since start of program" << std::endl;
			std::cout << circle->Name << " HP:" << std::to_string(circle->HP) << std::endl;
			std::cout << other->Name << " HP:" << std::to_string(other->HP) << std::endl;
			std::cout << std::endl;
			std::cout << "Frame Time: " << DeltaTime << std::endl;
			std::cout << std::endl;
		}
	}
}

