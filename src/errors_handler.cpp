#include "errors_handler.h"

AssemblerError::AssemblerError(const string& msg): message(msg) {}


const char* AssemblerError::what() const noexcept {
    return message.c_str();
}