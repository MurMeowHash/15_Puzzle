#include "Scene.h"
#include <QFontDataBase>
#include "ManagersSystem/Managers.h"

const QString Scene::FONT_PATH{"./fonts/JosefinSans-Regular.ttf"};
QFont Scene::applicationFont;
QMessageBox *Scene::messageBox;

void Scene::setUpApplicationFont() {
    int fontId = QFontDatabase::addApplicationFont(FONT_PATH);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    applicationFont = QFont{fontFamily};
}

void Scene::setUpWidgetButton(QPushButton *&targetButton, const QString &spritePath, const QString &text, int fontSize,
                              const QSize &buttonSize) {
    targetButton = new QPushButton{text};
    targetButton->setFixedSize(buttonSize);
    targetButton->setStyleSheet("background-color: transparent;"
                                "border-image: url(" + spritePath + ");"
                                "font-size: " + QString::number(fontSize) + "px;"
                                "color: white;"
                                "font-weight: 600;");
}

void Scene::setUpWindow(QWidget *&targetWindow, const QString &background, const QSize &targetSize,
                        const QPoint &targetPos, QWidget *parent) {
    targetWindow = new QWidget{parent};
    targetWindow->setFixedSize(targetSize);
    targetWindow->move(targetPos);
    targetWindow->setStyleSheet("background-color: transparent;" + background);
}

void Scene::setUpLabel(QLabel *&targetLabel, const QString &text, int fontSize) {
    targetLabel = new QLabel{text};
    targetLabel->setFont(applicationFont);
    targetLabel->setStyleSheet("background-color: transparent;"
                               "border-image: none;"
                               "font-size: " + QString::number(fontSize) + "px");
}

void Scene::setUpMessageBox() {
    messageBox = new QMessageBox{nullptr};
    messageBox->setStyleSheet("background-color: #1c1c1c;"
                              "font-size: 24px;");
}

void Scene::raiseMessageBox(const QString &message) {
    messageBox->setText(message);
    messageBox->exec();
}

void Scene::clearMessageBox() {
    delete messageBox;
}