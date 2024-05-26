#include "Point.h"

bool Point::operator==(const Point &targetPoint) const {
    return x == targetPoint.x && y == targetPoint.y;
}

bool Point::operator!=(const Point &targetPoint) const {
    return !(*this == targetPoint);
}