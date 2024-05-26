#pragma once
#include "BoardNode.h"

struct BoardNodeComparator {
    bool operator()(const BoardNode *firstNode, const BoardNode *secondNode) const;
};