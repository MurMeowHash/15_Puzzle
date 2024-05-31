#pragma once
#include <QObject>
#include "IDisposable.h"
#include "IConnectable.h"

class QString;

class AbstractController : public QObject, public IDisposable, public IConnectable {
    Q_OBJECT
public:
    virtual void initializeController(const QString &flag) = 0;
    static void interconnectControllers(AbstractController *menuController, AbstractController *gameController);
public slots:
    void getControl(AbstractController *controller, const QString &flag = nullptr);
    signals:
    void passControl(AbstractController *controller, const QString &flag = nullptr);
};