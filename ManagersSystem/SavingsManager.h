#pragma once
#include <string>
#include "IGameManger.h"
#include "../BoardModel.h"

#define tryClose(arg) \
if(arg != nullptr) {  \
fclose(arg);          \
}


class SavingsManager : public IGameManger {
private:
    static const std::string SAVINGS_PATH;
    static const std::string SAVINGS_STATE_PATH;
    static const std::string SCORE_DATA_PATH;
    static const std::string PREFERENCES_PATH;
    static void writeFile(FILE *fileHandler, void *buffer, int elSize, int elCount, const std::string &msg);
    static void readFile(FILE *fileHandler, void *buffer, int elSize, int elCount, const std::string &msg);
public:
    static void saveState(const BoardModel &targetBoard);
    static bool saveBestScore();
    static void savePreferences();
    static qint64 readBestScore();
    static BoardModel *getSavedState();
    static void loadPreferences();
    static bool savedStateExist();
    void startUp() override;
};