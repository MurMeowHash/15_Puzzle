#include <QPushButton>
#include <QMovie>
#include "GameSceneView.h"
#include "ManagersSystem/Managers.h"
#include <iostream>

const QString GameSceneView::LOADING_GIF_PATH{Scene::SPRITES_DIRECTORY + QString{"loading.gif"}};
const QString GameSceneView::HOME_BUTTON_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"home_button_back.png"}};
const QString GameSceneView::RESTART_BUTTON_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"restart_button_back.png"}};
const QString GameSceneView::POPUP_RES_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"popup_res_back.png"}};
const QString GameSceneView::REGULAR_BUTTON_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"button_back.png"}};
const QString GameSceneView::BLOCKED_BUTTON_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"button_blocked_back.png"}};
const QString GameSceneView::BEST_SCORE_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"best_score.png"}};
const QString GameSceneView::INITIAL_WIN_LABEL_TEXT{"You won!"};

GameSceneView::GameSceneView() : board{nullptr}, solveButton{nullptr}, proceedButton{nullptr}, saveButton{nullptr},
    restartButton{nullptr}, menuButton{nullptr}, restartButtonLayout{nullptr}, menuButtonLayout{nullptr},
    managementWidgetsLayout{nullptr}, loadingBackground{nullptr}, loadingMovieVLayout{nullptr}, loadingMovieHLayout{nullptr},
    loadingMovie{nullptr}, loadingScreenLabel{nullptr}, popupSearchResults{nullptr}, searchResultsLabel{nullptr},
    acknowledgmentButton{nullptr}, acknowledgmentLayout{nullptr}, popupSearchLayout{nullptr}, popupGameWin{nullptr},
    winLabel{nullptr}, winLabelLayout{nullptr}, winBackground{nullptr}, restartWinButton{nullptr}, menuWinButton{nullptr},
    managementWinButtonsLayout{nullptr}, winLayout{nullptr}, bestScoreText{nullptr}, bestScoreIcon{nullptr},
    bestScoreLayout{nullptr}, connections{nullptr} {

}

void GameSceneView::initializeScene() {
    board = new BoardView{this};
    setUpCanvas();
    connections = new std::list<QMetaObject::Connection>(0);
    setUpConnections();
}

void GameSceneView::setUpCanvas() {
    QVBoxLayout *mainLayout = Managers::getScene()->getMainLayout();
    managementWidgetsLayout = new QVBoxLayout;
    setUpWidgetButton(menuButton, HOME_BUTTON_ICON_PATH, nullptr, UNDEFINED_SIZE,
                      MANAGEMENT_BUTTONS_SIZE);
    setUpWidgetButton(restartButton, RESTART_BUTTON_ICON_PATH, nullptr, UNDEFINED_SIZE,
                      MANAGEMENT_BUTTONS_SIZE);
    setUpSingleLayout(menuButtonLayout, menuButton, Qt::AlignRight);
    setUpSingleLayout(restartButtonLayout, restartButton, Qt::AlignRight);
    managementWidgetsLayout->addLayout(menuButtonLayout);
    managementWidgetsLayout->addLayout(restartButtonLayout);
    if(setUpBestScore()) {
        managementWidgetsLayout->addLayout(bestScoreLayout);
    }
    managementWidgetsLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
    mainLayout->addLayout(managementWidgetsLayout);
    setUpLoadingScreen();
    setUpPopupSearchResults();
    setUpPopupGameWin();
}

void GameSceneView::dispose() {
    tryDestroyConnections(connections)
    clear();
    tryDispose(board)
    tryDispose(solveButton)
    tryDispose(proceedButton)
    tryDispose(saveButton)
    tryDispose(restartButton)
    tryDispose(menuButton)
    tryDispose(bestScoreText)
    tryDispose(bestScoreIcon)
    tryDispose(managementWidgetsLayout)
    tryDispose(loadingBackground)
    tryDispose(popupSearchResults)
    tryDispose(winBackground)
    tryDispose(connections)
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
    std::shared_ptr<QGraphicsView> painterWidget = Managers::getScene();
    setUpWindow(loadingBackground, "background-color: rgba(76, 76, 76, 150)", painterWidget->size(),
                QPoint{0,0}, painterWidget.get());
    setUpLoadingMovie();
    enableLoadingScreen();
    disableLoadingScreen();
}

void GameSceneView::setUpLoadingMovie() {
    loadingScreenLabel = new QLabel;
    loadingScreenLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    loadingMovie = new QMovie{LOADING_GIF_PATH};
    loadingMovie->setScaledSize(LOADING_MOVIE_SIZE);
    loadingScreenLabel->setMovie(loadingMovie);
    loadingMovie->setParent(loadingScreenLabel);
    setUpSingleLayout(loadingMovieHLayout, loadingScreenLabel, Qt::AlignCenter);
    loadingMovieVLayout = new QVBoxLayout{loadingBackground};
    loadingMovieVLayout->addLayout(loadingMovieHLayout);
    loadingMovieVLayout->setAlignment(Qt::AlignCenter);
    loadingScreenLabel->raise();
    enableLoadingScreen();
    disableLoadingScreen();
}

