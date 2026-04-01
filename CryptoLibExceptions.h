#ifndef CRYPTOLIBEXCEPTIONS_H
#define CRYPTOLIBEXCEPTIONS_H


#endif // CRYPTOLIBEXCEPTIONS_H \
//
#include <exception>    // из std
#include <QString>

#define EXCEPTION_FILE_NOT_FOUND 1001
#define EXCEPTION_UNABLE_TO_OPEN_FILE 1002
#define EXCEPTION_UNABLE_TO_CREATE_FILE 1003
#define EXCEPTION_UNABLE_TO_WRITE_ENCRYPTEDTEXT_TO_FILE 1004
#define EXCEPTION_OPENSSL_PKCS5_PBKDF2_HMAC 2001
#define EXCEPTION_OPENSSL_UNABLE_TO_WRITE_SALT 2002
#define EXCEPTION_OPENSSL_EVP_CIPHER_CTX_new 2003 // ошибка в создании контекста шифрования (openssl)
#define EXCEPTION_OPENSSL_EVP_EncryptInit_ex 2004 // ошибка в инициализации (openssl)
#define EXCEPTION_OPENSSL_EVP_EncryptUpdate 2005 // ошибка в шифровании блока (openssl)
#define EXCEPTION_OPENSSL_EVP_EncryptFinal_ex 2006 // ошибка завершения шифрования (последний блок) (openssl)



    // QString.toUtf8() -> QByteArray \
    // QByteArray.constData -> cosnt char*


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



 /* Исключение: файл не найден */
class ExceptionFileNotFound : public Exceptions {
public:
    ExceptionFileNotFound() : Exceptions(EXCEPTION_FILE_NOT_FOUND) {}

    const char *what() const noexcept override {
        return "File not Found!";
    }
};

/* Исключение: невозможно открыть файл */
class ExceptionUnableToOpenFile : public Exceptions {
public:
    ExceptionUnableToOpenFile() : Exceptions(EXCEPTION_UNABLE_TO_OPEN_FILE) {}

    const char *what() const noexcept override {
        return "Unable to open file!";
    }
};

/* Исключение: невозможно создать файл */
class ExceptionUnableToCreateFile : public Exceptions {
public:
    ExceptionUnableToCreateFile() : Exceptions(EXCEPTION_UNABLE_TO_CREATE_FILE) {}

    const char *what() const noexcept override {
        return "Unable to create file!";
    }
};

/* Исключение: ошибка при записи в файл */
class ExceptionUnableToWriteEncryptedTextToFile : public Exceptions {
public:
    ExceptionUnableToWriteEncryptedTextToFile() : Exceptions(EXCEPTION_UNABLE_TO_WRITE_ENCRYPTEDTEXT_TO_FILE) {}

    const char *what() const noexcept override {
        return "Unable to write encrypted text to file!";
    }
};

/*
 *   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
 *  =   =   =   =  Исключения от OpenSSL функций  =   =   =   =
 *  =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
*/
/* Исключение: ошибка в функции PKCS5_PBKDF2_HMAC() библиотеки OpenSSL */
class ExceptionOpensslHMAC : public Exceptions {
public:
    ExceptionOpensslHMAC() : Exceptions(EXCEPTION_OPENSSL_PKCS5_PBKDF2_HMAC) {}

    const char *what() const noexcept override {
        return "Error: PKCS5_PBKDF2_HMAC() completed with problem!";
    }
};

/* Исключение: невозможно дописать "соль" в файл */
class ExceptionOpensslWriteSaltToFile : public Exceptions {
public:
    ExceptionOpensslWriteSaltToFile() : Exceptions(EXCEPTION_OPENSSL_UNABLE_TO_WRITE_SALT) {}

    const char *what() const noexcept override {
        return "Unable to write salt to file!";
    }
};

/* Исключение: ошибка функции  EVP_CIPHER_CTX_new() библиотеки OpenSSL */
class ExceptionOpensslCipherCTXnew : public Exceptions {
public:
    ExceptionOpensslCipherCTXnew() : Exceptions(EXCEPTION_OPENSSL_EVP_CIPHER_CTX_new) {}

    const char *what() const noexcept override {
        return "Error: EVP_CIPHER_CTX_new() completed with problem!";
    }
};

/* Исключение: ошибка функции EVP_EncryptInit_ex()  библиотеки OpenSSL */
class ExceptionOpensslEncryptInit : public Exceptions {
public:
    ExceptionOpensslEncryptInit() : Exceptions(EXCEPTION_OPENSSL_EVP_EncryptInit_ex) {}

    const char *what() const noexcept override {
        return "Error: EVP_EncryptInit_ex() completed with problem!";
    }
};

/* Исключение: ошибка функции EVP_EncryptUpdate()  библиотеки OpenSSL */
class ExceptionOpensslEncryptUpdate : public Exceptions {
public:
    ExceptionOpensslEncryptUpdate() : Exceptions(EXCEPTION_OPENSSL_EVP_EncryptUpdate) {}

    const char *what() const noexcept override {
        return "Error: EVP_EncryptUpdate() completed with problem!";
    }
};

/* Исключение: ошибка функции EVP_EncryptFinal_ex()  библиотеки OpenSSL */
class ExceptionOpensslEncryptFinal : public Exceptions {
public:
    ExceptionOpensslEncryptFinal() : Exceptions(EXCEPTION_OPENSSL_EVP_EncryptFinal_ex) {}

    const char *what() const noexcept override {
        return "Error: EVP_EncryptFinal_ex() compelted with problem!";
    }
};


