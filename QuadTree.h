#pragma once
// Implementation from
// https://www.geeksforgeeks.org/quad-tree/
// Updated for this project

#include <vector>

#include "Entity.h"
#include "CVector2.h"


// The objects that we want stored in the quadtree
struct Node
{
    CVector2 pos;
    Circle* circle;
    Node(CVector2 _pos, Circle* _cir)
    {
        pos = _pos;
        circle = _cir;
    }

};

class QuadTree
{
public:
    QuadTree(CVector2 topL, CVector2 botR);

    void Add(Circle* circle);
    Node* Search(CVector2 point);
    bool InBoundary(CVector2 point);

    void CheckCollisions(std::vector<Circle*>& Circles);

private:
    CVector2 m_TopLeft;
    CVector2 m_BotRight;

    Node* m_Node;

    // Children of this tree
    QuadTree* m_TopLeftTree;
    QuadTree* m_TopRightTree;
    QuadTree* m_BotLeftTree;
    QuadTree* m_BotRightTree;
};