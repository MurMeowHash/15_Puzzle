#include <exception>
#include <string>
#include "utils.h"

class SearchException : public std::exception {
private:
    std::string message;
public:
    SearchException(std::string &&mainPart, std::string &&detail);
    NODISCARD const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};