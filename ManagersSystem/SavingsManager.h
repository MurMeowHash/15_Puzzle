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
    std::string savingsPath;
    std::string savingStatePath;
    std::string scoreDataPath;
    std::string preferencesPath;
    static void writeFile(FILE *fileHandler, void *buffer, int elSize, int elCount, const std::string &msg);
    static void readFile(FILE *fileHandler, void *buffer, int elSize, int elCount, const std::string &msg);
public:
    void saveState(const BoardModel &targetBoard);
    bool saveBestScore();
    void savePreferences();
    qint64 readBestScore();
    BoardModel *getSavedState();
    void loadPreferences();
    bool savedStateExist();
    void startUp() override;
};