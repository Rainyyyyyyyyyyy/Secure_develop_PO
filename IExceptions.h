#ifndef IEXCEPTIONS_H
#define IEXCEPTIONS_H

#include <exception>    // из std
#include <string>

class CustomExceptions : public std::exception{
protected:
    unsigned int errorCode;           // код ошибки\исключения
    std::string message;    // описание

public:
    CustomExceptions(unsigned int code__, const char * message__) : errorCode(code__), message(message__) {};

    const char *what() const noexcept override{ return message.c_str(); }

    // getter кода ошибки/исключения
    unsigned int getCode() const { return errorCode; }
};


#endif // IEXCEPTIONS_H
