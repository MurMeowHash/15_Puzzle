#pragma once
#include <QObject>
#include <QElapsedTimer>
#include "IGameManger.h"
#include "../GameSceneView.h"
#include "../utils.h"

class ProgressManager : public QObject,  public IGameManger {
    Q_OBJECT
private:
    static constexpr int TARGET_PROGRESS = 15;
    static constexpr int INITIAL_PROGRESS = 0;
    static constexpr qint64 CONVERSION_FACTOR = 1000;
    int currentProgress;
    bool solverUsage;
    QElapsedTimer currentTimer;
    qint64 timeSolving;
    qint64 movesCount;
    qint64 bestScore;
public:
    void increaseProgress();
    void decreaseProgress();
    void setProgress(int currentProgress);
    void enableSolverUsage(bool usage);
    void startTrackingTime();
    qint64 stopTrackingTime();
    qint64 increaseMovesCount();
    void setSolvingTime(qint64 targetTime);
    void setBestScore(qint64 targetBestScore);
    void wipeProgress();
    NODISCARD qint64 getTimeSolving() const;
    NODISCARD qint64 getBestScore() const;
    NODISCARD bool getSolverUsage() const;
    NODISCARD bool checkWin() const;
    void startUp() override;
    signals:
    void gameWin(const QString &timeData = nullptr);
    void bestScoreUpdated();
};