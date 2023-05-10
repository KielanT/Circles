#pragma once

#include <string>
#include "CVector3.h"
#include "CVector2.h"
#include "Rectangle.h"

struct Circle
{
    CVector2 Position{ 0.0f, 0.0f };
    CVector2 Velocity{ 0.0f, 0.0f };
    CVector3 Colour{ 0.0f, 0.0f, 0.0f };
    float Radius{ 1.0f };
    int HP{ 100 };
    std::string Name{ "" };
    Rectangle Bounds;

    Circle() = default;
};