#include <iostream>
#include "Managers.h"

std::shared_ptr<ProgressManager> Managers::progress;
std::shared_ptr<SceneManager> Managers::scene;
std::shared_ptr<PreferencesManager> Managers::preferences;
std::shared_ptr<SavingsManager> Managers::savings;

void Managers::setUpManagers() {
    preferences = std::make_shared<PreferencesManager>();
    scene = std::make_shared<SceneManager>();
    progress = std::make_shared<ProgressManager>();
    savings = std::make_shared<SavingsManager>();
}

void Managers::startManagers() {
    setUpManagers();
    QList<std::shared_ptr<IGameManger>> managersList{preferences, scene, progress, savings};
    std::cout<<"Starting managers...\n";
    for(auto &manager : managersList) {
        manager->startUp();
    }
    std::cout<<"Managers has been loaded\n";
}

std::shared_ptr<PreferencesManager> Managers::getPreferences() {
    return preferences;
}

std::shared_ptr<SceneManager> Managers::getScene() {
    return scene;
}

std::shared_ptr<ProgressManager> Managers::getProgress() {
    return progress;
}

std::shared_ptr<SavingsManager> Managers::getSavings() {
    return savings;
}