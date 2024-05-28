#pragma once
#include <QGraphicsProxyWidget>

class QPushButton;
class QLabel;

template<typename T>
struct GraphicsWidget {
    T *widgetPart;
    QGraphicsProxyWidget *graphicsPart;
};