#pragma once

#define tryDispose(arg) \
if(arg != nullptr) {    \
delete arg;             \
arg = nullptr;          \
}

class IDisposable {
public:
    virtual void dispose() = 0;
    virtual ~IDisposable() = default;
};