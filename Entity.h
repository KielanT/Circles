#pragma once

#include <string>
#include "CVector3.h"
#include "CVector2.h"

struct AABB
{
    CVector2 Centre;
    float HalfDimension;

    AABB(CVector2 centre, float halfDimension) : Centre(centre), HalfDimension(halfDimension) {}
    AABB() = default;

    bool Contains(CVector2 point)
    {
        return (point.x >= Centre.x - HalfDimension &&
            point.x <= Centre.x + HalfDimension &&
            point.y >= Centre.y - HalfDimension &&
            point.y <= Centre.y + HalfDimension);
    }


    bool Intersects(AABB other)
    {
        return (abs(Centre.x - other.Centre.x) < HalfDimension + other.HalfDimension &&
            abs(Centre.y - other.Centre.y) < HalfDimension + other.HalfDimension);
    }
};

struct Circle
{
    CVector2 Position{ 0.0f, 0.0f };
    CVector2 Velocity{ 0.0f, 0.0f };
    CVector3 Colour{ 0.0f, 0.0f, 0.0f };
    float Radius{ 1.0f };
    int HP{ 100 };
    std::string Name{ "" };
    AABB Bounds;

    Circle() = default;
};