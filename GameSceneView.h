#pragma once
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QMovie>
#include "Scene.h"
#include "BoardView.h"
#include "GraphicsWidget.h"
#include "utils.h"

class GameSceneView : public Scene {
    Q_OBJECT
private:
    static const QString SPRITES_DIRECTORY;
    static const QString LOADING_GIF_PATH;
    static const QString POPUP_RESULTS_PATH;
    static const QString POPUP_WIN_PATH;
    static const QString INITIAL_WIN_LABEL_TEXT;
    std::list<QMetaObject::Connection> *connections;
    BoardView *board;
    QMovie *loadingMovie;
    GraphicsWidget<QPushButton> *solveButton;
    GraphicsWidget<QPushButton> *proceedButton;
    GraphicsWidget<QLabel> *loadingScreen;
    QGraphicsPixmapItem *popupSearchResults;
    GraphicsWidget<QLabel> *searchResultsLabel;
    GraphicsWidget<QPushButton> *acknowledgmentButton;
    QGraphicsPixmapItem *popupGameWin;
    GraphicsWidget<QLabel> *winLabel;
    GraphicsWidget<QPushButton> *restartButton;
    GraphicsWidget<QPushButton> *menuButton;
    GraphicsWidget<QPushButton> *saveButton;
    void setUpLoadingScreen();
    void setUpPopupSearchResults();
    void setUpPopupGameWin();
    void setUpConnections() override;
    void destroyConnections() override;
public:
    GameSceneView();
    NODISCARD BoardView *getBoardView() const;
    NODISCARD GraphicsWidget<QPushButton> *getSolveButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getProceedButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getRestartButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getSaveButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getMenuButton() const;
public slots:
    void enableLoadingScreen();
    void disableLoadingScreen();
    void drawResultsPopup(const QString &context);
    void hideResultsPopup();
    void showWinPopup(const QString &timeSolving = nullptr);
    void hideWinPopup();
    void initializeScene() override;
    void dispose() override;
};