#include "Algorithms.h"

int Algorithms::calculateManhattan(const Point &currentPos, const Point &targetPos) {
    return abs(targetPos.x - currentPos.x) + abs(targetPos.y - currentPos.y);
}