#include "BoardNodeComparator.h"
#include "NullReferenceException.h"

bool BoardNodeComparator::operator()(const BoardNode *firstNode, const BoardNode *secondNode) const {
    handleNullReferences("Comparison nodes can not be null", 2, firstNode, secondNode);
    BoardModel *firstBoard = firstNode->getBoard();
    BoardModel *secondBoard = secondNode->getBoard();
    handleNullReferences("Comparison boards can not be null", 2, firstNode, secondNode);
    return (*firstBoard) < (*secondBoard);
}