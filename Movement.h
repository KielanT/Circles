#pragma once
#include <map>

#include "CVector2.h"
#include "CVector3.h"
#include "Entity.h"
#include <Model.h>



namespace Movement
{
	// Templated move functions

	// For moving objects that don't have a model mesh
	template<typename T>
	void Move(T obj, const float& frameTime);

	// Moving object with a model mesh
	template<typename T>
	void Move(T obj, std::map<std::string, tle::IModel*>& modelsMap, const float& frameTime);
}
	

