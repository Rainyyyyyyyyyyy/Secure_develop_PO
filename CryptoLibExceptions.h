#ifndef CRYPTOLIBEXCEPTIONS_H
#define CRYPTOLIBEXCEPTIONS_H


#endif // CRYPTOLIBEXCEPTIONS_H \
//
   // из std
#include "IExceptions.h"

#define EXCEPTION_FILE_NOT_FOUND 1001
#define EXCEPTION_UNABLE_TO_OPEN_FILE 1002
#define EXCEPTION_UNABLE_TO_CREATE_FILE 1003
#define EXCEPTION_UNABLE_TO_WRITE_ENCRYPTEDTEXT_TO_FILE 1004
#define EXCEPTION_UNABLE_TO_WRITE_FINAL_ENCRYPTED_DATA_TO_FILE 1005
#define EXCEPTION_UNABLE_TO_WRITE_DECRYPTEDTEXT_TO_FILE 1006
#define EXCEPTION_UNABLE_TO_READ_SALT_FROM_FILE 1007

#define EXCEPTION_OPENSSL_PKCS5_PBKDF2_HMAC 2001
#define EXCEPTION_OPENSSL_RAND_bytes 2002   // ошибка RAND_bytes() (генерация соли) openssl)
#define EXCEPTION_OPENSSL_UNABLE_TO_WRITE_SALT 2003
#define EXCEPTION_OPENSSL_EVP_CIPHER_CTX_new 2004 // ошибка в создании контекста шифрования (openssl)
#define EXCEPTION_OPENSSL_EVP_EncryptInit_ex 2005 // ошибка в инициализации (openssl)
#define EXCEPTION_OPENSSL_EVP_EncryptUpdate 2006 // ошибка в шифровании блока (openssl)
#define EXCEPTION_OPENSSL_EVP_EncryptFinal_ex 2007 // ошибка завершения шифрования (последний блок) (openssl)
#define EXCEPTION_OPENSSL_EVP_DecryptInit_ex 2008 // // ошибка в инициализации (openssl)
#define EXCEPTION_OPENSSL_EVP_DecryptUpdate 2009 // ошибка в дешифровании блока (openssl)
#define EXCEPTION_OPENSSL_EVP_DecryptFinal_ex 2010 // ошибка завершения дешифрования (последний блок) (openssl)

#define EXCEPTION_FILE_TOO_SMALL_TO_BE_DECRYPTED 3001 // файл слишком мал, для наличия сигнатуры для дешифрования


// QString.toUtf8() -> QByteArray \
    // QByteArray.constData -> cosnt char*





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

/* Исключение: ошибка при записи в файл зашифрованных данных  */
class ExceptionUnableToWriteEncryptedTextToFile : public Exceptions {
public:
    ExceptionUnableToWriteEncryptedTextToFile() : Exceptions(EXCEPTION_UNABLE_TO_WRITE_ENCRYPTEDTEXT_TO_FILE) {}

    const char *what() const noexcept override {
        return "Unable to write encrypted text to file!";
    }
};

/* Исключение: ошибка при записи в файл дешифрованных данных */
class ExceptionUnableToWriteDecryptedTextToFile : public Exceptions {
public:
    ExceptionUnableToWriteDecryptedTextToFile() : Exceptions(EXCEPTION_UNABLE_TO_WRITE_DECRYPTEDTEXT_TO_FILE) {}

    const char *what() const noexcept override {
        return "Unable to write decrypted text to file!";
    }
};

/* Исключение: невозможно дописать последний блок шифрования в файл */
class ExceptionUnableToWriteFinalDataToFile : public Exceptions {
public:
    ExceptionUnableToWriteFinalDataToFile() : Exceptions(EXCEPTION_UNABLE_TO_WRITE_FINAL_ENCRYPTED_DATA_TO_FILE) {}

    const char *what() const noexcept override {
        return "Unable to write final data to file!";
    }
};

/* Исключение: невозможно прочитать сигнатуру (соль) из файла для дешифрования*/
class ExceptionUnableToReadSalt : public Exceptions {
public:
    ExceptionUnableToReadSalt() : Exceptions(EXCEPTION_UNABLE_TO_READ_SALT_FROM_FILE) {}

    const char *what() const noexcept override {
        return "Unable to read salt from file!";
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

/* Исключение: ошибка функции EVP_EncryptInit_ex()  библиотеки OpenSSL */
class ExceptionOpensslDecryptInit : public Exceptions {
public:
    ExceptionOpensslDecryptInit() : Exceptions(EXCEPTION_OPENSSL_EVP_DecryptInit_ex) {}

    const char *what() const noexcept override {
        return "Error: EVP_DecryptInit_ex() completed with problem!";
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

/* Исключение: ошибка функции EVP_EncryptUpdate()  библиотеки OpenSSL */
class ExceptionOpensslDecryptUpdate : public Exceptions {
public:
    ExceptionOpensslDecryptUpdate() : Exceptions(EXCEPTION_OPENSSL_EVP_DecryptUpdate) {}

    const char *what() const noexcept override {
        return "Error: EVP_DecryptUpdate() completed with problem!";
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

/* Исключение: ошибка функции EVP_EncryptFinal_ex()  библиотеки OpenSSL */
class ExceptionOpensslDecryptFinal : public Exceptions {
public:
    ExceptionOpensslDecryptFinal() : Exceptions(EXCEPTION_OPENSSL_EVP_DecryptFinal_ex) {}

    const char *what() const noexcept override {
        return "Error: EVP_DecryptFinal_ex() compelted with problem!";
    }
};

/* Исключение: ошибка функции RAND_bytes()  библиотеки OpenSSL */
class ExceptionOpensslRandbytes : public Exceptions {
public:
    ExceptionOpensslRandbytes() : Exceptions(EXCEPTION_OPENSSL_RAND_bytes) {}

    const char *what() const noexcept override {
        return "Error: RAND_bytes() completed with problem!";
    }
};

/* Исключение: файл может быть повреждён, файл слишком мал, для наличия сигнатуры (соли) для дешфирования */
class ExceptionFileTooSmallToDecrypt : public Exceptions {
public:
    ExceptionFileTooSmallToDecrypt() : Exceptions(EXCEPTION_FILE_TOO_SMALL_TO_BE_DECRYPTED) {}

    const char *what() const noexcept override {
        return "File might be corrupted: too small to be encrypted!";
    }
};


