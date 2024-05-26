#pragma once
#include <QList>
#include "PreferencesManager.h"
#include "SceneManager.h"
#include "ProgressManager.h"
#include "SavingsManager.h"

class Managers {
private:
    static std::shared_ptr<PreferencesManager> preferences;
    static std::shared_ptr<SceneManager> scene;
    static std::shared_ptr<ProgressManager> progress;
    static std::shared_ptr<SavingsManager> savings;
    static void setUpManagers();
public:
    static void startManagers();
    static std::shared_ptr<PreferencesManager> getPreferences();
    static std::shared_ptr<SceneManager> getScene();
    static std::shared_ptr<ProgressManager> getProgress();
    static std::shared_ptr<SavingsManager> getSavings();
};