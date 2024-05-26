#pragma once
#include <QRandomGenerator>
#include <QList>
#include "IGenericArray.h"
#include "Point.h"

class Algorithms {
private:
    template<typename T> static int mergeCountInversions(IGenericArray<T> &targetArray, int beginIndex, int middleIndex, int endIndex);
    template<typename T> static int sortGetInversionsCount(IGenericArray<T> &targetArray, int beginIndex, int endIndex);
public:
    static inline void transformToMatrixIndex(int arrayIndex, Point &outPosition, int columnLength);
    static inline int transformToArrayIndex(const Point &position, int columnLength);
    template<typename T> static void randomSort(IGenericArray<T> &array, int beginIndex, int endIndex);
    template<typename T> static int getInversionsCount(const IGenericArray<T> &targetArray, int beginIndex, int endIndex);
    static inline int pickRandom(const QList<int> &array);
    template <typename T> static void approachElement(IGenericArray<T> &targetArray, int approachingPosition, int targetPosition);
    static int calculateManhattan(const Point &currentPos, const Point &targetPos);
};

template<typename T>
void Algorithms::randomSort(IGenericArray<T> &array, int beginIndex, int endIndex) {
    int upperBound = endIndex + 1;
    for(int i = beginIndex; i < endIndex; i++) {
        int randomIndex = QRandomGenerator::global()->bounded(i, upperBound);
        std::swap(array.elementAt(i), array.elementAt(randomIndex));
    }
}

template<typename T>
int Algorithms::mergeCountInversions(IGenericArray<T> &targetArray, int beginIndex, int middleIndex, int endIndex) {
    int inversionsCount{0};
    int leftHalfLength{middleIndex - beginIndex + 1};
    int rightHalfLength{endIndex - middleIndex};
    T *leftHalf = new T[leftHalfLength];
    T *rightHalf = new T[rightHalfLength];

    for(int i = 0; i < leftHalfLength; i++) {
        leftHalf[i] = targetArray.elementAt(beginIndex + i);
    }
    for(int i = 0; i < rightHalfLength; i++) {
        rightHalf[i] = targetArray.elementAt(middleIndex + i + 1);
    }

    int leftHalfIndex{0}, rightHalfIndex{0}, targetArrayIndex{beginIndex};
    while(leftHalfIndex < leftHalfLength
          && rightHalfIndex < rightHalfLength) {
        if(leftHalf[leftHalfIndex] <= rightHalf[rightHalfIndex]) {
            targetArray.elementAt(targetArrayIndex++) = leftHalf[leftHalfIndex++];
        } else {
            targetArray.elementAt(targetArrayIndex++) = rightHalf[rightHalfIndex++];
            inversionsCount += leftHalfLength - leftHalfIndex;
        }
    }

    while(leftHalfIndex < leftHalfLength) {
        targetArray.elementAt(targetArrayIndex++) = leftHalf[leftHalfIndex++];
    }
    while(rightHalfIndex < rightHalfLength) {
        targetArray.elementAt(targetArrayIndex++) = rightHalf[rightHalfIndex++];
    }

    delete [] leftHalf;
    delete [] rightHalf;
    return inversionsCount;
}

template<typename T>
int Algorithms::sortGetInversionsCount(IGenericArray<T> &targetArray, int beginIndex, int endIndex) {
    if(beginIndex >= endIndex) {
        return 0;
    }

    int middleIndex = (beginIndex + endIndex) / 2;
    int leftInversionsCount = sortGetInversionsCount(targetArray, beginIndex, middleIndex);
    int rightInversionsCount = sortGetInversionsCount(targetArray, middleIndex + 1, endIndex);
    int splitInversionsCount = mergeCountInversions(targetArray, beginIndex, middleIndex, endIndex);
    return leftInversionsCount + rightInversionsCount + splitInversionsCount;
}

template<typename T>
int Algorithms::getInversionsCount(const IGenericArray<T> &targetArray, int beginIndex, int endIndex) {
    IGenericArray<T> *copiedArray = targetArray.copy();
    int inversionsCount = sortGetInversionsCount(*copiedArray, beginIndex, endIndex);
    delete copiedArray;
    return inversionsCount;
}

template<typename T>
void Algorithms::approachElement(IGenericArray<T> &targetArray, int approachingPosition, int targetPosition) {
    for(int i = approachingPosition; i < targetPosition; i++) {
        std::swap(targetArray.elementAt(i), targetArray.elementAt(i + 1));
    }
}

inline int Algorithms::pickRandom(const QList<int> &array) {
    return array.at(QRandomGenerator::global()->bounded(0, array.size()));
}

inline int Algorithms::transformToArrayIndex(const Point &position, int columnLength) {
    return position.x * columnLength + position.y;
}

inline void Algorithms::transformToMatrixIndex(int arrayIndex, Point &outPosition, int columnLength) {
    if(columnLength == 0) {
        throw std::runtime_error{"Division by zero occurred in Algorithms::transformToMatrixIndex()"};
    }
    outPosition.x = arrayIndex / columnLength;
    outPosition.y = arrayIndex % columnLength;
}