void GameSceneView::setUpPopupSearchResults() {
    std::shared_ptr<SceneManager> painterWidget = Managers::getScene();
    QPoint targetPos = painterWidget->getCenterPoint(POPUP_SEARCH_RES_SIZE);
    setUpWindow(popupSearchResults, "border-image: url(" + POPUP_RES_ICON_PATH + ");",
                POPUP_SEARCH_RES_SIZE, targetPos, painterWidget.get());
    setUpLabel(searchResultsLabel, nullptr, 24);
    setUpWidgetButton(acknowledgmentButton, REGULAR_BUTTON_ICON_PATH, "Got it", 22,
                      REGULAR_BUTTON_SIZE);
    setUpSingleLayout(acknowledgmentLayout, acknowledgmentButton, Qt::AlignCenter);
    popupSearchLayout = new QVBoxLayout{popupSearchResults};
    popupSearchLayout->addWidget(searchResultsLabel);
    popupSearchLayout->addSpacing(SPACE_BETWEEN_RES_WIDGETS);
    popupSearchLayout->addLayout(acknowledgmentLayout);
    popupSearchLayout->setAlignment(Qt::AlignCenter);
    popupSearchResults->hide();
}

void GameSceneView::enableLoadingScreen() {
    handleNullReferences("Loading screen should be provided", 2, loadingBackground, loadingMovie);
    loadingBackground->show();
    loadingMovie->start();
}

void GameSceneView::disableLoadingScreen() {
    handleNullReferences("Loading screen should be provided", 2, loadingBackground, loadingMovie);
    loadingBackground->hide();
    loadingMovie->stop();
}

void GameSceneView::drawResultsPopup(const QString &context) {
    handleNullReferences("Results popup should be provided", 2, searchResultsLabel, popupSearchResults);
    searchResultsLabel->setText(context);
    popupSearchResults->show();
}

void GameSceneView::hideResultsPopup() {
    handleNullReferences("Result popup should be provided", 1, popupSearchResults);
    popupSearchResults->hide();
}

void GameSceneView::setUpConnections() {
    ProgressManager *progressManager = Managers::getProgress().get();
    connections->push_back(connect(acknowledgmentButton, &QPushButton::clicked, this,
                                  &GameSceneView::hideResultsPopup));
    connections->push_back(connect(board, &BoardView::boardDrawn, this,
                                   &GameSceneView::setUpControlButtons));
    connections->push_back(connect(progressManager, &ProgressManager::gameWin, this,
                                   &GameSceneView::showWinPopup));
    connections->push_back(connect(progressManager, &ProgressManager::bestScoreUpdated, this,
                                   &GameSceneView::onUpdateBestScore));
}

void GameSceneView::destroyConnections() {
    handleNullReferences("No connections to destroy", 1, connections);
    for(auto &connection : *connections) {
        disconnect(connection);
    }
    connections->clear();
}

void GameSceneView::setUpPopupGameWin() {
    std::shared_ptr<SceneManager> painterWidget = Managers::getScene();
    QPoint centerPos = painterWidget->getCenterPoint(POP_RES_WIN_SIZE);
    setUpWindow(winBackground, "background-color: rgba(0, 0, 0, 45)", painterWidget->size(),
                QPoint{0, 0}, painterWidget.get());
    setUpWindow(popupGameWin, "border-image: url(" + POPUP_RES_ICON_PATH + ");", POP_RES_WIN_SIZE,
                centerPos, winBackground);
    setUpWinLabel();
    setUpWinButtons();
    winLayout = new QVBoxLayout{popupGameWin};
    winLayout->addLayout(winLabelLayout);
    winLayout->addSpacing(SPACE_BETWEEN_RES_WIDGETS);
    winLayout->addLayout(managementWinButtonsLayout);
    winLayout->setAlignment(Qt::AlignCenter);
    winBackground->hide();
}

void GameSceneView::showWinPopup(const QString &timeSolving) {
    handleNullReferences("Win popup should be provided", 2, winLabel, winBackground);
    if(timeSolving == nullptr) {
        winLabel->setText(INITIAL_WIN_LABEL_TEXT);
    } else {
        winLabel->setText(INITIAL_WIN_LABEL_TEXT + " Time took to solve: " + timeSolving + " s");
    }
    winBackground->show();
}

QPushButton *GameSceneView::getRestartButton() const {
    return restartButton;
}

void GameSceneView::hideWinPopup() {
    handleNullReferences("Win popup should be provided", 1, winBackground);
    winBackground->hide();
}

