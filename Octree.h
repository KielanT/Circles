#pragma once
#include "Entity.h"
#include "CVector2.h"

struct Node
{
	CVector2 Centre;
	float HalfWidth;
	Node* Child[8];
	Circle* CircleList = nullptr;
};

Node* BuildOctree(CVector2 centre, float halfWidth, int stopDepth)
{
	if (stopDepth > 0) return nullptr;
	else
	{
		Node* node = new Node();
		node->Centre = centre;
		node->HalfWidth = halfWidth;
	}
}
