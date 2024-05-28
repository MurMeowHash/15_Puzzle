#pragma once
#include <QObject>
#include <QFutureWatcher>
#include "AbstractController.h"
#include "IConnectable.h"
#include "SolutionModel.h"
#include "BoardModel.h"
#include "BoardView.h"
#include "GameSceneView.h"
#include "MoveTileScoreOption.h"

class GameController : public AbstractController, public IConnectable {
    Q_OBJECT
private:
    BoardModel *gameBoard;
    BoardModel *goalBoard;
    SolutionModel *solver;
    QFutureWatcher<bool> *asyncLoadingHandler;
    QFuture<bool> *asyncResult;
    std::list<QMetaObject::Connection> *connections;
    void setUpConnections() override;
    void destroyConnections() override;
public:
    GameController();
    static constexpr int PUZZLE_TABLE_LENGTH = 4;
    static const QString LOAD_STATE_FLAG;
    void dispose() override;
public slots:
    void moveTile(const Point &targetPosition, const MoveTileScoreOption &scoreOption = MoveTileScoreOption::Default);
    void solveState();
    void showNextMove();
    void handleSolvingResult();
    void restartGame();
    void saveGame();
    void returnToMenu();
    void connectControlButtons();
    void initializeController(const QString &flag) override;
    signals:
    void gameBoardSetUp(const BoardModel &referenceBoard) const;
    void updateBoard(const Point &targetPos, const Point &freePos, int boardLength) const;
    void updateFocusInfo(const Point &targetTilePos) const;
    void solvingStarted() const;
    void showResultsPopup(const QString &context) const;
    void redrawBoard(const BoardModel &sourceBoard);
    void processingRestart();
    void solutionFound();
};