// Implementation from
// https://www.geeksforgeeks.org/quad-tree/
// Updated for this project

#include "QuadTree.h"
#include <cmath>
#include "Collision.h"

QuadTree::QuadTree(CVector2 topL, CVector2 botR)
{
    m_TopLeft = topL;
    m_BotRight = botR;

    m_Node = nullptr;

    m_TopLeftTree = nullptr;
    m_TopRightTree = nullptr;
    m_BotLeftTree = nullptr;
    m_BotRightTree = nullptr;
    
}

void QuadTree::Add(Circle* circle)
{
    if (circle == nullptr)
        return;

    Node* node = new Node(circle->Position, circle);

    // Current QuadTree cannot contain it
    if (!InBoundary(node->pos))
        return;

    // We are at a QuadTree of unit area
    // We cannot subdivide this QuadTree further
    if (std::abs(m_TopLeft.x - m_BotRight.x) <= 1
        && std::abs(m_TopLeft.y - m_BotRight.y) <= 1)
    {
        if (m_Node == nullptr)
            m_Node = node;
        return;
    }

    if ((m_TopLeft.x + m_BotRight.x) / 2 >= node->pos.x)
    {
        // Indicates m_TopLeftTree
        if ((m_TopLeft.y + m_BotRight.y) / 2 >= node->pos.y)
        {
            if (m_TopLeftTree == nullptr)
                m_TopLeftTree = new QuadTree(
                    CVector2(m_TopLeft.x, m_TopLeft.y),
                    CVector2((m_TopLeft.x + m_BotRight.x) / 2,
                        (m_TopLeft.y + m_BotRight.y) / 2));
            m_TopLeftTree->Add(node->circle);
        }

        // Indicates botLeftTree
        else
        {
            if (m_BotLeftTree == nullptr)
                m_BotLeftTree = new QuadTree(
                    CVector2(m_TopLeft.x,
                        (m_TopLeft.y + m_BotRight.y) / 2),
                    CVector2((m_TopLeft.x + m_BotRight.x) / 2,
                        m_BotRight.y));
            m_BotLeftTree->Add(node->circle);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((m_TopLeft.y + m_BotRight.y) / 2 >= node->pos.y)
        {
            if (m_TopRightTree == nullptr)
                m_TopRightTree = new QuadTree(
                    CVector2((m_TopLeft.x + m_BotRight.x) / 2,
                        m_TopLeft.y),
                    CVector2(m_BotRight.x,
                        (m_TopLeft.y + m_BotRight.y) / 2));
            m_TopRightTree->Add(node->circle);
        }

        // Indicates m_BotRightTree
        else
        {
            if (m_BotRightTree == nullptr)
                m_BotRightTree = new QuadTree(
                    CVector2((m_TopLeft.x + m_BotRight.x) / 2,
                        (m_TopLeft.y + m_BotRight.y) / 2),
                    CVector2(m_BotRight.x, m_BotRight.y));
            m_BotRightTree->Add(node->circle);
        }
    }
}

Node* QuadTree::Search(CVector2 point)
{
    // Current quad cannot contain it
    if (!InBoundary(point))
        return nullptr;

    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (m_Node != nullptr)
        return m_Node;

    if ((m_TopLeft.x + m_BotRight.x) / 2 >= point.x)
    {
        // Indicates topLeftTree
        if ((m_TopLeft.y + m_BotRight.y) / 2 >= point.y)
        {
            if (m_TopLeftTree == nullptr)
                return nullptr;
            return m_TopLeftTree->Search(point);
        }

        // Indicates botLeftTree
        else
        {
            if (m_BotLeftTree == nullptr)
                return nullptr;
            return m_BotLeftTree->Search(point);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((m_TopLeft.y + m_BotRight.y) / 2 >= point.y)
        {
            if (m_TopRightTree == nullptr)
                return nullptr;
            return m_TopRightTree->Search(point);
        }

        // Indicates botRightTree
        else
        {
            if (m_BotRightTree == nullptr)
                return nullptr;
            return m_BotRightTree->Search(point);
        }
    }
}

bool QuadTree::InBoundary(CVector2 point)
{
    return (point.x >= m_TopLeft.x && point.x <= m_BotRight.x
        && point.y >= m_TopLeft.y && point.y <= m_BotRight.y);
}

void QuadTree::CheckCollisions(std::vector<Circle*>& Circles)
{
    // Check for collisions with circles in this node
    if (m_Node != nullptr)
    {
        for (Circle* circle : Circles)
        {
            if (m_Node->circle != circle)
            {
                Collision::CircleToCirlce(circle, m_Node->circle);
            }
        }
    }

    // Recursively check for collisions with circles in child nodes
    if (m_TopLeftTree != nullptr)
        m_TopLeftTree->CheckCollisions(Circles);
    if (m_TopRightTree != nullptr)
        m_TopRightTree->CheckCollisions(Circles);
    if (m_BotLeftTree != nullptr)
        m_BotLeftTree->CheckCollisions(Circles);
    if (m_BotRightTree != nullptr)
        m_BotRightTree->CheckCollisions(Circles);
}
