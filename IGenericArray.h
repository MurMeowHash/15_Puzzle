#pragma once

template <typename T> class IGenericArray {
public:
    virtual T &elementAt(int arrayIndex) = 0;
    virtual const T &readElement(int arrayIndex) const = 0;
    virtual int getElementsCount() const = 0;
    virtual IGenericArray<T> *copy() const = 0;
    virtual ~IGenericArray() = default;
};