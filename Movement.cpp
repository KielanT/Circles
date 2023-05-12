#include "Movement.h"
#include "Globals.h"

// Defualt move function
template<typename T>
void Movement::Move(T obj, const float& frameTime)
{
	/*auto objEnd = obj + NUM_CIRCLES;
	while (obj != circlesEnd)
	{
		obj->Position += (SPEED * obj->Velocity) * frameTime;

		if (obj->Position.x < -RANGE_POSITION || obj->Position.x > RANGE_POSITION)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -RANGE_POSITION || obj->Position.y > RANGE_POSITION)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		++obj;
	}*/
}

// Defualt move function with map
template<typename T>
void Movement::Move(T obj, std::map<std::string, tle::IModel*>& modelsMap, const float& frameTime)
{
}

// Circle Move function
template<> 
void Movement::Move<Circle*>(Circle* obj, const float& frameTime)
{
	auto circlesEnd = obj + NUM_CIRCLES;
	while (obj != circlesEnd)
	{
		// Changes the positon with speed and velocity
		obj->Position += (SPEED * obj->Velocity) * frameTime;

		// Keeps the object with in the bounds
		if (obj->Position.x < -RANGE_POSITION || obj->Position.x > RANGE_POSITION)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -RANGE_POSITION || obj->Position.y > RANGE_POSITION)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		++obj;
	}
}

// Sphere function
template<>
void Movement::Move<Sphere*>(Sphere* obj, const float& frameTime)
{
	auto circlesEnd = obj + NUM_CIRCLES;
	while (obj != circlesEnd)
	{
		obj->Position += (SPEED * obj->Velocity) * frameTime;

		if (obj->Position.x < -RANGE_POSITION || obj->Position.x > RANGE_POSITION)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -RANGE_POSITION || obj->Position.y > RANGE_POSITION)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		++obj;
	}
}

// Circle move function with map
template<>
void Movement::Move<Circle*>(Circle* obj, std::map<std::string, tle::IModel*>& modelsMap, const float& frameTime)
{
	auto circlesEnd = obj + NUM_CIRCLES;
	while (obj != circlesEnd)
	{
		
		obj->Position += (SPEED * obj->Velocity) * frameTime;

		if (obj->Position.x < -RANGE_POSITION || obj->Position.x > RANGE_POSITION)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -RANGE_POSITION || obj->Position.y > RANGE_POSITION)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		// Moves the model if exists in map
		if (modelsMap[obj->Name] != nullptr)
		{
			modelsMap[obj->Name]->SetPosition(obj->Position.x, obj->Position.y, 0);
		}

		++obj;
	}
}

// Sphere move function with map
template<>
void Movement::Move<Sphere*>(Sphere* obj, std::map<std::string, tle::IModel*>& modelsMap, const float& frameTime)
{
	auto circlesEnd = obj + NUM_CIRCLES;
	while (obj != circlesEnd)
	{
		obj->Position += (SPEED * obj->Velocity) * frameTime;

		if (obj->Position.x < -RANGE_POSITION || obj->Position.x > RANGE_POSITION)
		{
			obj->Velocity.x = -obj->Velocity.x;
		}

		if (obj->Position.y < -RANGE_POSITION || obj->Position.y > RANGE_POSITION)
		{
			obj->Velocity.y = -obj->Velocity.y;
		}

		modelsMap[obj->Name]->SetPosition(obj->Position.x, obj->Position.y, 0);

		++obj;
	}
}
