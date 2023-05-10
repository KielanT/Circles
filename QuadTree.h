#pragma once

#include <vector>
#include <array>
#include "Entity.h"
#include "Rectangle.h"


class QuadTree
{
public:
	QuadTree(int pLevel, Rectangle pBounds);

	void Insert(Circle* circle);

	void Split();
	
	int GetIndex(Rectangle rect);

	std::vector<Circle*> Retrieve(std::vector<Circle*>& returnCircles, Rectangle rect);

	void Clear();


private:
	int MAX_OBJECTS = 10;
	int MAX_LEVELS = 5;
	int level;
	std::vector<Circle*> objects;
	Rectangle bounds;
	std::array<QuadTree*, 4> nodes;
};

