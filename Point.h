#pragma once

struct Point {
    int x;
    int y;
    bool operator==(const Point &targetPoint) const;
    bool operator!=(const Point &targetPoint) const;
};