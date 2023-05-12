#pragma once

#include <vector>
#include <array>
#include "Entity.h"
namespace QuadTree
{

	class QuadTree
	{
	public:
		// Set the quad tree boundary and the amount a node can store
		QuadTree(AABB boundary, int nodeCapacity) : Boundary(boundary), m_NodeCapacity(nodeCapacity) {}
		~QuadTree();

		// Insert into quad tree
		bool Insert(Circle* circle);

		// Subdivide quadtree node
		void Subdivide();

		// Query the range
		std::vector<Circle*> QueryRange(AABB range);

		// Clear the quad trees
		void Clear();

		// World boundary
		AABB Boundary;

	private:
		// AMount the quad tree nodes can hold
		int m_NodeCapacity;

		std::vector<Circle*> m_Circles; // Circles in quad tree

		// Quad tree nodes
		QuadTree* m_NorthWest = nullptr;
		QuadTree* m_NorthEast = nullptr;
		QuadTree* m_SouthWest = nullptr;
		QuadTree* m_SouthEast = nullptr;
	};

}

