#pragma once
#include <exception>
#include <string>
#include "utils.h"

class FileException : public std::exception {
private:
    std::string msg;
public:
    FileException(const std::string &message, const std::string &detail);
    NODISCARD const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};