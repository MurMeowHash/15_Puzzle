#include <exception>
#include "utils.h"

class NullReferenceException : public std::exception {
private:
    const char *message;
public:
    explicit NullReferenceException(const char *message);
    NODISCARD const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};