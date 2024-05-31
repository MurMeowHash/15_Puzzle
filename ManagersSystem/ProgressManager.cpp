#include <QElapsedTimer>
#include "ProgressManager.h"
#include "Managers.h"

void ProgressManager::startUp() {
    notifyManagerStarting("Progress");
    wipeProgress();
    bestScore = INT_MAX;
}

void ProgressManager::increaseProgress() {
    currentProgress++;
    if(currentProgress > TARGET_PROGRESS) {
        currentProgress = TARGET_PROGRESS;
    }
    if(checkWin()) {
        stopTrackingTime();
        if(Managers::getSavings()->saveBestScore()) {
            std::shared_ptr<ProgressManager> progressManager = Managers::getProgress();
            progressManager->setBestScore(progressManager->getTimeSolving());
            emit bestScoreUpdated();
        }
        emit gameWin(QString::number(timeSolving));
    }
}

void ProgressManager::decreaseProgress() {
    currentProgress--;
    if(currentProgress < INITIAL_PROGRESS) {
        currentProgress = INITIAL_PROGRESS;
    }
}

bool ProgressManager::checkWin() const {
    return currentProgress >= TARGET_PROGRESS;
}

void ProgressManager::setProgress(int currentProgress) {
    if(currentProgress >= INITIAL_PROGRESS && currentProgress <= TARGET_PROGRESS) {
        this->currentProgress = currentProgress;
    }
}

void ProgressManager::enableSolverUsage(bool usage) {
    solverUsage = usage;
}

bool ProgressManager::getSolverUsage() const {
    return solverUsage;
}

void ProgressManager::startTrackingTime() {
    currentTimer.start();
}

qint64 ProgressManager::stopTrackingTime() {
    timeSolving += currentTimer.elapsed() / CONVERSION_FACTOR;
    return timeSolving;
}

qint64 ProgressManager::increaseMovesCount() {
    return ++movesCount;
}

qint64 ProgressManager::getTimeSolving() const {
    return timeSolving;
}

void ProgressManager::wipeProgress() {
    currentProgress = INITIAL_PROGRESS;
    solverUsage = false;
    timeSolving = 0;
    movesCount = 0;
}

void ProgressManager::setSolvingTime(qint64 targetTime) {
    if(targetTime > 0) {
        timeSolving = targetTime;
    }
}

void ProgressManager::setBestScore(qint64 targetBestScore) {
    if(targetBestScore > 0) {
        bestScore = targetBestScore;
    }
}

qint64 ProgressManager::getBestScore() const {
    return bestScore;
}