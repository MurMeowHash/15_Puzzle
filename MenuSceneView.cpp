#include "MenuSceneView.h"
#include "ManagersSystem/Managers.h"

const QString MenuSceneView::PREFERENCES_BACK_PATH{Scene::SPRITES_DIRECTORY + QString{"preferences_back.png"}};

MenuSceneView::MenuSceneView() : playButton{nullptr}, loadGameButton{nullptr}, preferencesButton{nullptr},
    quitButton{nullptr}, preferencesWindow{nullptr}, resDemandDropDown{nullptr}, timeSolvingInputLine{nullptr},
    demandLabel{nullptr}, closePreferencesButton{nullptr}, connections{nullptr} {

}

void MenuSceneView::initializeScene() {
    setUpGraphicsWidget(playButton, "Play", nullptr, QPointF{0, 0});
    setUpGraphicsWidget(loadGameButton, "Load Game", nullptr, QPointF{0, 50});
    setUpGraphicsWidget(preferencesButton, "Preferences", nullptr, QPointF{0, 100});
    setUpGraphicsWidget(quitButton, "Quit", nullptr, QPointF{0, 150});
    setUpPreferencesWindow();
    connections = new std::list<QMetaObject::Connection>(0);
    setUpConnections();
}

void MenuSceneView::setUpPreferencesWindow() {
    preferencesWindow = new QGraphicsPixmapItem{QPixmap{PREFERENCES_BACK_PATH}.scaled(800, 600)};
    preferencesWindow->setZValue(150);
    addItem(preferencesWindow);
    setUpGraphicsWidget(demandLabel, "Resources demand", preferencesWindow, QPointF{0, 0});
    setUpGraphicsWidget(resDemandDropDown, preferencesWindow, QPointF{0, 20});
    resDemandDropDown->widgetPart->addItem("Low");
    resDemandDropDown->widgetPart->addItem("High");
    resDemandDropDown->widgetPart->setCurrentIndex(static_cast<int>(Managers::getPreferences()->getResourcesDemand()));
    QString currentMaxSolutionTime = QString::number(Managers::getPreferences()->getMaxSolutionTimeSec());
    setUpGraphicsWidget(timeSolvingInputLine, currentMaxSolutionTime, preferencesWindow,
                        QPointF{0, 100});
    setUpGraphicsWidget(closePreferencesButton, "Menu", preferencesWindow, QPointF{0, 300});
    preferencesWindow->hide();
}

GraphicsWidget<QPushButton> *MenuSceneView::getPlayButton() const {
    return playButton;
}

GraphicsWidget<QPushButton> *MenuSceneView::getQuitButton() const {
    return quitButton;
}

void MenuSceneView::setUpConnections() {
    connections->push_back(connect(preferencesButton->widgetPart, &QPushButton::clicked, this,
                                   &MenuSceneView::showPreferences));
    connections->push_back(connect(closePreferencesButton->widgetPart, &QPushButton::clicked, this,
                                   &MenuSceneView::hidePreferences));
}

void MenuSceneView::destroyConnections() {
    handleNullReferences("No connections to destroy", 1, connections);
    for(auto &connection : *connections) {
        disconnect(connection);
    }
    connections->clear();
}

void MenuSceneView::dispose() {
    std::cout<<"In menu dispose\n";
    tryDestroyConnections(connections)
    clear();
    tryDispose(playButton)
    tryDispose(loadGameButton)
    tryDispose(preferencesButton)
    tryDispose(quitButton)
    tryDispose(connections)
    tryDispose(resDemandDropDown)
    tryDispose(timeSolvingInputLine)
    tryDispose(closePreferencesButton)
    std::cout<<"After menu dispose\n";
}

GraphicsWidget<QPushButton> *MenuSceneView::getLoadGameButton() const {
    return loadGameButton;
}

GraphicsWidget<QPushButton> *MenuSceneView::getPreferencesButton() const {
    return preferencesButton;
}

void MenuSceneView::showPreferences() {
    preferencesWindow->show();
}

GraphicsWidget<QComboBox> *MenuSceneView::getResourcesDemandDropDown() const {
    return resDemandDropDown;
}

GraphicsWidget<QLineEdit> *MenuSceneView::getTimeSolvingInputLine() const {
    return timeSolvingInputLine;
}

void MenuSceneView::hidePreferences() {
    preferencesWindow->hide();
}