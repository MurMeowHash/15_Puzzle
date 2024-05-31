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

class GameController : public AbstractController {
    Q_OBJECT
private:
    BoardModel *gameBoard{nullptr};
    BoardModel *goalBoard{nullptr};
    SolutionModel *solver{nullptr};
    QFutureWatcher<bool> *asyncLoadingHandler{nullptr};
    QFuture<bool> *asyncResult{nullptr};
    std::list<QMetaObject::Connection> *connections{nullptr};
    void setUpConnections() override;
    void destroyConnections() override;
public:
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
    void updateBoard(const Point &freePos) const;
    void updateFocusInfo(const Point &targetTilePos) const;
    void solvingStarted() const;
    void showResultsPopup(const QString &context) const;
    void redrawBoard(const BoardModel &sourceBoard);
    void processingRestart();
    void solutionFound();
};