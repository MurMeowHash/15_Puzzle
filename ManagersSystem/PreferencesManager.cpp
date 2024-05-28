#include "PreferencesManager.h"

void PreferencesManager::startUp() {
    notifyManagerStarting("Preferences");
    updateData(ResourcesOption::LowDemand, DEFAULT_SOLUTION_TIME * CONVERSION_FACTOR);
}

void PreferencesManager::setResourcesOption(const ResourcesOption &targetDemand) {
    resourcesDemand = targetDemand;
}

ResourcesOption PreferencesManager::getResourcesDemand() const {
    return resourcesDemand;
}

void PreferencesManager::setMaxSolutionTime(qint64 targetTime) {
    if(targetTime > UPPER_BOUND_SOLUTION_TIME) {
        maxSolutionTime = UPPER_BOUND_SOLUTION_TIME;
    } else if(targetTime < LOWER_BOUND_SOLUTION_TIME) {
        maxSolutionTime = LOWER_BOUND_SOLUTION_TIME;
    } else {
        maxSolutionTime = targetTime;
    }
    maxSolutionTime *= CONVERSION_FACTOR;
}

qint64 PreferencesManager::getMaxSolutionTimeSec() const {
    return maxSolutionTime / CONVERSION_FACTOR;
}

qint64 PreferencesManager::getMaxSolutionTimeMSec() const {
    return maxSolutionTime;
}

void PreferencesManager::updateData(const ResourcesOption &targetDemand, qint64 targetMaxSolutionTime) {
    resourcesDemand = targetDemand;
    setMaxSolutionTime(targetMaxSolutionTime);
}