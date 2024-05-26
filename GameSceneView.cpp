#include <QPushButton>
#include <QMovie>
#include "GameSceneView.h"
#include "ManagersSystem/Managers.h"
#include <iostream>

const QString GameSceneView::LOADING_GIF_PATH{Scene::SPRITES_DIRECTORY + QString{"loading.gif"}};
const QString GameSceneView::POPUP_RESULTS_PATH{Scene::SPRITES_DIRECTORY + QString{"popup_menu.png"}};
const QString GameSceneView::POPUP_WIN_PATH{Scene::SPRITES_DIRECTORY + QString{"popup_win.png"}};
const QString GameSceneView::INITIAL_WIN_LABEL_TEXT{"You won!"};

GameSceneView::GameSceneView() : board{nullptr}, loadingMovie{nullptr}, solveButton{nullptr}, proceedButton{nullptr},
    loadingScreen{nullptr}, popupSearchResults{nullptr}, searchResultsLabel{nullptr}, acknowledgmentButton{nullptr},
    popupGameWin{nullptr}, winLabel{nullptr}, restartButton{nullptr}, menuButton{nullptr}, saveButton{nullptr},
    connections{nullptr} {

}

void GameSceneView::initializeScene() {
    board = new BoardView(this);
    setUpGraphicsWidget(solveButton, "Solve", nullptr, QPointF{0, 0});
    setUpGraphicsWidget(proceedButton, "Proceed", nullptr, QPointF{0, 50});
    setUpGraphicsWidget(saveButton, "Save", nullptr, QPointF{0, 100});
    setUpLoadingScreen();
    setUpPopupSearchResults();
    setUpPopupGameWin();
    connections = new std::list<QMetaObject::Connection>(0);
    setUpConnections();
}

void GameSceneView::dispose() {
    std::cout<<"In game dispose\n";
    tryDestroyConnections(connections)
    clear();
    tryDispose(board)
    tryDispose(solveButton)
    tryDispose(proceedButton)
    tryDispose(loadingScreen)
    tryDispose(searchResultsLabel)
    tryDispose(acknowledgmentButton)
    tryDispose(winLabel)
    tryDispose(restartButton)
    tryDispose(menuButton)
    tryDispose(saveButton)
    tryDispose(connections)
    std::cout<<"After game dispose\n";
}

BoardView *GameSceneView::getBoardView() const {
    return board;
}

GraphicsWidget<QPushButton> *GameSceneView::getSolveButton() const {
    return solveButton;
}

GraphicsWidget<QPushButton> *GameSceneView::getProceedButton() const {
    return proceedButton;
}

void GameSceneView::setUpLoadingScreen() {
    loadingMovie = new QMovie{LOADING_GIF_PATH};
    setUpGraphicsWidget(loadingScreen, nullptr, QPointF{300, 200});
    loadingScreen->widgetPart->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    loadingScreen->widgetPart->setMovie(loadingMovie);
    loadingMovie->setParent(loadingScreen->widgetPart);
    enableLoadingScreen();
    disableLoadingScreen();
}

void GameSceneView::setUpPopupSearchResults() {
    popupSearchResults = new QGraphicsPixmapItem(QPixmap{POPUP_RESULTS_PATH}.scaled(128, 128));
    popupSearchResults->setPos(50, 50);
    popupSearchResults->setZValue(100);
    addItem(popupSearchResults);
    setUpGraphicsWidget(searchResultsLabel, popupSearchResults);
    setUpGraphicsWidget(acknowledgmentButton, "Got it", popupSearchResults);
    popupSearchResults->hide();
}

void GameSceneView::enableLoadingScreen() {
    handleNullReferences("Loading screen should be provided", 2, loadingScreen, loadingMovie);
    loadingScreen->graphicsPart->setVisible(true);
    loadingMovie->start();
}

void GameSceneView::disableLoadingScreen() {
    handleNullReferences("Loading screen should be provided", 2, loadingScreen, loadingMovie);
    loadingScreen->graphicsPart->setVisible(false);
    loadingMovie->stop();
}

void GameSceneView::drawResultsPopup(const QString &context) {
    handleNullReferences("Results popup should be provided", 2, searchResultsLabel, popupSearchResults);
    searchResultsLabel->widgetPart->setText(context);
    popupSearchResults->show();
}

void GameSceneView::hideResultsPopup() {
    handleNullReferences("Result popup should be provided", 1, popupSearchResults);
    popupSearchResults->hide();
}

void GameSceneView::setUpConnections() {
    connections->push_back(connect(acknowledgmentButton->widgetPart, &QPushButton::clicked, this,
                                  &GameSceneView::hideResultsPopup));
}

void GameSceneView::destroyConnections() {
    handleNullReferences("No connections to destroy", 1, connections);
    for(auto &connection : *connections) {
        disconnect(connection);
    }
    connections->clear();
}

void GameSceneView::setUpPopupGameWin() {
    popupGameWin = new QGraphicsPixmapItem(QPixmap{POPUP_WIN_PATH}.scaled(256,256));
    popupGameWin->setPos(50, 50);
    popupGameWin->setZValue(100);
    addItem(popupGameWin);
    setUpGraphicsWidget(winLabel, INITIAL_WIN_LABEL_TEXT, popupGameWin);
    setUpGraphicsWidget(restartButton, "Restart", popupGameWin, QPointF{0, 50});
    setUpGraphicsWidget(menuButton, "Menu", nullptr, QPointF{0, 200});
    popupGameWin->hide();
}

void GameSceneView::showWinPopup(const QString &timeSolving) {
    handleNullReferences("Win popup should be provided", 2, winLabel, popupGameWin);
    if(timeSolving == nullptr) {
        winLabel->widgetPart->setText(INITIAL_WIN_LABEL_TEXT);
    } else {
        winLabel->widgetPart->setText(INITIAL_WIN_LABEL_TEXT + "Time took to solve: " + timeSolving + " s");
    }
    popupGameWin->show();
}

GraphicsWidget<QPushButton> *GameSceneView::getRestartButton() const {
    return restartButton;
}

void GameSceneView::hideWinPopup() {
    popupGameWin->hide();
}

GraphicsWidget<QPushButton> *GameSceneView::getSaveButton() const {
    return saveButton;
}

GraphicsWidget<QPushButton> *GameSceneView::getMenuButton() const {
    return menuButton;
}