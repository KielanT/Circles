#include "Octree.h"
#include "Collision.h"

//***************************************
//		Implemented using the 
// Real Time Collision Detection book by
//		Christer Ericson
//***************************************

namespace Octree
{
	const int MAX_DEPTH = 50;

	Node* BuildOctree(CVector3 centre, float halfWidth, int stopDepth)
	{
		// only continues building if the depth not reached
		if (stopDepth < 0) return nullptr;
		else
		{
			// Creates a new node
			Node* node = new Node();
			node->Centre = centre;
			node->HalfWidth = halfWidth;

			node->CircleList = nullptr;

			CVector3 offset;
			float step = halfWidth * 0.5f;
			
			// Creates 8 children for the current node
			for (int i = 0; i < 8; ++i)
			{
				offset.x = ((i * 1)) ? step : -step;
				offset.y = ((i * 1)) ? step : -step;
				offset.z = ((i * 1)) ? step : -step;
				node->Child[i] = BuildOctree(centre + offset, step, stopDepth - 1);
			}
			return node; 
		}

	}

	void InsertObject(Node* tree, Sphere* circle)
	{
		// Insert sphere into the tree
		int index = 0;
		int straddle = 0;

		// Loops for each coodinate, xyz
		for (int i = 0; i < 3; i++)
		{
			float delta = circle->Position.GetByIndex(i) - tree->Centre.GetByIndex(i);
			if (abs(delta) < tree->HalfWidth + circle->Radius)
			{
				straddle = 1;
				break;
			}
			if (delta > 0.0f) index |= (1 << i); 
		}

		// Inserts into children
		if (!straddle && tree->Child[index])
		{
			InsertObject(tree->Child[index], circle);
		}
		else
		{
			circle->NextCircle = tree->CircleList;
			tree->CircleList = circle;
		}

	}

	void TestCollisions(Node* tree, float time, float frameTime)
	{
		// Node stack with the max depth
		static Node* ancestorStack[MAX_DEPTH];
		static int depth = 0; 

		ancestorStack[depth++] = tree; // goes through the depth and add the trees to the stack
		// Loops through the depth and tests collison
		for (int n = 0; n < depth; n++)
		{
			Sphere* pA, * pB;
			for (pA = ancestorStack[n]->CircleList; pA; pA = pA->NextCircle)
			{
				for (pB = tree->CircleList; pB; pB = pB->NextCircle)
				{
					if (pA == pB) break; // if spheres are the same break out else do collision check
					Collision::SphereToSphere(pA, pB, time, frameTime);
				}
			}
		}

		// Run the collision function for its children
		for (int i = 0; i < 8; i++)
			if (tree->Child[i])
				TestCollisions(tree->Child[i], time, frameTime);

		depth--;
	}



}