GraphicsWidget<QPushButton> *GameSceneView::getSaveButton() const {
    return saveButton;
}

QPushButton *GameSceneView::getMenuButton() const {
    return menuButton;
}

void GameSceneView::freeMainLayout() {
    tryDispose(managementWidgetsLayout);
}

void GameSceneView::setUpWinButtons() {
    setUpWidgetButton(menuWinButton, HOME_BUTTON_ICON_PATH, nullptr, UNDEFINED_SIZE,
                      MANAGEMENT_BUTTONS_SIZE);
    setUpWidgetButton(restartWinButton, RESTART_BUTTON_ICON_PATH, nullptr, UNDEFINED_SIZE,
                      MANAGEMENT_BUTTONS_SIZE);
    managementWinButtonsLayout = new QHBoxLayout;
    managementWinButtonsLayout->addWidget(menuWinButton);
    managementWinButtonsLayout->addWidget(restartWinButton);
    managementWinButtonsLayout->setAlignment(Qt::AlignCenter);
}

QPushButton *GameSceneView::getRestartWinButton() const {
    return restartWinButton;
}

QPushButton *GameSceneView::getMenuWinButton() const {
    return menuWinButton;
}

void GameSceneView::setUpWinLabel() {
    setUpLabel(winLabel, INITIAL_WIN_LABEL_TEXT, 30);
    setUpSingleLayout(winLabelLayout, winLabel, Qt::AlignCenter);
}

void GameSceneView::setUpControlButtons(const QPointF &leftBorderCoords, const QPointF &rightBorderCoords) {
    QPushButton *widgetButtons[3];
    QString buttonNames[] = {"Solve", "Proceed", "Save"};
    GraphicsWidget<QPushButton> **controlButtons[] = {&solveButton, &proceedButton, &saveButton};
    int arrSize = sizeof(controlButtons) / sizeof(controlButtons[0]);
    int boardWidth = static_cast<int>(round(rightBorderCoords.x() - leftBorderCoords.x()));
    int controlButtonWidth = (boardWidth - (arrSize - 1) * CONTROL_BUTTONS_PADDING) / arrSize;
    QSize controlButtonSize{controlButtonWidth, controlButtonWidth / CONTROL_BUTTON_PROPORTION_FACTOR};
    QPointF currentPos{leftBorderCoords.x(),
                       leftBorderCoords.y() - controlButtonSize.height() / 2. - CONTROL_BUTTONS_OFFSET};
    for(int i = 0; i < arrSize; i++) {
        setUpWidgetButton(widgetButtons[i], REGULAR_BUTTON_ICON_PATH, buttonNames[i],
                          CONTROL_BUTTONS_FONT_SIZE, controlButtonSize);
        setUpGraphicsWidget(*controlButtons[i], widgetButtons[i], currentPos);
        currentPos.setX(currentPos.x() + controlButtonSize.width() + CONTROL_BUTTONS_PADDING);
    }
    unblockedSaveButtonStyleShit = saveButton->widgetPart->styleSheet();
    unblockedSaveButtonText = saveButton->widgetPart->text();
    emit controlButtonsCreated();
}

void GameSceneView::blockSaveButton() {
    saveButton->widgetPart->setStyleSheet("border-image: url(" + BLOCKED_BUTTON_ICON_PATH + ");"
                                          "background-color: transparent;");
    saveButton->widgetPart->setText(nullptr);
}

bool GameSceneView::setUpBestScore() {
    qint64 bestScore = Managers::getProgress()->getBestScore();
    if(bestScore == INT_MAX) {
        return false;
    }
    setUpLabel(bestScoreText, QString::number(bestScore) + " s", 20);
    setUpWindow(bestScoreIcon, "border-image: url(" + BEST_SCORE_ICON_PATH + ");",
                MANAGEMENT_BUTTONS_SIZE, COORDINATES_CENTER,nullptr);
    bestScoreLayout = new QHBoxLayout;
    bestScoreLayout->addWidget(bestScoreText);
    bestScoreLayout->addWidget(bestScoreIcon);
    bestScoreLayout->setAlignment(Qt::AlignRight);
    return true;
}

void GameSceneView::onUpdateBestScore() {
    if(bestScoreText == nullptr) {
        setUpBestScore();
        managementWidgetsLayout->addLayout(bestScoreLayout);
    }
    bestScoreText->setText(QString::number(Managers::getProgress()->getBestScore()) + " s");
}

void GameSceneView::unblockSaveButton() {
    if(saveButton->widgetPart->styleSheet() != unblockedSaveButtonStyleShit) {
        saveButton->widgetPart->setStyleSheet(unblockedSaveButtonStyleShit);
        saveButton->widgetPart->setText(unblockedSaveButtonText);
    }
}