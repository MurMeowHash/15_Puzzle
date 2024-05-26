#include "FileException.h"

FileException::FileException(const std::string &message, const std::string &detail) {
    msg = message + " : " + detail;
}

const char *FileException::what() const noexcept {
    return msg.data();
}
