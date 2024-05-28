#pragma once
#include <QGraphicsScene>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include "Scene.h"
#include "utils.h"

class MenuSceneView : public Scene {
    Q_OBJECT
private:
    static const QString PREFERENCES_BACK_PATH;
    static const QString BUTTON_ICON_PATH;
    static const QString HOME_BUTTON_ICON_PATH;
    static constexpr QSize MENU_BUTTON_SIZE{150, 64};
    static constexpr QSize PREFERENCES_WINDOW_SIZE{800, 600};
    static constexpr int BUTTON_PADDING = 25;
    static constexpr int PREFERENCES_SHIFT = 70;
    QPushButton *playButton;
    QPushButton *loadGameButton;
    QPushButton *preferencesButton;
    QPushButton *quitButton;
    QLabel *gameNameLabel;
    QHBoxLayout *playButtonLayout;
    QHBoxLayout *loadGameButtonLayout;
    QHBoxLayout *preferencesButtonLayout;
    QHBoxLayout *quitButtonLayout;
    QHBoxLayout *gameNameLabelLayout;
    QVBoxLayout *menuLayout;
    QWidget *preferencesWindow;
    QComboBox *resDemandDropDown;
    QVBoxLayout *preferencesItemsLayout;
    QLabel *timeSolvingLabel;
    QLineEdit *timeSolvingInputLine;
    QVBoxLayout *timeSolvingLayout;
    QVBoxLayout *demandLayout;
    QLabel *demandLabel;
    QPushButton *closePreferencesButton;
    QVBoxLayout *closeButtonLayout;
    std::list<QMetaObject::Connection> *connections;
    void setUpMenuView();
    void setUpDemandView();
    void setUpTimeSolvingView();
    void setUpClosePrefsButton();
public:
    MenuSceneView();
    NODISCARD QPushButton *getPlayButton() const;
    NODISCARD QPushButton *getLoadGameButton() const;
    NODISCARD QPushButton *getPreferencesButton() const;
    NODISCARD QPushButton *getQuitButton() const;
    NODISCARD QComboBox *getResourcesDemandDropDown() const;
    NODISCARD QLineEdit *getTimeSolvingInputLine() const;
    void setUpPreferencesWindow();
    void setUpConnections() override;
    void destroyConnections() override;
    void freeMainLayout() override;
public slots:
    void showPreferences();
    void hidePreferences();
    void initializeScene() override;
    void dispose() override;
};