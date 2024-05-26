#include "SearchException.h"

SearchException::SearchException(std::string &&mainPart, std::string &&detail) {
    message = "<" + mainPart + "> : " + detail;
}

const char *SearchException::what() const noexcept {
    return message.data();
}