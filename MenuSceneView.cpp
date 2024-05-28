#include "MenuSceneView.h"
#include "ManagersSystem/Managers.h"

const QString MenuSceneView::PREFERENCES_BACK_PATH{Scene::SPRITES_DIRECTORY + QString{"preferences_back.png"}};
const QString MenuSceneView::BUTTON_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"button_back.png"}};
const QString MenuSceneView::HOME_BUTTON_ICON_PATH{Scene::SPRITES_DIRECTORY + QString{"home_button_back.png"}};

MenuSceneView::MenuSceneView() : playButton{nullptr}, loadGameButton{nullptr}, preferencesButton{nullptr},
    quitButton{nullptr}, gameNameLabel{nullptr}, playButtonLayout{nullptr}, loadGameButtonLayout{nullptr},
    preferencesButtonLayout{nullptr}, quitButtonLayout{nullptr}, gameNameLabelLayout{nullptr}, menuLayout{nullptr},
    preferencesWindow{nullptr}, resDemandDropDown{nullptr}, preferencesItemsLayout{nullptr}, timeSolvingLabel{nullptr},
    timeSolvingInputLine{nullptr}, timeSolvingLayout{nullptr}, demandLayout{nullptr}, demandLabel{nullptr},
    closePreferencesButton{nullptr}, closeButtonLayout{nullptr}, connections{nullptr} {

}

void MenuSceneView::initializeScene() {
    QVBoxLayout *mainLayout = Managers::getScene()->getMainLayout();
    setUpMenuView();
    mainLayout->addLayout(menuLayout);
    setUpPreferencesWindow();
    connections = new std::list<QMetaObject::Connection>(0);
    setUpConnections();
}

void MenuSceneView::setUpPreferencesWindow() {
    std::shared_ptr<SceneManager> painterWidget = Managers::getScene();
    QPoint centerPos = painterWidget->getCenterPoint(PREFERENCES_WINDOW_SIZE);
    setUpWindow(preferencesWindow, "border-image: url(" + PREFERENCES_BACK_PATH + ");",
                PREFERENCES_WINDOW_SIZE, centerPos, painterWidget.get());
    setUpDemandView();
    setUpTimeSolvingView();
    setUpClosePrefsButton();
    preferencesItemsLayout = new QVBoxLayout{preferencesWindow};
    preferencesItemsLayout->addLayout(closeButtonLayout);
    preferencesItemsLayout->addSpacing(PREFERENCES_SHIFT);
    preferencesItemsLayout->addLayout(demandLayout);
    preferencesItemsLayout->addSpacing(50);
    preferencesItemsLayout->addLayout(timeSolvingLayout);
    preferencesItemsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    preferencesWindow->raise();
    preferencesWindow->hide();
}

QPushButton *MenuSceneView::getPlayButton() const {
    return playButton;
}

QPushButton *MenuSceneView::getQuitButton() const {
    return quitButton;
}

void MenuSceneView::setUpConnections() {
    connections->push_back(connect(preferencesButton, &QPushButton::clicked, this,
                                   &MenuSceneView::showPreferences));
    connections->push_back(connect(closePreferencesButton, &QPushButton::clicked, this,
                                   &MenuSceneView::hidePreferences));
}

void MenuSceneView::destroyConnections() {
    handleNullReferences("No connections to destroy", 1, connections);
    for(auto &connection : *connections) {
        disconnect(connection);
    }
    connections->clear();
}

void MenuSceneView::dispose() {
    tryDestroyConnections(connections)
    tryDispose(playButton)
    tryDispose(loadGameButton)
    tryDispose(preferencesButton)
    tryDispose(quitButton)
    tryDispose(gameNameLabel)
//    tryDispose(playButtonLayout)
//    tryDispose(loadGameButtonLayout)
//    tryDispose(preferencesButtonLayout)
//    tryDispose(quitButtonLayout)
//    tryDispose(gameNameLabelLayout)
    tryDispose(menuLayout)
    tryDispose(preferencesWindow)
    tryDispose(connections)
}

