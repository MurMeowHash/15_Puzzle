#include <iostream>
#include "BoardModel.h"
#include "Algorithms.h"
#include "ManagersSystem/Managers.h"

BoardModel::BoardModel(int length, const BoardGenerationOption &genOption) : board(length), length{length} {
    allocateColumns();
    switch (genOption) {
        case BoardGenerationOption::Ordinary:
            shuffle();
            break;
        case BoardGenerationOption::Goal:
            toGoalBoard();
            break;
    }
}

BoardModel::BoardModel(const QList<QList<int>> &refBoard) : board{refBoard} {
    length = static_cast<int>(refBoard.size());
    syncFreePos();
    //TODO: check for invalid board?
}

void BoardModel::allocateColumns() {
    for(auto &row : board) {
        row.resize(length);
    }
}

void BoardModel::generateTable() {
    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length; j++) {
            board[i][j] = i * length + j;
        }
    }
}

QList<int> BoardModel::generatePossibleRows(int countInversions) const {
    QList<int> possibleRows(length / 2);
    int firstRow, step = 2;
    if(countInversions % 2 == 0) {
        firstRow = FIRST_EVEN_ROW;
    } else {
        firstRow = FIRST_ODD_ROW;
    }
    std::generate(possibleRows.begin(), possibleRows.end(), [&firstRow, &step] {return firstRow += step;});
    return possibleRows;
}

void BoardModel::updateFreePosition(const Point &position) {
    freePos = position;
}

void BoardModel::insertEmptyCell() {
    Point randomPosition;
    int countInversions = Algorithms::getInversionsCount(*this, TILE_START_POSITION,
                                                         getElementsCount() - 1);
    randomPosition.y = QRandomGenerator::global()->bounded(0, length);
    auto possibleRows = generatePossibleRows(countInversions);
    randomPosition.x = Algorithms::pickRandom(possibleRows);
    int approachingPosition = TILE_START_POSITION - 1;
    int targetPosition = Algorithms::transformToArrayIndex(randomPosition,length);
    Algorithms::approachElement(*this, approachingPosition, targetPosition);
    updateFreePosition(randomPosition);
}

bool BoardModel::move(const Point &targetPos) {
    if(!canMove(targetPos)) {
        return false;
    }
    std::swap((*this)[targetPos], (*this)[freePos]);
    updateFreePosition(targetPos);
    return true;
}

bool BoardModel::moveScored(const Point &targetPos) {
    if(!canMove(targetPos)) {
        return false;
    }
    if(isRightPlaced(targetPos)) {
        Managers::getProgress()->decreaseProgress();
    }
    std::swap((*this)[targetPos], (*this)[freePos]);
    if(isRightPlaced(freePos)) {
        Managers::getProgress()->increaseProgress();
    }
    updateFreePosition(targetPos);
    return true;
}

int BoardModel::getLength() const {
    return length;
}

void BoardModel::print() const {
    std::cout<<"\n";
    for(const auto &row : board) {
        for(auto el : row) {
            std::cout<<el<<" ";
        }
        std::cout<<std::endl;
    }
}

int &BoardModel::elementAt(int arrayIndex) {
    Point position;
    Algorithms::transformToMatrixIndex(arrayIndex, position, length);
    return (*this)[position];
}

int BoardModel::getElementsCount() const {
    return length * length;
}

IGenericArray<int> *BoardModel::copy() const {
    return new BoardModel{*this};
}

bool BoardModel::equals(const BoardModel &targetBoard) const {
    if(length != targetBoard.length) {
        return false;
    }
    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length; j++) {
            if(board[i][j] != targetBoard.board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

Point BoardModel::getFreePos() const {
    return freePos;
}

bool BoardModel::canMove(const Point &targetPos) const {
    bool neighbours = (abs(targetPos.x - freePos.x) + abs(targetPos.y - freePos.y)) == 1;
    return neighbours && targetPos.x >= 0 && targetPos.x < length && targetPos.y >= 0 && targetPos.y < length;
}

int &BoardModel::operator[](const Point &position) {
    return board[position.x][position.y];
}

int BoardModel::operator[](const Point &position) const {
    return board.at(position.x).at(position.y);
}

const QList<int> &BoardModel::operator[](int index) const {
    return board.at(index);
}

QList<BoardModel *> BoardModel::makePossibleMoves(BoardModel *exclusiveBoard) {
    QList<BoardModel *> possibleConfigurations;
    Point possibleMoves[] = {Point{freePos.x, freePos.y - 1}, Point{freePos.x - 1, freePos.y},
                             Point{freePos.x, freePos.y + 1}, Point{freePos.x + 1, freePos.y}};
    int possibleMovesLength = sizeof(possibleMoves) / sizeof(possibleMoves[0]);
    BoardModel *currentBoard;
    for(int i = 0; i < possibleMovesLength; i++) {
        if(exclusiveBoard != nullptr) {
            if(exclusiveBoard->freePos == possibleMoves[i]) {
                continue;
            }
        }
        if(canMove(possibleMoves[i])) {
            currentBoard = new BoardModel{*this};
            currentBoard->move(possibleMoves[i]);
            possibleConfigurations.push_back(currentBoard);
        }
    }
    return possibleConfigurations;
}

void BoardModel::temporary() {
    board[0][0] = 1;
    board[0][1] = 3;
    board[0][2] = 14;
    board[0][3] = 5;
    board[1][0] = 12;
    board[1][1] = 9;
    board[1][2] = 4;
    board[1][3] = 2;
    board[2][0] = 15;
    board[2][1] = 7;
    board[2][2] = 13;
    board[2][3] = 8;
    board[3][0] = 0;
    board[3][1] = 10;
    board[3][2] = 11;
    board[3][3] = 6;
//    for(int i = 0; i < length; i++) {
//        for(int j = 0; j < length; j++) {
//            board[i][j] = i * length + j + 1;
//        }
//    }
//    board[3][3] = 15;
//    board[3][2] = 0;
    freePos.x = 3;
    freePos.y = 0;
}

bool BoardModel::operator<(const BoardModel &targetModel) const {
    return board < targetModel.board;
}

bool BoardModel::isRightPlaced(const Point &targetPosition) const {
    return Algorithms::transformToArrayIndex(targetPosition,
                                             length) + 1 == board.at(targetPosition.x).at(targetPosition.y);
}

int BoardModel::countRightPlacedTiles() const {
    int count{0};
    Point currentPoint;
    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length; j++) {
            currentPoint.x = i;
            currentPoint.y = j;
            if(isRightPlaced(currentPoint)) {
                count++;
            }
        }
    }
    return count;
}

void BoardModel::toGoalBoard() {
    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length; j++) {
            board[i][j] = i * length + j + 1;
        }
    }
    int lastIndex = length - 1;
    board[lastIndex][lastIndex] = 0;
    updateFreePosition(Point{lastIndex, lastIndex});
}

//TODO: possible remove
const int &BoardModel::readElement(int arrayIndex) const {
    Point position;
    Algorithms::transformToMatrixIndex(arrayIndex, position, length);
    return board[position.x][position.y];
}

void BoardModel::shuffle() {
    generateTable();
    Algorithms::randomSort(*this, TILE_START_POSITION, length * length - 1);
    insertEmptyCell();
}

void BoardModel::syncFreePos() {
    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length; j++) {
            if(board.at(i).at(j) == EMPTY_SPACE) {
                freePos = {i, j};
            }
        }
    }
}