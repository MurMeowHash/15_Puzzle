#include <QPushButton>
#include <QtConcurrent/QtConcurrent>
#include <QFile>
#include <QTextStream>
#include "GameController.h"
#include "ManagersSystem/Managers.h"
#include "Tile.h"
#include "GraphicsWidget.h"
#include "FileException.h"

const QString GameController::LOAD_STATE_FLAG{"-l"};

GameController::GameController() : gameBoard{nullptr}, goalBoard{nullptr}, solver{nullptr}, asyncLoadingHandler{nullptr},
    asyncResult{nullptr}, connections{nullptr} {

}

void GameController::initializeController(const QString &flag) {
    Managers::getScene()->changeScene(Scenes::Game);
    BoardModel *savedBoard{nullptr};
    if(flag == LOAD_STATE_FLAG) {
        try {
            savedBoard = SavingsManager::getSavedState();
        } catch(FileException &ex) {
//            TODO: message box
            std::cout<<ex.what();
        }
    }
    gameBoard = savedBoard == nullptr ? new BoardModel{PUZZLE_TABLE_LENGTH} : savedBoard;
    solver = new SolutionModel;
    goalBoard = new BoardModel{PUZZLE_TABLE_LENGTH, BoardGenerationOption::Goal};
    Managers::getProgress()->setProgress(gameBoard->countRightPlacedTiles());
    asyncResult = new QFuture<bool>;
    connections = new std::list<QMetaObject::Connection>(0);
    asyncLoadingHandler = new QFutureWatcher<bool>{this};
    setUpConnections();
    gameBoardSetUp(*gameBoard);
}

void GameController::moveTile(const Point &targetPosition, const MoveTileScoreOption &scoreOption) {
    if(scoreOption == MoveTileScoreOption::Scored
    && Managers::getProgress()->getSolverUsage()) {
        return;
    }
    handleNullReferences("To move tile board should be provided", 1, gameBoard);
    Point initialFreePos = gameBoard->getFreePos();
    bool tileMoved{false};
    switch (scoreOption) {
        case MoveTileScoreOption::Default:
            tileMoved = gameBoard->move(targetPosition);
            break;
        case MoveTileScoreOption::Scored:
            tileMoved = gameBoard->moveScored(targetPosition);
            break;
    }
    if(tileMoved) {
        if(Managers::getProgress()->increaseMovesCount() == 1) {
            Managers::getProgress()->startTrackingTime();
        }
        emit updateBoard(targetPosition, initialFreePos, gameBoard->getLength());
    }
}

void GameController::solveState() {
    handleNullReferences("To solve state board, goal board, solver and async result should be provided",
                         4, gameBoard, goalBoard, solver, asyncResult);
    solver->clearPath();
    switch (Managers::getPreferences()->getResourcesDemand()) {
        case ResourcesOption::HighDemand:
            *asyncResult = QtConcurrent::run([=](){
                return solver->aStar(*gameBoard, *goalBoard);
            });
            break;
        case ResourcesOption::LowDemand:
            *asyncResult = QtConcurrent::run([=](){
                return solver->idaStar(*gameBoard, *goalBoard);
            });
            break;
    }
    asyncLoadingHandler->setFuture(*asyncResult);
    emit solvingStarted();
    Managers::getProgress()->enableSolverUsage(true);
}

void GameController::setUpConnections() {
    std::shared_ptr<GameSceneView> gameScene = Managers::getScene()->getGameScene();
    BoardView *boardView = gameScene->getBoardView();
    GraphicsWidget<QPushButton> *solveButton = gameScene->getSolveButton();
    GraphicsWidget<QPushButton> *proceedButton = gameScene->getProceedButton();
    GraphicsWidget<QPushButton> *restartButton = gameScene->getRestartButton();
    GraphicsWidget<QPushButton> *saveButton = gameScene->getSaveButton();
    GraphicsWidget<QPushButton> *menuButton = gameScene->getMenuButton();
    connections->push_back(connect(this, &GameController::gameBoardSetUp, boardView,
                                  &BoardView::drawTiles));
    connections->push_back(connect(boardView, &BoardView::requestMove, this,
                                  &GameController::moveTile));
    connections->push_back(connect(this, &GameController::updateBoard, boardView,
                                  &BoardView::onUpdateBoard));
    connections->push_back(connect(solveButton->widgetPart, &QPushButton::clicked, this,
                                  &GameController::solveState));
    connections->push_back(connect(proceedButton->widgetPart, &QPushButton::clicked, this,
                                  &GameController::showNextMove));
    connections->push_back(connect(this, &GameController::updateFocusInfo, boardView,
                                  &BoardView::setFocus));
    connections->push_back(connect(this, &GameController::solvingStarted, gameScene.get(),
                                  &GameSceneView::enableLoadingScreen));
    connections->push_back(connect(asyncLoadingHandler, &QFutureWatcher<bool>::finished, gameScene.get(),
                                  &GameSceneView::disableLoadingScreen));
    connections->push_back(connect(asyncLoadingHandler, &QFutureWatcher<bool>::finished, this,
                                  &GameController::handleSolvingResult));
    connections->push_back(connect(this, &GameController::showResultsPopup, gameScene.get(),
                                  &GameSceneView::drawResultsPopup));
    connections->push_back(connect(this, &GameController::redrawBoard, boardView,
                                  &BoardView::resetTiles));
    connections->push_back(connect(restartButton->widgetPart, &QPushButton::clicked, this,
                                  &GameController::restartGame));
    connections->push_back(connect(this, &GameController::processingRestart, gameScene.get(),
                                  &GameSceneView::hideWinPopup));
    connections->push_back(connect(saveButton->widgetPart, &QPushButton::clicked, this,
                                  &GameController::saveGame));
    connections->push_back(connect(menuButton->widgetPart, &QPushButton::clicked, this,
                                  &GameController::returnToMenu));
}

void GameController::destroyConnections() {
    handleNullReferences("No connections to destroy", 1, connections);
    for(auto &connection : *connections) {
        disconnect(connection);
    }
    connections->clear();
}

void GameController::showNextMove() {
    handleNullReferences("To show next state solver should be provided", 1, solver);
    BoardModel *nextBoard = solver->getNextMove();
    if(nextBoard != nullptr) {
        Point targetPos = nextBoard->getFreePos();
        emit updateFocusInfo(targetPos);
        moveTile(targetPos);
        if(solver->lastMove()) {
            emit Managers::getProgress()->gameWin();
        }
    }
}

void GameController::handleSolvingResult() {
    handleNullReferences("No result to handle", 1, asyncResult);
    QString context;
    if(asyncResult->result()) {
        context = "Solution was successfully found";
    } else {
        context = "Solution was not found";
        Managers::getProgress()->enableSolverUsage(false);
    }
    emit showResultsPopup(context);
}

void GameController::restartGame() {
    handleNullReferences("No game to restart", 2, gameBoard, solver);
    gameBoard->shuffle();
    emit redrawBoard(*gameBoard);
    Managers::getProgress()->wipeProgress();
    solver->clearPath();
    emit processingRestart();
}

void GameController::saveGame() {
    handleNullReferences("No game to save", 1, gameBoard);
    SavingsManager::saveState(*gameBoard);
}

void GameController::dispose() {
    tryDestroyConnections(connections)
    tryDispose(gameBoard)
    tryDispose(goalBoard)
    tryDispose(solver)
    tryDispose(asyncResult)
    tryDispose(asyncLoadingHandler)
    tryDispose(connections)
    Managers::getProgress()->wipeProgress();
}

void GameController::returnToMenu() {
    emit passControl(this, nullptr);
}