#include "Entity.h"

bool QuadTree::Contains(AABB bound, CVector2 point)
{
    return (point.x >= bound.Centre.x - bound.HalfDimension &&
        point.x <= bound.Centre.x + bound.HalfDimension &&
        point.y >= bound.Centre.y - bound.HalfDimension &&
        point.y <= bound.Centre.y + bound.HalfDimension);
}

bool QuadTree::Intersects(AABB bound, AABB other)
{
    return (abs(bound.Centre.x - other.Centre.x) < bound.HalfDimension + other.HalfDimension &&
        abs(bound.Centre.y - other.Centre.y) < bound.HalfDimension + other.HalfDimension);
}
