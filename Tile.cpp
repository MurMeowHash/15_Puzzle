#include "Tile.h"
#include "BoardView.h"

Tile::Tile(const QPixmap &pixmap, const Point &currentPosition, BoardView *parent, int identifier)
: QGraphicsPixmapItem{pixmap}, QObject{parent}, position{currentPosition}, parent{parent}, identifier{identifier} {

}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(parent != nullptr) {
        parent->requestTileMove(this);
    }
}

void Tile::moveTile(const Point &freePos) {
    Point direction = {freePos.x - position.x, freePos.y - position.y};
    setPos(x() + (BoardView::TILE_WIDTH + BoardView::PADDING) * direction.y, y() +
    (BoardView::TILE_HEIGHT + BoardView::PADDING) * direction.x);
    position = freePos;
}

Point Tile::getPosition() const {
    return position;
}

int Tile::getIdentifier() const {
    return identifier;
}

void Tile::updatePos(const Point &target) {
    position = target;
}