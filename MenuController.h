#pragma once
#include <QObject>
#include "AbstractController.h"
#include "IConnectable.h"
#include "MenuSceneView.h"
#include "ResourcesOption.h"

class MenuController : public AbstractController {
    Q_OBJECT
private:
    std::list<QMetaObject::Connection> *connections{nullptr};
    void setUpConnections() override;
    void destroyConnections() override;
public:
    void setResourcesDemand(int index);
    void setMaxTimeSolving();
    void dispose() override;
public slots:
    void initializeController(const QString &flag) override;
    void startGame();
    void loadGame();
    void exitGame();
    signals:
    void updateSolvingTimeLine(const QString &targetTime);
};