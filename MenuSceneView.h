#pragma once
#include <QGraphicsScene>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "GraphicsWidget.h"
#include "Scene.h"
#include "utils.h"

class MenuSceneView : public Scene {
    Q_OBJECT
private:
    static const QString PREFERENCES_BACK_PATH;
    GraphicsWidget<QPushButton> *playButton;
    GraphicsWidget<QPushButton> *loadGameButton;
    GraphicsWidget<QPushButton> *preferencesButton;
    GraphicsWidget<QPushButton> *quitButton;
    QGraphicsPixmapItem *preferencesWindow;
    GraphicsWidget<QComboBox> *resDemandDropDown;
    GraphicsWidget<QLineEdit> *timeSolvingInputLine;
    GraphicsWidget<QLabel> *demandLabel;
    GraphicsWidget<QPushButton> *closePreferencesButton;
    std::list<QMetaObject::Connection> *connections;
public:
    MenuSceneView();
    NODISCARD GraphicsWidget<QPushButton> *getPlayButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getLoadGameButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getPreferencesButton() const;
    NODISCARD GraphicsWidget<QPushButton> *getQuitButton() const;
    NODISCARD GraphicsWidget<QComboBox> *getResourcesDemandDropDown() const;
    NODISCARD GraphicsWidget<QLineEdit> *getTimeSolvingInputLine() const;
    void setUpPreferencesWindow();
    void setUpConnections() override;
    void destroyConnections() override;
public slots:
    void showPreferences();
    void hidePreferences();
    void initializeScene() override;
    void dispose() override;
};