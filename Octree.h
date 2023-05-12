#pragma once
#include "Entity.h"
#include "CVector3.h"
#include "ThreadHelper.h"

//***************************************
//		Implemented using the 
// Real Time Collision Detection book by
//		Christer Ericson
//***************************************

namespace Octree
{
	// Node for storing all the octree data
	struct Node
	{
		CVector3 Centre;
		float HalfWidth;
		Node* Child[8];
		Sphere* CircleList = nullptr;
	};

	// Build the tree
	Node* BuildOctree(CVector3 centre, float halfWidth, int stopDepth);

	// insert into tree
	void InsertObject(Node* tree, Sphere* circle);
	
	// test collision
	void TestCollisions(Node* tree, float time, float frameTime);


}