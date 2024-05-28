#include "BoardView.h"
#include "Algorithms.h"
#include "ManagersSystem/Managers.h"

const QString BoardView::SPRITES_PATH{"./sprites/"};
const QString BoardView::TILE_PREFIX{"Puzzle_"};
const QString BoardView::SPRITE_EXTENSION{".png"};

BoardView::BoardView(QObject *parent) : QObject{parent}, currentFocusTile{nullptr} {

}

QString BoardView::formTilePath(int num) {
    return SPRITES_PATH + TILE_PREFIX + QString::number(num) + SPRITE_EXTENSION;
}

void BoardView::setUpInitialCoords(qreal &currentXCoord, qreal &currentYCoord, const QPoint &relativePoint, int boardLength) {
    currentXCoord = relativePoint.x() - boardLength / 2. * TILE_WIDTH - (boardLength - 1.) / 2. * PADDING;
    currentYCoord = relativePoint.y() - boardLength / 2. * TILE_HEIGHT - (boardLength - 1.) / 2. * PADDING;
}

void BoardView::drawTiles(const BoardModel &sourceBoard) {
    int boardLength = sourceBoard.getLength();
    int elementsCount = sourceBoard.getElementsCount();
    tilesArray.resize(elementsCount - 1);
    Point currentPos;
    std::shared_ptr<QGraphicsScene> gameScene = Managers::getScene()->getGameScene();
    qreal currentXCoord, currentYCoord, initialXCoord;
    setUpInitialCoords(currentXCoord, currentYCoord, Scene::COORDINATES_CENTER, boardLength);
    initialXCoord = currentXCoord;
    QPointF leftBorderCoords, rightBorderCoords;
    for(int i = 0, currentTileIndex = 0; i < elementsCount; i++) {
        if(i % boardLength == 0
        && i != 0) {
            currentXCoord = initialXCoord;
            currentYCoord += TILE_HEIGHT + PADDING;
        }
        if(i == 0) {
            leftBorderCoords = {currentXCoord, currentYCoord};
        } else if(i == elementsCount - 1) {
            rightBorderCoords = {currentXCoord + TILE_WIDTH, currentYCoord};
        }
        Algorithms::transformToMatrixIndex(i, currentPos, boardLength);
        int currentElement = sourceBoard[currentPos.x][currentPos.y];
        if(currentElement != BoardModel::EMPTY_SPACE) {
            QString tilePath = formTilePath(currentElement);
            tilesArray[currentTileIndex] = new Tile{QPixmap{tilePath}.scaled(TILE_WIDTH, TILE_HEIGHT),
                                     currentPos, this, currentElement};
            tilesArray[currentTileIndex]->setPos(currentXCoord, currentYCoord);
            gameScene->addItem(tilesArray[currentTileIndex]);
            currentTileIndex++;
        }
        currentXCoord += TILE_WIDTH + PADDING;
    }
    emit boardDrawn(leftBorderCoords, rightBorderCoords);
}

void BoardView::setFocus(const Point &targetPosition) {
    for(Tile *tile : tilesArray) {
        if(tile->getPosition() == targetPosition) {
            currentFocusTile = tile;
        }
    }
}

void BoardView::requestTileMove(const Point &targetPosition, Tile *currentFocus) {
    currentFocusTile = currentFocus;
    emit requestMove(targetPosition, MoveTileScoreOption::Scored);
}

void BoardView::onUpdateBoard(const Point &targetPosition, const Point &freePosition, int boardLength) {
    if(currentFocusTile != nullptr) {
        currentFocusTile->moveTile(freePosition);
    }
}

void BoardView::resetTiles(const BoardModel &sourceBoard) {
    int boardLength = sourceBoard.getLength();
    qreal currentXCoord, currentYCoord, initialXCoord;
    setUpInitialCoords(currentXCoord, currentYCoord, Scene::COORDINATES_CENTER, boardLength);
    initialXCoord = currentXCoord;
    for(int i = 0; i < boardLength; i++) {
        for(int j = 0; j < boardLength; j++) {
            int currentElement = sourceBoard[i][j];
            auto currentTile = std::find_if(tilesArray.begin(), tilesArray.end(),
                                                  [currentElement](Tile *currentTile){
                return currentTile->getIdentifier() == currentElement;
            });
            if(currentTile != tilesArray.end()) {
                (*currentTile)->setPos(currentXCoord, currentYCoord);
                (*currentTile)->updatePos(Point{i, j});
            }
            currentXCoord += TILE_WIDTH + PADDING;
        }
        currentYCoord += TILE_HEIGHT + PADDING;
        currentXCoord = initialXCoord;
    }
}