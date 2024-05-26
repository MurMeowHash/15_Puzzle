#include <iostream>
#include "SavingsManager.h"
#include "Managers.h"
#include "../FileException.h"
#include "../GameController.h"

const std::string SavingsManager::SAVINGS_PATH{"./savings/"};
const std::string SavingsManager::SAVINGS_STATE_NAME{"state_data.dat"};

const std::string SavingsManager::SCORE_DATA_NAME{"best_score_data.dat"};

void SavingsManager::startUp() {
    notifyManagerStarting("Savings");
}

void SavingsManager::writeFile(FILE *fileHandler, void *buffer, int elSize, int elCount) {
    int writtenCount = static_cast<int>(fwrite(buffer, elSize, elCount, fileHandler));
    checkIOCorruptions(writtenCount, elCount);
}

void SavingsManager::readFile(FILE *fileHandler, void *buffer, int elSize, int elCount) {
    int readCount = static_cast<int>(fread(buffer, elSize, elCount, fileHandler));
    checkIOCorruptions(readCount, elCount);
}

void SavingsManager::saveState(const BoardModel &targetBoard) {
    try {
        std::string fileName{SAVINGS_PATH + SAVINGS_STATE_NAME};
        FILE *targetStateStream = fopen(fileName.data(), "wb");
        checkOpeningCorruptions(targetStateStream, fileName);
        int boardLength = targetBoard.getLength();
        quint64 elementSize = sizeof(targetBoard[0][0]);
        int writtenCount{0};
        for(int i = 0; i < boardLength; i++) {
            writtenCount += static_cast<int>(fwrite(targetBoard[i].data(), elementSize,
                                                    boardLength, targetStateStream));
        }
        checkIOCorruptions(writtenCount, targetBoard.getElementsCount());
        qint64 timeSolving = Managers::getProgress()->stopTrackingTime();
        writeFile(targetStateStream, &timeSolving, sizeof(timeSolving), 1);
        fclose(targetStateStream);
    } catch(FileException &ex) {
        std::cout<<ex.what();
    }
}

qint64 SavingsManager::readBestScore() {
    FILE *bestScoreStream = fopen((SAVINGS_PATH + SCORE_DATA_NAME).data(), "rb");
    if(bestScoreStream == nullptr) {
        return INT_MAX;
    }
    qint64 bestScore{INT_MAX};
    try {
        readFile(bestScoreStream, &bestScore, sizeof(bestScore), 1);
    } catch(FileException &ex) {
        //TODO: message box
        std::cout<<ex.what();
    }
    fclose(bestScoreStream);
    return bestScore;
}

void SavingsManager::saveBestScore() {
    qint64 prevScore = readBestScore();
    qint64 currentScore = Managers::getProgress()->getTimeSolving();
    if(currentScore >= prevScore) {
        return;
    }

    try {
        std::string fileName{SAVINGS_PATH + SCORE_DATA_NAME};
        FILE *bestScoreStream = fopen(fileName.data(), "wb");
        checkOpeningCorruptions(bestScoreStream, fileName);
        writeFile(bestScoreStream, &currentScore, sizeof(currentScore), 1);
        fclose(bestScoreStream);
    } catch(FileException &ex) {
        //TODO: Message box
        std::cout<<ex.what();
    }
}

BoardModel *SavingsManager::getSavedState() {
    std::string fileName{SAVINGS_PATH + SAVINGS_STATE_NAME};
    FILE *savedStateStream = fopen(fileName.data(), "rb");
    checkOpeningCorruptions(savedStateStream, fileName);
    QList<QList<int>> retrievedMatrix(GameController::PUZZLE_TABLE_LENGTH);
    for(QList<int> &row : retrievedMatrix) {
        row.resize(GameController::PUZZLE_TABLE_LENGTH);
    }
    for(QList<int> &row : retrievedMatrix) {
        readFile(savedStateStream, row.data(), sizeof(int), GameController::PUZZLE_TABLE_LENGTH);
    }
    qint64 prevSolvingTime;
    readFile(savedStateStream, &prevSolvingTime, sizeof(prevSolvingTime), 1);
    Managers::getProgress()->setSolvingTime(prevSolvingTime);
    return new BoardModel{retrievedMatrix};
}

bool SavingsManager::savedStateExist() {
    FILE *savedStateStream = fopen((SAVINGS_PATH + SAVINGS_STATE_NAME).data(), "rb");
    if(savedStateStream == nullptr) {
        return false;
    }
    fclose(savedStateStream);
    return true;
}