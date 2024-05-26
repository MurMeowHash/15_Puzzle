#include "SceneManager.h"

SceneManager::SceneManager(QWidget *parent) : QGraphicsView(parent) {

}

void SceneManager::startUp() {
    notifyManagerStarting("Scene");
    menuScene = std::make_shared<MenuSceneView>();
    gameScene = std::make_shared<GameSceneView>();
    activeScene = nullptr;
}

void SceneManager::changeScene(Scenes scene) {
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

QSize SceneManager::sizeHint() const {
    return QSize{800, 600};
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
}