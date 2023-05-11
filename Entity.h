#pragma once

#include <string>
#include "CVector3.h"
#include "CVector2.h"

namespace QuadTree
{

struct AABB
{
    CVector2 Centre;
    float HalfDimension;

    AABB(CVector2 centre, float halfDimension) : Centre(centre), HalfDimension(halfDimension) {}
    AABB() = default;


};

bool Contains(AABB bound, CVector2 point);

bool Intersects(AABB bound, AABB other);
}


struct Circle
{
    CVector2 Position{ 0.0f, 0.0f };
    CVector2 Velocity{ 0.0f, 0.0f };
    CVector3 Colour{ 0.0f, 0.0f, 0.0f };
    float Radius{ 1.0f };
    int HP{ 100 };
    std::string Name{ "" };
    QuadTree::AABB Bounds;

    Circle() = default;
};

struct Sphere
{
    CVector3 Position{ 0.0f, 0.0f, 0.0f };
    CVector3 Velocity{ 0.0f, 0.0f, 0.0f };
    CVector3 Colour{ 0.0f, 0.0f, 0.0f };
    float Radius{ 1.0f };
    int HP{ 100 };
    std::string Name{ "" };
    Sphere* NextCircle;

    Sphere() = default;
};