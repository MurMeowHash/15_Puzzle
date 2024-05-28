#include "SceneManager.h"

SceneManager::SceneManager(QWidget *parent) : QGraphicsView(parent) {

}

void SceneManager::startUp() {
    notifyManagerStarting("Scene");
    Scene::setUpApplicationFont();
    menuScene = std::make_shared<MenuSceneView>();
    gameScene = std::make_shared<GameSceneView>();
    activeScene = nullptr;
    setStyleSheet("background-color: #27292E");
    setWindowState(Qt::WindowFullScreen);
    mainLayout = new QVBoxLayout{this};
    Scene::setUpMessageBox();
}

void SceneManager::changeScene(Scenes scene) {
    if(activeScene != nullptr) {
        activeScene->freeMainLayout();
    }
    QMetaObject::invokeMethod(activeScene.get(), "dispose", Qt::QueuedConnection);
    switch(scene) {
        case Scenes::Menu:
            setScene(menuScene.get());
            activeScene = menuScene;
            break;
        case Scenes::Game:
            setScene(gameScene.get());
            activeScene = gameScene;
            break;
    }
    activeScene->initializeScene();
    show();
}

std::shared_ptr<MenuSceneView> SceneManager::getMenuScene() const {
    return menuScene;
}

std::shared_ptr<GameSceneView> SceneManager::getGameScene() const {
    return gameScene;
}

void SceneManager::clearScenes() {
    menuScene->dispose();
    gameScene->dispose();
    Scene::clearMessageBox();
    setScene(nullptr);
}

QVBoxLayout *SceneManager::getMainLayout() const {
    return mainLayout;
}

QPoint SceneManager::getCenterPoint(const QSize &targetWidget) {
    return {width() / 2 - targetWidget.width() / 2,height() / 2 - targetWidget.height() / 2};
}