#pragma once
#include <QGraphicsPixmapItem>
#include "Point.h"
#include "utils.h"

class BoardView;

class Tile : public QObject, public QGraphicsPixmapItem {
private:
    int identifier;
    Point position;
    BoardView *parent;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public:
    Tile(const QPixmap &pixmap, const Point &currentPosition, BoardView *parent, int identifier);
    void moveTile(const Point &freePos);
    void updatePos(const Point &target);
    NODISCARD Point getPosition() const;
    NODISCARD int getIdentifier() const;
};