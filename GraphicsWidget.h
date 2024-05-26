#pragma once
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>

class QPushButton;
class QLabel;

#include <iostream>

template<typename T>
struct GraphicsWidget {
    T *widgetPart;
    QGraphicsProxyWidget *graphicsPart;
    void setUp(QGraphicsScene &targetScene);
    void setUpGraphicsPart(QGraphicsScene &targetScene);
};

template<typename T>
void GraphicsWidget<T>::setUp(QGraphicsScene &targetScene) {
    widgetPart = new T;
    graphicsPart = targetScene.addWidget(widgetPart);
}

template<typename T>
void GraphicsWidget<T>::setUpGraphicsPart(QGraphicsScene &targetScene) {
    graphicsPart = targetScene.addWidget(widgetPart);
}
