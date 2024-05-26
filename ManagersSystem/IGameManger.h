#pragma once

class IGameManger {
public:
    virtual void startUp() = 0;
    virtual ~IGameManger() = default;
};