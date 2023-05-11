#include "Movement.h"

template<typename T>
void Movement::Move(T obj, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	/*auto objEnd = obj + numCircles;
	while (obj != circlesEnd)
	{
		obj->Position += (speed * obj->Velocity) * frameTime;

		if (obj->Position.x < -range || obj->Position.x > range)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -range || obj->Position.y > range)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		++obj;
	}*/
}

template<typename T>
void Movement::Move(T obj, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
}


template<> 
void Movement::Move<Circle*>(Circle* obj, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	auto circlesEnd = obj + numCircles;
	while (obj != circlesEnd)
	{
		obj->Position += (speed * obj->Velocity) * frameTime;

		if (obj->Position.x < -range || obj->Position.x > range)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -range || obj->Position.y > range)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		++obj;
	}
}

template<>
void Movement::Move<Sphere*>(Sphere* obj, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	auto circlesEnd = obj + numCircles;
	while (obj != circlesEnd)
	{
		obj->Position += (speed * obj->Velocity) * frameTime;

		if (obj->Position.x < -range || obj->Position.x > range)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -range || obj->Position.y > range)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		++obj;
	}
}


template<>
void Movement::Move<Circle*>(Circle* obj, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	auto circlesEnd = obj + numCircles;
	while (obj != circlesEnd)
	{
		
		obj->Position += (speed * obj->Velocity) * frameTime;

		if (obj->Position.x < -range || obj->Position.x > range)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -range || obj->Position.y > range)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		
		if (modelsMap[obj->Name] != nullptr)
		{
			modelsMap[obj->Name]->SetPosition(obj->Position.x, obj->Position.y, 0);
		}

		++obj;
	}
}

template<>
void Movement::Move<Sphere*>(Sphere* obj, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime)
{
	auto circlesEnd = obj + numCircles;
	while (obj != circlesEnd)
	{
		obj->Position += (speed * obj->Velocity) * frameTime;

		if (obj->Position.x < -range || obj->Position.x > range)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -range || obj->Position.y > range)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		modelsMap[obj->Name]->SetPosition(obj->Position.x, obj->Position.y, 0);

		++obj;
	}
}

//
//void Movement::Move(Circle* circles, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime)
//{
//	auto circlesEnd = circles + numCircles;
//	while (circles != circlesEnd)
//	{
//		circles->Position += (speed * circles->Velocity) * frameTime;
//
//		if (circles->Position.x < -range || circles->Position.x > range)
//		{
//			circles->Velocity.x = -circles->Velocity.x;
//		}
//
//		if (circles->Position.y < -range || circles->Position.y > range)
//		{
//			circles->Velocity.y = -circles->Velocity.y;
//		}
//
//		modelsMap[circles->Name]->SetPosition(circles->Position.x, circles->Position.y, 0);
//
//		++circles;
//	}
//}
//
//void Movement::Move(Sphere* spheres, std::map<std::string, tle::IModel*>& modelsMap, const int& numSphere, const float& speed, const float& range, const float& frameTime)
//{
//	auto spheresEnd = spheres + numSphere;
//	while (spheres != spheresEnd)
//	{
//		spheres->Position += (speed * spheres->Velocity) * frameTime;
//
//		if (spheres->Position.x < -range || spheres->Position.x > range)
//		{
//			spheres->Velocity.x = -spheres->Velocity.x;
//		}
//
//		if (spheres->Position.y < -range || spheres->Position.y > range)
//		{
//			spheres->Velocity.y = -spheres->Velocity.y;
//		}
//
//		modelsMap[spheres->Name]->SetPosition(spheres->Position.x, spheres->Position.y, 0);
//
//		++spheres;
//	}
//}
//
//
//
