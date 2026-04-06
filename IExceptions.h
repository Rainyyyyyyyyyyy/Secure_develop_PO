#ifndef IEXCEPTIONS_H
#define IEXCEPTIONS_H

#include <exception>    // из std

class Exceptions : public std::exception{
protected:
    int errorCode;           // код ошибки\исключения

public:
    // what() будет переопределяться в каждом наследнике
    virtual const char *what() const noexcept = 0;
    // деструктор
    virtual ~Exceptions() = default;
    // конструктор по умолчанию
    Exceptions() : errorCode(0) {}
    // конструктор по значению
    Exceptions(int code) : errorCode(code) {}

    // getter кода ошибки/исключения
    int getCode() const { return errorCode; }
};



#endif // IEXCEPTIONS_H
