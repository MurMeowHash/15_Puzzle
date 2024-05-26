#pragma once
#include <cstdio>
#include <string>
#define NODISCARD [[nodiscard]]
#define GENERIC_PTR void*

void notifyManagerStarting(const char *manager);
void handleNullReferences(const char *message, int count, ...);
void checkIOCorruptions(int actualElemCount, int expectedElemCount);
void checkOpeningCorruptions(FILE *targetStream, const std::string &fileName);