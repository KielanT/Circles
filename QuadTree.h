#pragma once

#include <vector>
#include <array>
#include "Entity.h"
namespace QuadTree
{

	class QuadTree
	{
	public:
		QuadTree(AABB boundary, int nodeCapacity) : Boundary(boundary), m_NodeCapacity(nodeCapacity) {}
		~QuadTree();

		bool Insert(Circle* circle);

		void Subdivide();

		std::vector<Circle*> QueryRange(AABB range);

		AABB Boundary;

		void Clear();
	private:
		int m_NodeCapacity;

		std::vector<Circle*> m_Circles;

		QuadTree* m_NorthWest = nullptr;
		QuadTree* m_NorthEast = nullptr;
		QuadTree* m_SouthWest = nullptr;
		QuadTree* m_SouthEast = nullptr;
	};

}

