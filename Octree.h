#pragma once
#include "Entity.h"
#include "CVector3.h"

namespace Octree
{

	struct Node
	{
		CVector3 Centre;
		float HalfWidth;
		Node* Child[8];
		Sphere* CircleList = nullptr;
	};

	Node* BuildOctree(CVector3 centre, float halfWidth, int stopDepth);

	void InsertObject(Node* tree, Sphere* circle);

	void TestCollisions(Node* tree, float time, float frameTime);
}