#pragma once
#include <QIntegerForSize>
#include "IGameManger.h"
#include "../ResourcesOption.h"
#include "../utils.h"

class PreferencesManager : public IGameManger {
private:
    static constexpr qint64 CONVERSION_FACTOR = 1000;
    ResourcesOption resourcesDemand;
    qint64 maxSolutionTime;
public:
    static constexpr qint64 LOWER_BOUND_SOLUTION_TIME = 1;
    static constexpr qint64 UPPER_BOUND_SOLUTION_TIME = 120;
    static constexpr qint64 DEFAULT_SOLUTION_TIME = 10;
    void setResourcesOption(const ResourcesOption &targetAlgorithm);
    void updateData(const ResourcesOption &targetDemand, qint64 targetMaxSolutionTime);
    NODISCARD ResourcesOption getResourcesDemand() const;
    void setMaxSolutionTime(qint64 targetTime);
    NODISCARD qint64 getMaxSolutionTimeSec() const;
    NODISCARD qint64 getMaxSolutionTimeMSec() const;
    void startUp() override;
};