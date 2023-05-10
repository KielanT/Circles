#include "QuadTree.h"

QuadTree::QuadTree(int pLevel, Rectangle pBounds)
{
	level = pLevel;
	bounds = pBounds;

    for (int i = 0; i < 4; i++)
    {
        nodes[i] = nullptr;
    }
}

void QuadTree::Insert(Circle* circle)
{
    if (nodes[0] != nullptr)
    {
        int index = GetIndex(circle->Bounds);
        if (index != -1)
        {
            nodes[index]->Insert(circle);
            return;
        }
    }

    objects.push_back(circle);
    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
    {
        if (nodes[0] == nullptr)
        {
            Split();
        }
        int i = 0;
        while (i < objects.size())
        {
            int index = GetIndex(objects[i]->Bounds);
            if (index != -1)
            {
                nodes[index]->Insert(objects[i]);
                objects.erase(objects.begin() + i);
            }
            else
            {
                i++;
            }
        }
    }
}

void QuadTree::Split()
{
    int subWidth = (int)(bounds.Width / 2);
    int subHeight = (int)(bounds.Height / 2);
    int x = (int)bounds.X;
    int y = (int)bounds.Y;
    nodes[0] = new QuadTree(level + 1, Rectangle(x + subWidth, y, subWidth, subHeight));
    nodes[1] = new QuadTree(level + 1, Rectangle(x, y, subWidth, subHeight));
    nodes[2] = new QuadTree(level + 1, Rectangle(x, y + subHeight, subWidth, subHeight));
    nodes[3] = new QuadTree(level + 1, Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
}

int QuadTree::GetIndex(Rectangle rect)
{
    int index = -1;
    double verticalMidpoint = bounds.X + (bounds.Width / 2);
    double horizontalMidpoint = bounds.Y + (bounds.Height / 2);
    // Object can completely fit within the top quadrants 
    bool topQuadrant = (rect.Y < horizontalMidpoint && rect.Y + rect.Height < horizontalMidpoint);
    // Object can completely fit within the bottom quadrants 
    bool bottomQuadrant = (rect.Y > horizontalMidpoint);
    // Object can completely fit within the left quadrants 
    if (rect.X < verticalMidpoint && rect.X + rect.Width < verticalMidpoint)
    {
        if (topQuadrant)
        {
            index = 1;
        }
        else if (bottomQuadrant)
        {
            index = 2;
        }
    }
    // Object can completely fit within the right quadrants 
    else if (rect.X > verticalMidpoint)
    {
        if (topQuadrant)
        {
            index = 0;
        }
        else if (bottomQuadrant)
        {
            index = 3;
        }
    }
    return index;
}

std::vector<Circle*> QuadTree::Retrieve(std::vector<Circle*>& returnCircles, Rectangle rect)
{
    int index = GetIndex(rect);
    if (index != -1 && nodes[0] != nullptr)
    {
        nodes[index]->Retrieve(returnCircles, rect);
    }
    returnCircles.insert(returnCircles.end(), objects.begin(), objects.end());
    return returnCircles;
}

void QuadTree::Clear()
{
    if(!objects.empty())
        objects.clear();

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i] != nullptr)
        {
            nodes[i]->Clear();
            nodes[i] = nullptr;
        }
    }
}
