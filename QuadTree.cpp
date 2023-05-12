#include "QuadTree.h"

namespace QuadTree
{

	QuadTree::~QuadTree()
	{
		// Delete the children
		if (m_NorthWest != nullptr) delete m_NorthWest;
		if (m_NorthEast != nullptr) delete m_NorthEast;
		if (m_SouthWest != nullptr) delete m_SouthWest;
		if (m_SouthEast != nullptr) delete m_SouthEast;
	}


	//void QuadTree::Subdivide()
	//{
	//	// Subdivde based on the bounday and half it
	//	CVector2 centre = Boundary.Centre;
	//	float halfDimension = Boundary.HalfDimension / 2.0f;

	//	m_NorthWest = new QuadTree(AABB(centre - CVector2(halfDimension, halfDimension), halfDimension), m_NodeCapacity);
	//	m_NorthEast = new QuadTree(AABB(centre + CVector2(halfDimension, -halfDimension), halfDimension), m_NodeCapacity);
	//	m_SouthWest = new QuadTree(AABB(centre + CVector2(-halfDimension, halfDimension), halfDimension), m_NodeCapacity);
	//	m_SouthEast = new QuadTree(AABB(centre + CVector2(halfDimension, halfDimension), halfDimension), m_NodeCapacity);
	//}


	//void QuadTree::Clear()
	//{
	//	// Clear all the circles in all the quads
	//	m_Circles.clear();

	//	if (m_NorthWest != nullptr)
	//	{
	//		m_NorthWest->Clear();
	//		delete m_NorthWest;
	//		m_NorthWest = nullptr;
	//	}

	//	if (m_NorthEast != nullptr)
	//	{
	//		m_NorthEast->Clear();
	//		delete m_NorthEast;
	//		m_NorthEast = nullptr;
	//	}

	//	if (m_SouthWest != nullptr)
	//	{
	//		m_SouthWest->Clear();
	//		delete m_SouthWest;
	//		m_SouthWest = nullptr;
	//	}

	//	if (m_SouthEast != nullptr)
	//	{
	//		m_SouthEast->Clear();
	//		delete m_SouthEast;
	//		m_SouthEast = nullptr;
	//	}
	//}

	bool Insert(QuadTree* tree, Circle* circle)
	{
		// Insert the circle

		// If the circle is not in the main boundary then return
		if (!Contains(tree->Boundary, circle->Position))
		{
			return false;
		}

		// Make sure that the node isn't full then add to it
		if (tree->m_Circles.size() < tree->m_NodeCapacity)
		{
			tree->m_Circles.push_back(circle);
			return true;
		}

		// If node capacity reach, subdivde and fill 
		if (tree->m_NorthWest == nullptr)
		{
			Subdivide(tree);
		}

		if (Insert(tree->m_NorthWest, circle)) return true;
		if (Insert(tree->m_NorthEast, circle)) return true;
		if (Insert(tree->m_SouthWest, circle)) return true;
		if (Insert(tree->m_SouthEast, circle)) return true;

		return false;
	}

	void Subdivide(QuadTree* tree)
	{
		// Subdivde based on the bounday and half it
		CVector2 centre = tree->Boundary.Centre;
		float halfDimension = tree->Boundary.HalfDimension / 2.0f;

		tree->m_NorthWest = new QuadTree(AABB(centre - CVector2(halfDimension, halfDimension), halfDimension), tree->m_NodeCapacity);
		tree->m_NorthEast = new QuadTree(AABB(centre + CVector2(halfDimension, -halfDimension), halfDimension), tree->m_NodeCapacity);
		tree->m_SouthWest = new QuadTree(AABB(centre + CVector2(-halfDimension, halfDimension), halfDimension), tree->m_NodeCapacity);
		tree->m_SouthEast = new QuadTree(AABB(centre + CVector2(halfDimension, halfDimension), halfDimension), tree->m_NodeCapacity);
	}


	std::vector<Circle*> QueryRange(QuadTree* tree, AABB range)
	{
		// Vector of circles in range
		std::vector<Circle*> CirclesInRange;

		// If the range is not in boundary, return
		if (!Intersects(tree->Boundary, range))
		{
			return CirclesInRange;
		}

		// for all the circles on this quad 
		// Add to the vector
		for (auto& cir : tree->m_Circles)
		{
			if (Contains(range, cir->Position))
			{
				CirclesInRange.push_back(cir);
			}
		}

		// if the quad has no children return
		if (tree->m_NorthWest == nullptr)
		{
			return CirclesInRange;
		}

		// if has child add to the array

		std::vector<Circle*> nwCircles = QueryRange(tree->m_NorthWest, range);
		CirclesInRange.insert(CirclesInRange.end(), nwCircles.begin(), nwCircles.end());

		std::vector<Circle*> neCircles = QueryRange(tree->m_NorthEast, range);
		CirclesInRange.insert(CirclesInRange.end(), neCircles.begin(), neCircles.end());

		std::vector<Circle*> swCircles = QueryRange(tree->m_SouthWest, range);
		CirclesInRange.insert(CirclesInRange.end(), swCircles.begin(), swCircles.end());

		std::vector<Circle*> seCircles = QueryRange(tree->m_SouthEast, range);
		CirclesInRange.insert(CirclesInRange.end(), seCircles.begin(), seCircles.end());

		return CirclesInRange;
	}

	void Clear(QuadTree* tree)
	{
		// Clear all the circles in all the quads
		tree->m_Circles.clear();

		if (tree->m_NorthWest != nullptr)
		{
			Clear(tree->m_NorthWest);
			delete tree->m_NorthWest;
			tree->m_NorthWest = nullptr;
		}

		if (tree->m_NorthEast != nullptr)
		{
			Clear(tree->m_NorthEast);
			delete tree->m_NorthEast;
			tree->m_NorthEast = nullptr;
		}

		if (tree->m_SouthWest != nullptr)
		{
			Clear(tree->m_SouthWest);
			delete tree->m_SouthWest;
			tree->m_SouthWest = nullptr;
		}

		if (tree->m_SouthEast != nullptr)
		{
			Clear(tree->m_SouthEast);
			delete tree->m_SouthEast;
			tree->m_SouthEast = nullptr;
		}
	}

}
	