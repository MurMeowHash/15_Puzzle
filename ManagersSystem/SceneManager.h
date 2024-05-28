#pragma once
#include <QGraphicsView>
#include <QVBoxLayout>
#include "IGameManger.h"
#include "../MenuSceneView.h"
#include "../GameSceneView.h"
#include "../Scenes.h"
#include "../utils.h"

class SceneManager : public QGraphicsView, public IGameManger {
    Q_OBJECT
private:
    std::shared_ptr<MenuSceneView> menuScene;
    std::shared_ptr<GameSceneView> gameScene;
    std::shared_ptr<Scene> activeScene;
    QVBoxLayout *mainLayout;
public:
    explicit SceneManager(QWidget *parent = nullptr);
    void changeScene(Scenes scene);
    void clearScenes();
    NODISCARD QPoint getCenterPoint(const QSize &targetWidget);
    NODISCARD std::shared_ptr<GameSceneView> getGameScene() const;
    NODISCARD std::shared_ptr<MenuSceneView> getMenuScene() const;
    NODISCARD QVBoxLayout *getMainLayout() const;
    void startUp() override;
};