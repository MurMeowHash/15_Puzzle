#include <QApplication>
#include "MenuController.h"
#include "GameController.h"
#include "ManagersSystem/Managers.h"
#include "FileException.h"

MenuController::MenuController() : connections{nullptr} {

}

void MenuController::startGame() {
    emit passControl(this);
}

void MenuController::loadGame() {
    if(SavingsManager::savedStateExist()) {
        emit passControl(this, GameController::LOAD_STATE_FLAG);
    } else {
        //TODO: message box
        std::cout<<"No savings\n";
    }
}

void MenuController::exitGame() {
    //TODO: save preferences
    QApplication::quit();
}

void MenuController::initializeController(const QString &flag) {
    Managers::getScene()->changeScene(Scenes::Menu);
    connections = new std::list<QMetaObject::Connection>(0);
    setUpConnections();
}

void MenuController::setUpConnections() {
    std::shared_ptr<MenuSceneView> menuScene = Managers::getScene()->getMenuScene();
    GraphicsWidget<QPushButton> *playButton = menuScene->getPlayButton();
    GraphicsWidget<QPushButton> *loadGameButton = menuScene->getLoadGameButton();
    GraphicsWidget<QPushButton> *quitButton = menuScene->getQuitButton();
    GraphicsWidget<QComboBox> *resDemandDropDown = menuScene->getResourcesDemandDropDown();
    GraphicsWidget<QLineEdit> *timeSolvingInputLine = menuScene->getTimeSolvingInputLine();
    connections->push_back(connect(playButton->widgetPart, &QPushButton::clicked, this,
                                   &MenuController::startGame));
    connections->push_back(connect(loadGameButton->widgetPart, &QPushButton::clicked, this,
                                   &MenuController::loadGame));
    connections->push_back(connect(quitButton->widgetPart, &QPushButton::clicked, this,
                                   &MenuController::exitGame));
    connections->push_back(connect(resDemandDropDown->widgetPart, &QComboBox::currentIndexChanged, this,
                                   &MenuController::setResourcesDemand));
    connections->push_back(connect(timeSolvingInputLine->widgetPart, &QLineEdit::editingFinished, this,
                                   &MenuController::setMaxTimeSolving));
    connections->push_back(connect(this, &MenuController::updateSolvingTimeLine, timeSolvingInputLine->widgetPart,
                                   &QLineEdit::setText));
}

void MenuController::destroyConnections() {
    handleNullReferences("No connections to destroy", 1, connections);
    for(auto &connection : *connections) {
        disconnect(connection);
    }
    connections->clear();
}

void MenuController::dispose() {
    tryDestroyConnections(connections)
    tryDispose(connections);
}

void MenuController::setResourcesDemand(int index) {
    Managers::getPreferences()->setResourcesOption(static_cast<ResourcesOption>(index));
}

void MenuController::setMaxTimeSolving() {
    GraphicsWidget<QLineEdit> *timeSolvingInputLine = Managers::getScene()->getMenuScene()->getTimeSolvingInputLine();
    bool correctInput;
    QString inputTime = timeSolvingInputLine->widgetPart->text();
    qint64 targetMaxTime = inputTime.toLongLong(&correctInput);
    int newTimeSolving = static_cast<int>(Managers::getPreferences()->getMaxSolutionTimeSec());
    if(correctInput) {
        Managers::getPreferences()->setMaxSolutionTime(targetMaxTime);
        newTimeSolving = static_cast<int>(Managers::getPreferences()->getMaxSolutionTimeSec());
    }
    emit updateSolvingTimeLine(QString::number(newTimeSolving));
}