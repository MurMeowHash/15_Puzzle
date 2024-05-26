#pragma once

#define tryDestroyConnections(arg) \
if(arg != nullptr){                \
destroyConnections();              \
}

class IConnectable {
public:
    virtual void setUpConnections() = 0;
    virtual void destroyConnections() = 0;
    virtual ~IConnectable() = default;
};