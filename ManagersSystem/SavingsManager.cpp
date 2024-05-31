#include "SavingsManager.h"
#include "Managers.h"
#include "../FileException.h"
#include "../GameController.h"

void SavingsManager::startUp() {
    notifyManagerStarting("Savings");
    savingsPath = "./savings/";
    savingStatePath = savingsPath + "state_data.dat";
    scoreDataPath = savingsPath + "best_score_data.dat";
    preferencesPath = savingsPath + "preferences.dat";
    loadPreferences();
    Managers::getProgress()->setBestScore(readBestScore());
}

void SavingsManager::writeFile(FILE *fileHandler, void *buffer, int elSize, int elCount, const std::string &msg) {
    int writtenCount = static_cast<int>(fwrite(buffer, elSize, elCount, fileHandler));
    checkIOCorruptions(writtenCount, elCount, msg);
}

void SavingsManager::readFile(FILE *fileHandler, void *buffer, int elSize, int elCount, const std::string &msg) {
    int readCount = static_cast<int>(fread(buffer, elSize, elCount, fileHandler));
    checkIOCorruptions(readCount, elCount, msg);
}

void SavingsManager::saveState(const BoardModel &targetBoard) {
    FILE *targetStateStream{nullptr};
    std::string errorMessage{"Can not save current state"};
    try {
        targetStateStream = fopen(savingStatePath.data(), "wb");
        checkOpeningCorruptions(targetStateStream, errorMessage);
        int boardLength = targetBoard.getLength();
        quint64 elementSize = sizeof(targetBoard[0][0]);
        int writtenCount{0};
        for(int i = 0; i < boardLength; i++) {
            writtenCount += static_cast<int>(fwrite(targetBoard[i].data(), elementSize,
                                                    boardLength, targetStateStream));
        }
        checkIOCorruptions(writtenCount, targetBoard.getElementsCount(), errorMessage);
        qint64 timeSolving = Managers::getProgress()->stopTrackingTime();
        writeFile(targetStateStream, &timeSolving, sizeof(timeSolving), 1, errorMessage);
    } catch(FileException &ex) {
        Scene::raiseMessageBox(ex.what());
    }
    tryClose(targetStateStream)
}

qint64 SavingsManager::readBestScore() {
    FILE *bestScoreStream = fopen(scoreDataPath.data(), "rb");
    if(bestScoreStream == nullptr) {
        return INT_MAX;
    }
    qint64 bestScore{INT_MAX};
    try {
        readFile(bestScoreStream, &bestScore, sizeof(bestScore), 1, "Unable to get best score");
    } catch(FileException &ex) {
        Scene::raiseMessageBox(ex.what());
    }
    fclose(bestScoreStream);
    return bestScore;
}

bool SavingsManager::saveBestScore() {
    qint64 prevScore = readBestScore();
    qint64 currentScore = Managers::getProgress()->getTimeSolving();
    if(currentScore >= prevScore) {
        return false;
    }
    std::string errorMessage{"Unable to save best score"};
    FILE *bestScoreStream{nullptr};
    try {
        bestScoreStream = fopen(scoreDataPath.data(), "wb");
        checkOpeningCorruptions(bestScoreStream, errorMessage);
        writeFile(bestScoreStream, &currentScore, sizeof(currentScore), 1, errorMessage);
        fclose(bestScoreStream);
        return true;
    } catch(FileException &ex) {
        tryClose(bestScoreStream)
        Scene::raiseMessageBox(ex.what());
        return false;
    }
}

BoardModel *SavingsManager::getSavedState() {
    FILE *savedStateStream = fopen(savingStatePath.data(), "rb");
    std::string errorMessage{"Can not get saved state"};
    checkOpeningCorruptions(savedStateStream, errorMessage);
    QList<QList<int>> retrievedMatrix(GameController::PUZZLE_TABLE_LENGTH);
    for(QList<int> &row : retrievedMatrix) {
        row.resize(GameController::PUZZLE_TABLE_LENGTH);
    }
    for(QList<int> &row : retrievedMatrix) {
        readFile(savedStateStream, row.data(), sizeof(int), GameController::PUZZLE_TABLE_LENGTH,
                 errorMessage);
    }
    qint64 prevSolvingTime;
    readFile(savedStateStream, &prevSolvingTime, sizeof(prevSolvingTime), 1, errorMessage);
    Managers::getProgress()->setSolvingTime(prevSolvingTime);
    return new BoardModel{retrievedMatrix};
}

bool SavingsManager::savedStateExist() {
    FILE *savedStateStream = fopen((savingStatePath).data(), "rb");
    if(savedStateStream == nullptr) {
        return false;
    }
    fclose(savedStateStream);
    return true;
}

void SavingsManager::savePreferences() {
    FILE *preferencesStream{nullptr};
    std::string errorMessage{"Unable to save preferences"};
    try {
        preferencesStream = fopen(preferencesPath.data(), "wb");
        checkOpeningCorruptions(preferencesStream, errorMessage);
        int currentDemandOption = static_cast<int>(Managers::getPreferences()->getResourcesDemand());
        int currentMaxTimeSolving = static_cast<int>(Managers::getPreferences()->getMaxSolutionTimeSec());
        writeFile(preferencesStream, &currentDemandOption, sizeof(currentDemandOption), 1,
                  errorMessage);
        writeFile(preferencesStream, &currentMaxTimeSolving, sizeof(currentMaxTimeSolving), 1,
                  errorMessage);
    } catch(FileException &ex) {
        Scene::raiseMessageBox(ex.what());
    }
    tryClose(preferencesStream)
}

void SavingsManager::loadPreferences() {
    FILE *preferencesStream = fopen(preferencesPath.data(), "rb");
    if(preferencesStream == nullptr) {
        return;
    }
    std::string errorMessage{"Can not obtain preferences"};
    int targetDemandOption, targetMaxSolvingTime;
    try {
        readFile(preferencesStream, &targetDemandOption, sizeof(targetDemandOption), 1,
                 errorMessage);
        readFile(preferencesStream, &targetMaxSolvingTime, sizeof(targetDemandOption), 1,
                 errorMessage);
        Managers::getPreferences()->updateData(static_cast<ResourcesOption>(targetDemandOption),
                                               targetMaxSolvingTime);
    } catch(FileException &ex) {
        Scene::raiseMessageBox(ex.what() + QString{" <Default preferences will be loaded>"});
    }
    fclose(preferencesStream);
}