#pragma once
#include <QElapsedTimer>
#include "BoardNode.h"

class SolutionModel {
private:
    static constexpr int FOUND_MARKER = -1;
    static constexpr int INFINITY_MARKER = INT_MAX;
    static constexpr int NOT_FOUND_MARKER = -100;
    std::list<BoardNode*> solutionPath;
    std::list<BoardNode*>::iterator solutionHead;
    int currentStep;
    void fillSolutionPath(BoardNode *lastNode);
    void fillSolutionPath(QStack<BoardNode *> &targetStack);
    int idDfs(QStack<BoardNode *> &currentPath, const BoardModel &goalState, int upperBound,
              QElapsedTimer &currentTimer, int travelCost = 0);
    static void clearNodes(std::set<BoardNode*, BoardNodeComparator> &graph);
    static void clearNodes(QList<BoardNode *> &nodes, BoardNode *exclusiveNode);
    void resetStep();
public:
    SolutionModel();
    bool aStar(const BoardModel &currentState, const BoardModel &goalState);
    bool idaStar(const BoardModel &currentState, const BoardModel &goalState);
    void clearPath();
    NODISCARD bool lastMove() const;
    BoardModel *getNextMove();
    ~SolutionModel();
};