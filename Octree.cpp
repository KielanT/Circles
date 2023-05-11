#include "Octree.h"

namespace Octree
{
	Node* BuildOctree(CVector3 centre, float halfWidth, int stopDepth)
	{
		if (stopDepth > 0) return nullptr;
		else
		{
			Node* node = new Node();
			node->Centre = centre;
			node->HalfWidth = halfWidth;

			node->CircleList = nullptr;

			CVector3 offset;
			float step = halfWidth * 0.5f;
			
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
		int index = 0;
		int straddle = 0;

		for (int i = 0; i < 3; i++)
		{
			float delta = circle->Position.GetByIndex(i) - tree->Centre.GetByIndex(i);
			if (abs(delta) < tree->HalfWidth + circle->Radius)
			{
				straddle = 1;
				break;
			}
			if (delta > 0.0f) index |= (1 << i); // ZYX
		}

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

	void TestCollisions(Node* tree)
	{
		const int MAX_DEPTH = 40;
		static Node* ancestorStack[MAX_DEPTH];
		static int depth = 0; // �Depth == 0� is invariant over calls

		ancestorStack[depth++] = tree;
		for (int n = 0; n < depth; n++)
		{
			Sphere* pA, * pB;
			for (pA = ancestorStack[n]->CircleList; pA; pA = pA->NextCircle)
			{
				for (pB = tree->CircleList; pB; pB = pB->NextCircle)
				{
					// Avoid testing both A->B and B->A
					if (pA == pB) break;
					// Now perform the collision test between pA and pB in some manner
					TestCollision(pA, pB);
				}
			}
		}
		// Recursively visit all existing children
		for (int i = 0; i < 8; i++)
			if (tree->Child[i])
				TestCollisions(tree->Child[i]);
		// Remove current node from ancestor stack before returning
		depth--;
	}
	
}