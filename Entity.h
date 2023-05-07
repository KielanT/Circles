#pragma once

#include <string>
#include "CVector3.h"

struct Circle
{
    CVector3 Position;
    CVector3 Velocity;
    CVector3 Colour;
    float Radius;
    int HP = 100;
    std::string Name;
};