#include <iostream>
#include <cstdarg>
#include "utils.h"
#include "NullReferenceException.h"
#include "FileException.h"

void notifyManagerStarting(const char *manager) {
    std::cout<<manager<<" manager is starting...\n";
}

void handleNullReferences(const char *message, int count, ...) {
    va_list argList;
    va_start(argList, count);
    for(int i = 0; i < count; i++) {
        if(va_arg(argList, GENERIC_PTR) == nullptr) {
            throw NullReferenceException{message};
        }
    }
    va_end(argList);
}

void checkIOCorruptions(int actualElemCount, int expectedElemCount) {
    if(actualElemCount != expectedElemCount) {
        throw FileException{"Unexpected number of elements was processed", std::to_string(actualElemCount)};
    }
}

void checkOpeningCorruptions(FILE *targetStream, const std::string &fileName) {
    if(targetStream == nullptr) {
        throw FileException{"Unable to open file", fileName};
    }
}