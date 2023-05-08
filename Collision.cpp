#include "Collision.h"
#include <mutex>



void Collision::SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numMovingCircles, uint32_t numBlockCircles, float time)
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

		SphereToSpheres(*MovingCircles, BlockCircles, numBlockCircles, time);

		++MovingCircles;
	}
}

void Collision::SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numBlockCircles, float time)
{
	static std::mutex coutMutex;

	auto BlockCirclesEnd = BlockCircles + numBlockCircles;
	while (BlockCircles != BlockCirclesEnd)
	{
		const float distance = sqrt((movingCirlce.Position.x - BlockCircles->Position.x) * (movingCirlce.Position.x - BlockCircles->Position.x) +
									(movingCirlce.Position.y - BlockCircles->Position.y) * (movingCirlce.Position.y - BlockCircles->Position.y) +
									(movingCirlce.Position.z - BlockCircles->Position.z) * (movingCirlce.Position.z - BlockCircles->Position.z));

		if (distance < movingCirlce.Radius + BlockCircles->Radius)
		{
			CVector3 normal = BlockCircles->Position - movingCirlce.Position;
			movingCirlce.Velocity = Reflect(movingCirlce.Velocity, Normalise(normal));
			
			movingCirlce.HP -= 20;
			BlockCircles->HP -= 20;

			// Stops the couts from getting jumbled with the other threads
			std::lock_guard<std::mutex> lock(coutMutex);
			{
				std::cout << std::endl;
				std::cout << "Collision Between: " << movingCirlce.Name << " and " << BlockCircles->Name << std::endl;
				std::cout << "Time of collision: " << time << " seconds since start of program" << std::endl;
				std::cout << movingCirlce.Name << " HP:" << std::to_string(movingCirlce.HP) << std::endl;
				std::cout << BlockCircles->Name << " HP:" << std::to_string(BlockCircles->HP) << std::endl;
				std::cout << std::endl;
			}
		}

		++BlockCircles;
	}
}

