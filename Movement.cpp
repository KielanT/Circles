#include "Movement.h"

void Movement::Move(Circle* circles, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	auto circlesEnd = circles + numCircles;
	while (circles != circlesEnd)
	{
		circles->Position += (speed * circles->Velocity) * frameTime;

		if (circles->Position.x < -range || circles->Position.x > range)
		{
			circles->Velocity.x = -circles->Velocity.x;
		}

		if (circles->Position.y < -range || circles->Position.y > range)
		{
			circles->Velocity.y = -circles->Velocity.y;
		}

		++circles;
	}

}

void Movement::Move(Sphere* spheres, const int& numSphere, const float& speed, const float& range, const float& frameTime)
{
	auto spheresEnd = spheres + numSphere;
	while (spheres != spheresEnd)
	{
		spheres->Position += (speed * spheres->Velocity) * frameTime;

		if (spheres->Position.x < -range || spheres->Position.x > range)
		{
			spheres->Velocity.x = -spheres->Velocity.x;
		}

		if (spheres->Position.y < -range || spheres->Position.y > range)
		{
			spheres->Velocity.y = -spheres->Velocity.y;
		}

		++spheres;
	}
}

void Movement::Move(Circle* circles, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	auto circlesEnd = circles + numCircles;
	while (circles != circlesEnd)
	{
		circles->Position += (speed * circles->Velocity) * frameTime;

		if (circles->Position.x < -range || circles->Position.x > range)
		{
			circles->Velocity.x = -circles->Velocity.x;
		}

		if (circles->Position.y < -range || circles->Position.y > range)
		{
			circles->Velocity.y = -circles->Velocity.y;
		}

		modelsMap[circles->Name]->SetPosition(circles->Position.x, circles->Position.y, 0);

		++circles;
	}
}

void Movement::Move(Sphere* spheres, std::map<std::string, tle::IModel*>& modelsMap, const int& numSphere, const float& speed, const float& range, const float& frameTime)
{
	auto spheresEnd = spheres + numSphere;
	while (spheres != spheresEnd)
	{
		spheres->Position += (speed * spheres->Velocity) * frameTime;

		if (spheres->Position.x < -range || spheres->Position.x > range)
		{
			spheres->Velocity.x = -spheres->Velocity.x;
		}

		if (spheres->Position.y < -range || spheres->Position.y > range)
		{
			spheres->Velocity.y = -spheres->Velocity.y;
		}

		modelsMap[spheres->Name]->SetPosition(spheres->Position.x, spheres->Position.y, 0);

		++spheres;
	}
}



