#pragma once
#include <string>
#include "IGameManger.h"
#include "../BoardModel.h"

class SavingsManager : public IGameManger {
private:
    static const std::string SAVINGS_PATH;
    static const std::string SAVINGS_STATE_NAME;
    static const std::string SCORE_DATA_NAME;
    static void writeFile(FILE *fileHandler, void *buffer, int elSize, int elCount);
    static void readFile(FILE *fileHandler, void *buffer, int elSize, int elCount);
public:
    static void saveState(const BoardModel &targetBoard);
    static void saveBestScore();
    static qint64 readBestScore();
    static BoardModel *getSavedState();
    static bool savedStateExist();
    void startUp() override;
};