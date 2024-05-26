#include <list>
#include <set>
#include <QStack>
#include "BoardNodeComparator.h"
#include "SolutionModel.h"
#include "SearchException.h"
#include "ManagersSystem/Managers.h"
#include <iostream>

SolutionModel::SolutionModel() : currentStep{0} {
    solutionHead = solutionPath.begin();
}

bool SolutionModel::aStar(const BoardModel &currentState, const BoardModel &goalState) {
    auto currentNode = new BoardNode{nullptr, new BoardModel{currentState}, 0};
    std::set<BoardNode*, BoardNodeComparator> openList{currentNode};
    std::set<BoardNode *, BoardNodeComparator> graph{currentNode};
    bool timeHasExpired{false};
    QElapsedTimer algorithmTimer;
    algorithmTimer.start();
    qint64 targetStopTime = Managers::getPreferences()->getMaxSolutionTimeMSec();
    while(!openList.empty()
    && !currentNode->getBoard()->equals(goalState)
    && !timeHasExpired) {
        currentNode->openNeighbors(graph, openList);
        openList.extract(currentNode);
        auto currentIterator = std::min_element(openList.begin(), openList.end(),
                                         [](BoardNode *first, BoardNode *second){
            if(first == nullptr || second == nullptr) {
                return false;
            }
            return first->getHeuristicValue() < second->getHeuristicValue();
        });
        if(currentIterator == openList.end()) {
            throw SearchException("Route was not found", "one or more nodes are null");
        } else {
            currentNode = *currentIterator;
        }
        timeHasExpired = algorithmTimer.hasExpired(targetStopTime);
    }
    if(!timeHasExpired) {
        fillSolutionPath(currentNode);
    }
    clearNodes(graph);
    return !timeHasExpired;
}

void SolutionModel::fillSolutionPath(BoardNode *lastNode) {
    while(lastNode != nullptr) {
        solutionPath.push_front(lastNode);
        lastNode->setPathNode();
        lastNode = lastNode->getParent();
    }
}

void SolutionModel::clearNodes(std::set<BoardNode*, BoardNodeComparator> &graph) {
    for(auto element : graph) {
        if(!element->isPathNode()) {
            delete element;
        }
    }
}

void SolutionModel::clearNodes(QList<BoardNode *> &nodes, BoardNode *exclusiveNode) {
    for(BoardNode *node : nodes) {
        if(node != exclusiveNode && node != nullptr) {
            delete node;
        }
    }
}

int SolutionModel::idDfs(QStack<BoardNode *> &currentPath, const BoardModel &goalState, int upperBound,
                         QElapsedTimer &currentTimer, int travelCost) {
    BoardNode *currentNode = currentPath.top();
    currentNode->setGValue(travelCost);
    if(currentNode->getHeuristicValue() > upperBound) {
        return currentNode->getHeuristicValue();
    }
    if(currentNode->getBoard()->equals(goalState)) {
        return FOUND_MARKER;
    }
    if(currentTimer.hasExpired(Managers::getPreferences()->getMaxSolutionTimeMSec())) {
        return NOT_FOUND_MARKER;
    }
    QList<BoardNode *> neighbors = currentNode->openNeighbors();
    int minimumBound{INFINITY_MARKER};
    for(auto &neighbor : neighbors) {
        if(std::find_if(currentPath.begin(), currentPath.end(), [&neighbor](BoardNode *exploredNode){
            handleNullReferences("Explored nodes can not be null", 2, neighbor, exploredNode);
            handleNullReferences("Explored boards can not be null", 2, exploredNode->getBoard(), neighbor->getBoard());
            return neighbor->getBoard()->equals(*exploredNode->getBoard());
        }) == currentPath.end()) {
            currentPath.push_back(neighbor);
            int dfsResult = idDfs(currentPath, goalState, upperBound, currentTimer, travelCost + 1);
            if(dfsResult == FOUND_MARKER
            || dfsResult == NOT_FOUND_MARKER) {
                clearNodes(neighbors, neighbor);
                return dfsResult;
            }
            if(dfsResult < minimumBound) {
                minimumBound = dfsResult;
            }
            currentPath.pop_back();
        }
        delete neighbor;
        neighbor = nullptr;
    }
    return minimumBound;
}

bool SolutionModel::idaStar(const BoardModel &currentState, const BoardModel &goalState) {
    auto currentNode = new BoardNode{nullptr, new BoardModel{currentState}, 0};
    QStack<BoardNode *> currentPath;
    currentPath.push_back(currentNode);
    int upperBound = currentNode->getHeuristicValue();
    int dfsResult;
    QElapsedTimer algorithmTimer;
    algorithmTimer.start();
    do {
        dfsResult = idDfs(currentPath, goalState, upperBound, algorithmTimer);
        if(dfsResult == INFINITY_MARKER) {
            throw SearchException{"Path wasn't found", "Nodes might be in different components"};
        }
        upperBound = dfsResult;
    } while(dfsResult != FOUND_MARKER
    && dfsResult != NOT_FOUND_MARKER);
    if(dfsResult == FOUND_MARKER) {
        fillSolutionPath(currentPath);
    }
    return dfsResult == FOUND_MARKER;
}

void SolutionModel::fillSolutionPath(QStack<BoardNode *> &targetStack) {
    for(BoardNode *node : targetStack) {
        solutionPath.push_back(node);
    }
    resetStep();
}

void SolutionModel::clearPath() {
    for(auto &pathNode : solutionPath) {
        delete pathNode;
    }
    solutionPath.clear();
    resetStep();
}

BoardModel *SolutionModel::getNextMove() {
    solutionHead++;
    currentStep++;
    return currentStep < solutionPath.size() ? (*solutionHead)->getBoard() : nullptr;
}

bool SolutionModel::lastMove() const {
    return currentStep + 1 >= solutionPath.size();
}

void SolutionModel::resetStep() {
    solutionHead = solutionPath.begin();
    currentStep = 0;
}

SolutionModel::~SolutionModel() {
    clearPath();
}