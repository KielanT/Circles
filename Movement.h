#pragma once
#include <map>

#include "CVector2.h"
#include "CVector3.h"
#include "Entity.h"
#include <Model.h>



namespace Movement
{
	template<typename T>
	void Move(T obj, const int& numCircles, const float& speed, const float& range, const float& frameTime);

	template<typename T>
	void Move(T obj, std::map<std::string, tle::IModel*>& modelsMap, const int& numCircles, const float& speed, const float& range, const float& frameTime);
}
	

