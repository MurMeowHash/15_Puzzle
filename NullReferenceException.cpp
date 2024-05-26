#include "NullReferenceException.h"

NullReferenceException::NullReferenceException(const char *message) : message{message} {

}

const char *NullReferenceException::what() const noexcept {
    return message;
}