QPushButton *MenuSceneView::getLoadGameButton() const {
    return loadGameButton;
}

QPushButton *MenuSceneView::getPreferencesButton() const {
    return preferencesButton;
}

void MenuSceneView::showPreferences() {
    preferencesWindow->show();
}

QComboBox *MenuSceneView::getResourcesDemandDropDown() const {
    return resDemandDropDown;
}

QLineEdit *MenuSceneView::getTimeSolvingInputLine() const {
    return timeSolvingInputLine;
}

void MenuSceneView::hidePreferences() {
    preferencesWindow->hide();
}

void MenuSceneView::setUpDemandView() {
    setUpLabel(demandLabel, "Resources demand", 24);
    resDemandDropDown = new QComboBox;
    resDemandDropDown->addItem("Low");
    resDemandDropDown->addItem("High");
    resDemandDropDown->setFont(applicationFont);
    resDemandDropDown->setCurrentIndex(static_cast<int>(Managers::getPreferences()->getResourcesDemand()));
    resDemandDropDown->setStyleSheet("border-image: none;"
                                     "font-size: 20px");
    demandLayout = new QVBoxLayout;
    demandLayout->addWidget(demandLabel);
    demandLayout->addWidget(resDemandDropDown);
    demandLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void MenuSceneView::setUpTimeSolvingView() {
    setUpLabel(timeSolvingLabel, "Max time solving(sec)", 24);
    timeSolvingInputLine = new QLineEdit;
    timeSolvingInputLine->setFont(applicationFont);
    QString currentMaxSolutionTime = QString::number(Managers::getPreferences()->getMaxSolutionTimeSec());
    timeSolvingInputLine->setText(currentMaxSolutionTime);
    timeSolvingInputLine->setStyleSheet("border-image: none;"
                                        "border-color: #FF3B62;"
                                        "border-width: 3px;"
                                        "border-style: solid;"
                                        "font-size: 24px");
    timeSolvingLayout = new QVBoxLayout;
    timeSolvingLayout->addWidget(timeSolvingLabel);
    timeSolvingLayout->addWidget(timeSolvingInputLine);
    timeSolvingLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void MenuSceneView::setUpClosePrefsButton() {
    setUpWidgetButton(closePreferencesButton, HOME_BUTTON_ICON_PATH, nullptr, UNDEFINED_SIZE,
                      MANAGEMENT_BUTTONS_SIZE);
    setUpSingleLayout(closeButtonLayout, closePreferencesButton, Qt::AlignRight | Qt::AlignTop);
}

void MenuSceneView::freeMainLayout() {
    tryDispose(menuLayout)
}

void MenuSceneView::setUpMenuView() {
    setUpLabel(gameNameLabel, "15 Puzzle", 64);
    setUpSingleLayout(gameNameLabelLayout, gameNameLabel, Qt::AlignCenter);
    menuLayout = new QVBoxLayout;
    menuLayout->addLayout(gameNameLabelLayout);
    menuLayout->addSpacing(BUTTON_PADDING);
    QPushButton **targetButtons[] = {&playButton, &loadGameButton, &preferencesButton, &quitButton};
    QString targetNames[] {"Play", "Load Game", "Preferences", "Quit"};
    QHBoxLayout **buttonLayouts[] = {&playButtonLayout, &loadGameButtonLayout, &preferencesButtonLayout,
                                     &quitButtonLayout};
    int targetLength = sizeof(targetButtons) / sizeof(targetButtons[0]);
    for(int i = 0; i < targetLength; i++) {
        setUpWidgetButton(*targetButtons[i], BUTTON_ICON_PATH, targetNames[i], 20,
                          MENU_BUTTON_SIZE);
        setUpSingleLayout(*buttonLayouts[i], *targetButtons[i], Qt::AlignCenter);
        menuLayout->addLayout(*buttonLayouts[i]);
        menuLayout->addSpacing(BUTTON_PADDING);
    }
    menuLayout->setAlignment(Qt::AlignCenter);
}