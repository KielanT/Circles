#pragma once

#include <vector>
#include <array>
#include "Entity.h"

class QuadTree
{
public:
	QuadTree(AABB boundary) : Boundary(boundary){}
	~QuadTree();

	bool Insert(Circle* circle);

	void Subdivide();

	std::vector<Circle*> QueryRange(AABB range);

	AABB Boundary;

	void Clear();
private:
	const int m_NodeCapacity = 4;
	

	std::vector<Circle*> m_Circles;

	QuadTree* m_NorthWest = nullptr;
	QuadTree* m_NorthEast = nullptr;
	QuadTree* m_SouthWest = nullptr;
	QuadTree* m_SouthEast = nullptr;
};



