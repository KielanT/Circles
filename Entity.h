#pragma once

#include <string>
#include "CVector3.h"
#include "CVector2.h"

struct Circle
{
    CVector2 Position;
    CVector2 Velocity;
    CVector3 Colour;
    float Radius;
    int HP = 100;
    std::string Name;
};