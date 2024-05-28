#pragma once
#include <QGraphicsScene>
#include <QFont>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QLayout>
#include "IConnectable.h"
#include "IDisposable.h"
#include "GraphicsWidget.h"

class Scene : public QGraphicsScene, public IConnectable, public IDisposable {
    Q_OBJECT
private:
    static QMessageBox *messageBox;
    static const QString FONT_PATH;
protected:
    static QFont applicationFont;
    template<typename T>
    void setUpGraphicsWidget(GraphicsWidget<T> *&targetWidget, T *widgetPart, const QPointF &targetPos);
    static void setUpWidgetButton(QPushButton *&targetButton, const QString &spritePath, const QString &text,
                           int fontSize, const QSize &buttonSize);
    static void setUpWindow(QWidget *&targetWindow, const QString &background, const QSize &targetSize, const QPoint &targetPos,
                     QWidget *parent);
    static void setUpLabel(QLabel *&targetLabel, const QString &text, int fontSize);
    template<typename T>
    static void setUpSingleLayout(T *&targetLayout, QWidget *targetWidget, const Qt::Alignment &alignment);
public:
    static constexpr const char *SPRITES_DIRECTORY{"./sprites/"};
    static constexpr QSize MANAGEMENT_BUTTONS_SIZE{64, 64};
    static constexpr QPoint COORDINATES_CENTER{0, 0};
    static constexpr int UNDEFINED_SIZE = 1;
    static void setUpApplicationFont();
    static void setUpMessageBox();
    static void clearMessageBox();
    static void raiseMessageBox(const QString &message);
    virtual void freeMainLayout() = 0;
public slots:
    virtual void initializeScene() = 0;
};

template<typename T>
void Scene::setUpGraphicsWidget(GraphicsWidget<T> *&targetWidget, T *widgetPart, const QPointF &targetPos) {
    targetWidget = new GraphicsWidget<T>;
    targetWidget->widgetPart = widgetPart;
    targetWidget->graphicsPart = addWidget(targetWidget->widgetPart);
    targetWidget->graphicsPart->setPos(targetPos);
}

template<typename T>
void Scene::setUpSingleLayout(T *&targetLayout, QWidget *targetWidget, const Qt::Alignment &alignment) {
    targetLayout = new T;
    targetLayout->addWidget(targetWidget);
    targetLayout->setAlignment(alignment);
}