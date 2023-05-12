#pragma once

#include <vector>
#include <array>
#include "Entity.h"
namespace QuadTree
{

	struct QuadTree
	{
		// Set the quad tree boundary and the amount a node can store
		QuadTree(AABB boundary, int nodeCapacity) : Boundary(boundary), m_NodeCapacity(nodeCapacity) {}
		~QuadTree();


		// World boundary
		AABB Boundary;

		std::vector<Circle*> m_Circles; // Circles in quad tree

		
		// Quad tree nodes
		QuadTree* m_NorthWest = nullptr;
		QuadTree* m_NorthEast = nullptr;
		QuadTree* m_SouthWest = nullptr;
		QuadTree* m_SouthEast = nullptr;

		// Amount the quad tree nodes can hold
		int m_NodeCapacity;
	};


	// Insert into quad tree
	bool Insert(QuadTree* tree, Circle* circle);

	// Subdivide quadtree node
	void Subdivide(QuadTree* tree);

	// Query the range
	std::vector<Circle*> QueryRange(QuadTree* tree, AABB range);

	// Clear the quad trees
	void Clear(QuadTree* tree);
}

