#ifndef ERRORS_HANDLER_H
#define ERRORS_HANDLER_H

// identificar erro, linha e tipo do erro (lexico, sintatico, semantico)
#include <iostream>
#include <exception>
#include <string>
using namespace std;

class AssemblerError : public exception {
    private:
        string message;
        string lexic = "Erro léxico: ";
    public:
        explicit AssemblerError(const string& msg);
        
        const char* what() const noexcept override;
};

#endif