#include "QuadTree.h"

QuadTree::~QuadTree()
{
	if(m_NorthWest != nullptr) delete m_NorthWest;
	if(m_NorthEast != nullptr) delete m_NorthEast;
	if(m_SouthWest != nullptr) delete m_SouthWest;
	if(m_SouthEast != nullptr) delete m_SouthEast;
}

bool QuadTree::Insert(Circle* circle)
{
	if (!Boundary.Contains(circle->Position))
	{
		return false;
	}

	if (m_Circles.size() < m_NodeCapacity)
	{
		m_Circles.push_back(circle);
		return true;
	}

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
	CVector2 centre = Boundary.Centre;
	float halfDimension = Boundary.HalfDimension / 2.0f;

	m_NorthWest = new QuadTree(AABB(centre - CVector2(halfDimension, halfDimension), halfDimension));
	m_NorthEast = new QuadTree(AABB(centre + CVector2(halfDimension, -halfDimension), halfDimension));
	m_SouthWest = new QuadTree(AABB(centre + CVector2(-halfDimension, halfDimension), halfDimension));
	m_SouthEast = new QuadTree(AABB(centre + CVector2(halfDimension, halfDimension), halfDimension));
}

std::vector<Circle*> QuadTree::QueryRange(AABB range)
{
	std::vector<Circle*> CirclesInRange;

	if (!Boundary.Intersects(range))
	{
		return CirclesInRange;
	}

	for (auto& cir : m_Circles)
	{
		if (range.Contains(cir->Position))
		{
			CirclesInRange.push_back(cir);
		}
	}

	if (m_NorthWest == nullptr)
	{
		return CirclesInRange;
	}

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
