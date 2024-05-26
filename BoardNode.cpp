#include "BoardNode.h"
#include "Algorithms.h"
#include "SearchException.h"
#include "BoardNodeComparator.h"

BoardNode::BoardNode(BoardNode *parent, BoardModel *board, int gValue)
: parent{parent}, board{board}, gValue{gValue}, hValue{0}, fValue{0}, pathNode{false} {
    calculateHValue();
    calculateFValue();
}

void BoardNode::setGValue(int gValue) {
    this->gValue = gValue;
    calculateFValue();
}

int BoardNode::getGValue() const {
    return gValue;
}

void BoardNode::calculateHValue() {
    int boardLength = board->getLength();
    Point currentPos;
    for(int i = 0; i < boardLength; i++) {
        for(int j = 0; j < boardLength; j++) {
            int currentElement = (*board)[i].at(j);
            if(currentElement != BoardModel::EMPTY_SPACE) {
                Point targetPos;
                currentPos.x = i;
                currentPos.y = j;
                Algorithms::transformToMatrixIndex(currentElement - 1, targetPos,boardLength);
                hValue += Algorithms::calculateManhattan(currentPos, targetPos);
            }
        }
    }
}

void BoardNode::calculateFValue() {
    fValue = gValue + hValue;
}

int BoardNode::getHeuristicValue() const {
    return fValue;
}

BoardNode *BoardNode::getParent() const {
    return parent;
}

BoardModel *BoardNode::getBoard() const {
    return board;
}

QList<BoardNode *> BoardNode::convertToNodes(const QList<BoardModel *> &targetModels, BoardNode *parent, int gValue) {
    int modelsLength = static_cast<int>(targetModels.length());
    QList<BoardNode *> nodes(modelsLength);
    for(int i = 0; i < modelsLength; i++) {
        nodes[i] = new BoardNode(parent, targetModels.at(i), gValue);
    }
    return nodes;
}

void BoardNode::openNeighbors(std::set<BoardNode*, BoardNodeComparator> &graph,
                              std::set<BoardNode*, BoardNodeComparator> &openList) {
    handleNullReferences("board reference is null", 1, board);
    QList<BoardModel *> possibleBoards = board->makePossibleMoves();
    int costToMove = gValue + 1;
    QList<BoardNode *> possibleNodes = convertToNodes(possibleBoards, this, costToMove);
    for(BoardNode *currentNode : possibleNodes) {
        auto graphNode = graph.find(currentNode);
        if(graphNode == graph.end()) {
            graph.insert(currentNode);
            openList.insert(currentNode);
        } else {
            if(costToMove < (*graphNode)->gValue) {
                (*graphNode)->setGValue(costToMove);
                (*graphNode)->setParent(this);
                auto openListNode = openList.find(*graphNode);
                if(openListNode == openList.end()) {
                    openList.insert(*graphNode);
                }
            }
            delete currentNode;
        }
    }
}

QList<BoardNode *> BoardNode::openNeighbors() {
    handleNullReferences("board reference is null", 1, board);
    QList<BoardModel *> possibleBoards = board->makePossibleMoves(parent == nullptr ? nullptr : parent->board);
    int costToMove = gValue + 1;
    return convertToNodes(possibleBoards, this, costToMove);
}

void BoardNode::setParent(BoardNode *parent) {
    this->parent = parent;
}

void BoardNode::setPathNode() {
    pathNode = true;
}

bool BoardNode::isPathNode() const {
    return pathNode;
}

BoardNode::~BoardNode() {
    delete board;
}