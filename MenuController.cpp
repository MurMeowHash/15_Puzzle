#include <QApplication>
#include "MenuController.h"
#include "GameController.h"
#include "ManagersSystem/Managers.h"
#include "FileException.h"

void MenuController::startGame() {
    emit passControl(this);
}

void MenuController::loadGame() {
    if(Managers::getSavings()->savedStateExist()) {
        emit passControl(this, GameController::LOAD_STATE_FLAG);
    } else {
        Scene::raiseMessageBox("No savings");
    }
}

void MenuController::exitGame() {
    QApplication::quit();
}

void MenuController::initializeController(const QString &flag) {
    Managers::getScene()->changeScene(Scenes::Menu);
    connections = new std::list<QMetaObject::Connection>(0);
    setUpConnections();
}

void MenuController::setUpConnections() {
    std::shared_ptr<MenuSceneView> menuScene = Managers::getScene()->getMenuScene();
    QPushButton *playButton = menuScene->getPlayButton();
    QPushButton *loadGameButton = menuScene->getLoadGameButton();
    QPushButton *quitButton = menuScene->getQuitButton();
    QComboBox *resDemandDropDown = menuScene->getResourcesDemandDropDown();
    QLineEdit *timeSolvingInputLine = menuScene->getTimeSolvingInputLine();
    connections->push_back(connect(playButton, &QPushButton::clicked, this,
                                   &MenuController::startGame));
    connections->push_back(connect(loadGameButton, &QPushButton::clicked, this,
                                   &MenuController::loadGame));
    connections->push_back(connect(quitButton, &QPushButton::clicked, this,
                                   &MenuController::exitGame));
    connections->push_back(connect(resDemandDropDown, &QComboBox::currentIndexChanged, this,
                                   &MenuController::setResourcesDemand));
    connections->push_back(connect(timeSolvingInputLine, &QLineEdit::editingFinished, this,
                                   &MenuController::setMaxTimeSolving));
    connections->push_back(connect(this, &MenuController::updateSolvingTimeLine, timeSolvingInputLine,
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
    QLineEdit *timeSolvingInputLine = Managers::getScene()->getMenuScene()->getTimeSolvingInputLine();
    bool correctInput;
    QString inputTime = timeSolvingInputLine->text();
    qint64 targetMaxTime = inputTime.toLongLong(&correctInput);
    int newTimeSolving = static_cast<int>(Managers::getPreferences()->getMaxSolutionTimeSec());
    if(correctInput) {
        Managers::getPreferences()->setMaxSolutionTime(targetMaxTime);
        newTimeSolving = static_cast<int>(Managers::getPreferences()->getMaxSolutionTimeSec());
    }
    emit updateSolvingTimeLine(QString::number(newTimeSolving));
}