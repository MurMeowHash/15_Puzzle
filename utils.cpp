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

void checkIOCorruptions(int actualElemCount, int expectedElemCount, const std::string &msg) {
    if(actualElemCount != expectedElemCount) {
        throw FileException{msg, "Unexpected number of elements was processed"};
    }
}

void checkOpeningCorruptions(FILE *targetStream, const std::string &msg) {
    if(targetStream == nullptr) {
        throw FileException{msg, "Unable to open file"};
    }
}