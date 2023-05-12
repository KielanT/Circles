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

	bool QuadTree::Insert(Circle* circle)
	{
		// Insert the circle

		// If the circle is not in the main boundary then return
		if (!Contains(Boundary, circle->Position))
		{
			return false;
		}

		// Make sure that the node isn't full then add to it
		if (m_Circles.size() < m_NodeCapacity)
		{
			m_Circles.push_back(circle);
			return true;
		}

		// If node capacity reach, subdivde and fill 
		if (m_NorthWest == nullptr)
		{
			Subdivide();
		}

		if (m_NorthWest->Insert(circle)) return true;
		if (m_NorthEast->Insert(circle)) return true;
		if (m_SouthWest->Insert(circle)) return true;
		if (m_SouthEast->Insert(circle)) return true;

		return false;
	}

	void QuadTree::Subdivide()
	{
		// Subdivde based on the bounday and half it
		CVector2 centre = Boundary.Centre;
		float halfDimension = Boundary.HalfDimension / 2.0f;

		m_NorthWest = new QuadTree(AABB(centre - CVector2(halfDimension, halfDimension), halfDimension), m_NodeCapacity);
		m_NorthEast = new QuadTree(AABB(centre + CVector2(halfDimension, -halfDimension), halfDimension), m_NodeCapacity);
		m_SouthWest = new QuadTree(AABB(centre + CVector2(-halfDimension, halfDimension), halfDimension), m_NodeCapacity);
		m_SouthEast = new QuadTree(AABB(centre + CVector2(halfDimension, halfDimension), halfDimension), m_NodeCapacity);
	}

	std::vector<Circle*> QuadTree::QueryRange(AABB range)
	{
		// Vector of circles in range
		std::vector<Circle*> CirclesInRange;

		// If the range is not in boundary, return
		if (!Intersects(Boundary, range))
		{
			return CirclesInRange;
		}

		// for all the circles on this quad 
		// Add to the vector
		for (auto& cir : m_Circles)
		{	
			if (Contains(range, cir->Position))
			{
				CirclesInRange.push_back(cir);
			}
		}

		// if the quad has no children return
		if (m_NorthWest == nullptr)
		{
			return CirclesInRange;
		}

		// if has child add to the array

		std::vector<Circle*> nwCircles = m_NorthWest->QueryRange(range);
		CirclesInRange.insert(CirclesInRange.end(), nwCircles.begin(), nwCircles.end());

		std::vector<Circle*> neCircles = m_NorthEast->QueryRange(range);
		CirclesInRange.insert(CirclesInRange.end(), neCircles.begin(), neCircles.end());

		std::vector<Circle*> swCircles = m_SouthWest->QueryRange(range);
		CirclesInRange.insert(CirclesInRange.end(), swCircles.begin(), swCircles.end());

		std::vector<Circle*> seCircles = m_SouthEast->QueryRange(range);
		CirclesInRange.insert(CirclesInRange.end(), seCircles.begin(), seCircles.end());

		return CirclesInRange;
	}



	void QuadTree::Clear()
	{
		// Clear all the circles in all the quads
		m_Circles.clear();

		if (m_NorthWest != nullptr)
		{
			m_NorthWest->Clear();
			delete m_NorthWest;
			m_NorthWest = nullptr;
		}

		if (m_NorthEast != nullptr)
		{
			m_NorthEast->Clear();
			delete m_NorthEast;
			m_NorthEast = nullptr;
		}

		if (m_SouthWest != nullptr)
		{
			m_SouthWest->Clear();
			delete m_SouthWest;
			m_SouthWest = nullptr;
		}

		if (m_SouthEast != nullptr)
		{
			m_SouthEast->Clear();
			delete m_SouthEast;
			m_SouthEast = nullptr;
		}
	}
}