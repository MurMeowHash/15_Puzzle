#pragma once
#include <QList>
#include "BoardModel.h"
#include "Tile.h"
#include "MoveTileScoreOption.h"
#include "utils.h"

class BoardView : public QObject {
    Q_OBJECT
private:
    static const QString SPRITES_PATH;
    static const QString TILE_PREFIX;
    static const QString SPRITE_EXTENSION;
    QList<Tile *> tilesArray;
    Tile *currentFocusTile;
    static QString formTilePath(int num);
    static void setUpInitialCoords(qreal &currentXCoord, qreal &currentYCoord, const QPoint &relativePoint, int boardLength);
public:
    static constexpr qreal TILE_WIDTH = 128.;
    static constexpr qreal TILE_HEIGHT = 128.;
    static constexpr qreal PADDING = 8.;
    explicit BoardView(QObject *parent);
    void requestTileMove(Tile *currentFocus);
public slots:
    void drawTiles(const BoardModel &sourceBoard);
    void resetTiles(const BoardModel &sourceBoard);
    void onUpdateBoard(const Point &freePosition);
    void setFocus(const Point &targetPosition);
signals:
    void requestMove(const Point &targetPosition, const MoveTileScoreOption &scoreOption);
    void boardDrawn(const QPointF &leftBorderCoords, const QPointF &rightBorderCoords);
};