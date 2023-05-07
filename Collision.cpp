#include "Collision.h"

void Collision::SpheresToSpheres(Circle* MovingCircles, Circle* BlockCircles, uint32_t numCircles)
{
	auto MovingCirclesEnd = MovingCircles + (numCircles / 2);
	auto BlockCirclesEnd = BlockCircles + (numCircles / 2);
	
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

		SphereToSpheres(*MovingCircles, BlockCircles, numCircles);

		++MovingCircles;
	}
}

void Collision::SphereToSpheres(Circle& movingCirlce, Circle* BlockCircles, uint32_t numCircles)
{
	auto BlockCirclesEnd = BlockCircles + (numCircles / 2);
	while (BlockCircles != BlockCirclesEnd)
	{
		const float distance = sqrt((movingCirlce.Position.x - BlockCircles->Position.x) * (movingCirlce.Position.x - BlockCircles->Position.x) +
									(movingCirlce.Position.y - BlockCircles->Position.y) * (movingCirlce.Position.y - BlockCircles->Position.y) +
									(movingCirlce.Position.z - BlockCircles->Position.z) * (movingCirlce.Position.z - BlockCircles->Position.z));

		if (distance < movingCirlce.Radius + BlockCircles->Radius)
		{
			CVector3 normal = BlockCircles->Position - movingCirlce.Position;
			movingCirlce.Velocity = Reflect(movingCirlce.Velocity, Normalise(normal));
			
			std::cout << "Collision Between: " << movingCirlce.Name << " and " << BlockCircles->Name << std::endl;
		}

		++BlockCircles;
	}
}

