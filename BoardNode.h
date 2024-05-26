#pragma once
#include <list>
#include <set>
#include "BoardModel.h"

class BoardNodeComparator;

class BoardNode {
private:
    BoardNode *parent;
    BoardModel *board;
    int gValue;
    int fValue;
    int hValue;
    bool pathNode;
    void calculateHValue();
    void calculateFValue();
public:
    BoardNode(BoardNode *parent, BoardModel *board, int gValue = 0);
    void setGValue(int gValue);
    void setParent(BoardNode *parent);
    void setPathNode();
    NODISCARD bool isPathNode() const;
    NODISCARD int getGValue() const;
    NODISCARD int getHeuristicValue() const;
    NODISCARD BoardNode *getParent() const;
    NODISCARD BoardModel *getBoard() const;
    static QList<BoardNode *> convertToNodes(const QList<BoardModel *> &targetModels, BoardNode *parent, int gValue);
    void openNeighbors(std::set<BoardNode*, BoardNodeComparator> &graph,
                       std::set<BoardNode*, BoardNodeComparator> &openList);
    QList<BoardNode *> openNeighbors();
    ~BoardNode();
};