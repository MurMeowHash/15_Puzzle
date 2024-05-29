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
    QPushButton *playButton{nullptr};
    QPushButton *loadGameButton{nullptr};
    QPushButton *preferencesButton{nullptr};
    QPushButton *quitButton{nullptr};
    QLabel *gameNameLabel{nullptr};
    QHBoxLayout *playButtonLayout{nullptr};
    QHBoxLayout *loadGameButtonLayout{nullptr};
    QHBoxLayout *preferencesButtonLayout{nullptr};
    QHBoxLayout *quitButtonLayout{nullptr};
    QHBoxLayout *gameNameLabelLayout{nullptr};
    QVBoxLayout *menuLayout{nullptr};
    QWidget *preferencesWindow{nullptr};
    QComboBox *resDemandDropDown{nullptr};
    QVBoxLayout *preferencesItemsLayout{nullptr};
    QLabel *timeSolvingLabel{nullptr};
    QLineEdit *timeSolvingInputLine{nullptr};
    QVBoxLayout *timeSolvingLayout{nullptr};
    QVBoxLayout *demandLayout{nullptr};
    QLabel *demandLabel{nullptr};
    QPushButton *closePreferencesButton{nullptr};
    QVBoxLayout *closeButtonLayout{nullptr};
    std::list<QMetaObject::Connection> *connections{nullptr};
    void setUpMenuView();
    void setUpDemandView();
    void setUpTimeSolvingView();
    void setUpClosePrefsButton();
public:
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