#pragma once
#include <QList>
#include "BoardModel.h"
#include "Tile.h"
#include "MoveTileScoreOption.h"

class BoardView : public QObject {
    Q_OBJECT
private:
    static const QString SPRITES_PATH;
    static const QString TILE_PREFIX;
    static const QString SPRITE_EXTENSION;
    QList<Tile *> tilesArray;
    Tile *currentFocusTile;
    static QString formTilePath(int num);
    static void setUpInitialCoords(qreal &currentXCoord, qreal &currentYCoord,
                                   const std::shared_ptr<QGraphicsScene> &currentScene, int boardLength);
public:
    static constexpr qreal TILE_WIDTH = 64.;
    static constexpr qreal TILE_HEIGHT = 64.;
    static constexpr qreal PADDING = 8.;
    explicit BoardView(QObject *parent);
    void requestTileMove(const Point &targetPosition, Tile *currentFocus);
public slots:
    void drawTiles(const BoardModel &sourceBoard);
    void resetTiles(const BoardModel &sourceBoard);
    void onUpdateBoard(const Point &targetPosition, const Point &freePosition, int boardLength);
    void setFocus(const Point &targetPosition);
signals:
    void requestMove(const Point &targetPosition, const MoveTileScoreOption &scoreOption);
};