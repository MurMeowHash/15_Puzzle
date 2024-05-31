#pragma once
#include <QList>
#include <QObject>
#include "IGenericArray.h"
#include "Point.h"
#include "BoardGenerationOption.h"
#include "utils.h"

class BoardModel : public IGenericArray<int> {
private:
    static constexpr int TILE_START_POSITION = 1;
    static constexpr int FIRST_EVEN_ROW = -1;
    static constexpr int FIRST_ODD_ROW = -2;
    QList<QList<int>> board;
    int length;
    Point freePos;
    void allocateColumns();
    void generateTable();
    NODISCARD QList<int> generatePossibleRows(int countInversions) const;
    void insertEmptyCell();
    void updateFreePosition(const Point &position);
    NODISCARD bool isRightPlaced(const Point &targetPosition) const;
    int& operator[](const Point &position);
public:
    static constexpr int EMPTY_SPACE = 0;
    explicit BoardModel(int length, const BoardGenerationOption &genOption = BoardGenerationOption::Ordinary);
    explicit BoardModel(const QList<QList<int>> &refBoard);
    NODISCARD bool canMove(const Point &targetPos) const;
    bool move(const Point &targetPos);
    bool moveScored(const Point &targetPos);
    NODISCARD Point getFreePos() const;
    NODISCARD bool equals(const BoardModel &targetBoard) const;
    NODISCARD int getLength() const;
    QList<BoardModel *> makePossibleMoves(BoardModel *exclusiveBoard = nullptr);
    void shuffle();
    void toGoalBoard();
    void syncFreePos();
    UNUSED void print() const;
    NODISCARD int countRightPlacedTiles() const;
    int &elementAt(int arrayIndex) override;
    NODISCARD const int &readElement(int arrayIndex) const override;
    NODISCARD int getElementsCount() const override;
    NODISCARD IGenericArray<int> *copy() const override;
    const QList<int> &operator[](int index) const;
    bool operator<(const BoardModel &targetModel) const;
    int operator[](const Point &position) const;
};