#pragma once
#include <QGraphicsScene>
#include "IConnectable.h"
#include "IDisposable.h"
#include "GraphicsWidget.h"

class Scene : public QGraphicsScene, public IConnectable, public IDisposable {
    Q_OBJECT
protected:
    static constexpr const char *SPRITES_DIRECTORY{"./sprites/"};
    template<typename T>
    void setUpGraphicsWidget(GraphicsWidget<T>*& targetWidget, const QString &context, QGraphicsItem *parent,
                             const QPointF &pos = QPointF{0, 0});
    template<typename T>
    void setUpGraphicsWidget(GraphicsWidget<T>*& targetWidget, QGraphicsItem *parent,
                             const QPointF &pos = QPointF{0, 0});
public slots:
    virtual void initializeScene() = 0;
};

template<typename T>
void Scene::setUpGraphicsWidget(GraphicsWidget<T>*& targetWidget, const QString &context, QGraphicsItem *parent,
                                const QPointF &pos) {
    setUpGraphicsWidget(targetWidget, parent, pos);
    targetWidget->widgetPart->setText(context);
}

//TODO: fix the problem: not to add widget to scene by myself if it has a parent

template<typename T>
void Scene::setUpGraphicsWidget(GraphicsWidget<T> *&targetWidget, QGraphicsItem *parent, const QPointF &pos) {
    targetWidget = new GraphicsWidget<T>;
    targetWidget->widgetPart = new T;
    targetWidget->setUpGraphicsPart(*this);
    targetWidget->graphicsPart->setParentItem(parent);
    targetWidget->graphicsPart->setPos(pos);
}