#pragma once
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QMovie>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsLinearLayout>
#include "Scene.h"
#include "BoardView.h"
#include "GraphicsWidget.h"
#include "utils.h"

class GameSceneView : public Scene {
    Q_OBJECT
private:
    static const QString SPRITES_DIRECTORY;
    static const QString LOADING_GIF_PATH;
    static const QString POPUP_WIN_PATH;
    static const QString INITIAL_WIN_LABEL_TEXT;
    static const QString HOME_BUTTON_ICON_PATH;
    static const QString RESTART_BUTTON_ICON_PATH;
    static const QString POPUP_RES_ICON_PATH;
    static const QString REGULAR_BUTTON_ICON_PATH;
    static const QString BLOCKED_BUTTON_ICON_PATH;
    static const QString BEST_SCORE_ICON_PATH;
    static constexpr QSize LOADING_MOVIE_SIZE{64, 64};
    static constexpr QSize POPUP_SEARCH_RES_SIZE{400, 300};
    static constexpr QSize REGULAR_BUTTON_SIZE{90, 32};
    static constexpr QSize POP_RES_WIN_SIZE{600, 450};
    static constexpr int SPACE_BETWEEN_RES_WIDGETS{25};
    static constexpr int CONTROL_BUTTONS_FONT_SIZE{24};
    static constexpr int CONTROL_BUTTONS_PADDING{10};
    static constexpr int CONTROL_BUTTON_PROPORTION_FACTOR{2};
    static constexpr qreal CONTROL_BUTTONS_OFFSET{50.};
    BoardView *board;
    GraphicsWidget<QPushButton> *solveButton;
    GraphicsWidget<QPushButton> *proceedButton;
    GraphicsWidget<QPushButton> *saveButton;
    QPushButton *restartButton;
    QPushButton *menuButton;
    QHBoxLayout *restartButtonLayout;
    QHBoxLayout *menuButtonLayout;
    QVBoxLayout *managementWidgetsLayout;
    QWidget *loadingBackground;
    QVBoxLayout *loadingMovieVLayout;
    QHBoxLayout *loadingMovieHLayout;
    QMovie *loadingMovie;
    QLabel *loadingScreenLabel;
    QWidget *popupSearchResults;
    QLabel *searchResultsLabel;
    QPushButton *acknowledgmentButton;
    QHBoxLayout *acknowledgmentLayout;
    QVBoxLayout *popupSearchLayout;
    QWidget *popupGameWin;
    QLabel *winLabel;
    QHBoxLayout *winLabelLayout;
    QWidget *winBackground;
    QPushButton *restartWinButton;
    QPushButton *menuWinButton;
    QHBoxLayout *managementWinButtonsLayout;
    QVBoxLayout *winLayout;
    QHBoxLayout *bestScoreLayout;
    QWidget *bestScoreIcon;
    QLabel *bestScoreText;
    std::list<QMetaObject::Connection> *connections;
    QString unblockedSaveButtonStyleShit;
    QString unblockedSaveButtonText;
    void setUpCanvas();
    void setUpLoadingScreen();
    void setUpLoadingMovie();
    void setUpPopupSearchResults();
    void setUpPopupGameWin();
    void setUpWinButtons();
    void setUpWinLabel();
    bool setUpBestScore();
    void setUpConnections() override;
    void destroyConnections() override;
    void freeMainLayout() override;
public:
    GameSceneView();
    NODISCARD BoardView *getBoardView() const;
    NODISCARD GraphicsWidget<QPushButton> *getSolveButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getProceedButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getSaveButton() const;
    NODISCARD QPushButton *getRestartButton() const;
    NODISCARD QPushButton *getMenuButton() const;
    NODISCARD QPushButton *getRestartWinButton() const;
    NODISCARD QPushButton *getMenuWinButton() const;
public slots:
    void setUpControlButtons(const QPointF &leftBorderCoords, const QPointF &rightBorderCoords);
    void enableLoadingScreen();
    void disableLoadingScreen();
    void drawResultsPopup(const QString &context);
    void hideResultsPopup();
    void showWinPopup(const QString &timeSolving = nullptr);
    void hideWinPopup();
    void blockSaveButton();
    void unblockSaveButton();
    void onUpdateBestScore();
    void initializeScene() override;
    void dispose() override;
    signals:
    void controlButtonsCreated();
};