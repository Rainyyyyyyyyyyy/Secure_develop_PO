#ifndef CRYPTOLIBEXCEPTIONS_H
#define CRYPTOLIBEXCEPTIONS_H


#endif // CRYPTOLIBEXCEPTIONS_H \
//

#include "IExceptions.h"


#define EXCEPTION_FILE_NOT_FOUND 1001   // файл не найден
#define EXCEPTION_UNABLE_TO_OPEN_FILE 1002  // не удалось открыть файл
#define EXCEPTION_UNABLE_TO_CREATE_FILE 1003    // не удалось создать файл
#define EXCEPTION_UNABLE_TO_WRITE_ENCRYPTEDTEXT_TO_FILE 1004    // не удалось записать шифртекст в файл
#define EXCEPTION_UNABLE_TO_WRITE_DECRYPTEDTEXT_TO_FILE 1006    // не удалось записать дешифрованный текст в файл
#define EXCEPTION_UNABLE_TO_READ_SIGN_FROM_FILE 1008    // не удалось прочитать сигнатуру
#define EXCEPTION_FILE_IS_EMPTY 1009    // файл пуст

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

#define EXCEPTION_FILE_TOO_SMALL 3001 // файл слишком мал, для наличия сигнатуры для дешифрования
#define EXCEPTION_FILE_IS_ALREADY_ENCRYPTED 3002    // файл уже зашифрован
#define EXCEPTION_FILE_IS_ALREADY_DECRYPTED 3003    // файл уже дешифрован
#define EXCEPTION_INCORRECT_PASSWORD 3004       // введён неверный пароль ( для дешифрования )



 /* Исключение: файл не найден */
class ExceptionFileNotFound : public CustomExceptions {
public:
    ExceptionFileNotFound() : CustomExceptions(EXCEPTION_FILE_NOT_FOUND, "File not found!") {}
};

/* Исключение: невозможно открыть файл */
class ExceptionUnableToOpenFile : public CustomExceptions {
public:
    ExceptionUnableToOpenFile() : CustomExceptions(EXCEPTION_UNABLE_TO_OPEN_FILE, "Unable to open file!") {}
};

/* Исключение: невозможно создать файл */
class ExceptionUnableToCreateFile : public CustomExceptions {
public:
    ExceptionUnableToCreateFile() : CustomExceptions(EXCEPTION_UNABLE_TO_CREATE_FILE, "Unable to create file!") {}
};

/* Исключение: ошибка при записи в файл зашифрованных данных  */
class ExceptionUnableToWriteEncryptedTextToFile : public CustomExceptions {
public:
    ExceptionUnableToWriteEncryptedTextToFile() : CustomExceptions(EXCEPTION_UNABLE_TO_WRITE_ENCRYPTEDTEXT_TO_FILE, "Unable to write encrypted text to file!") {}
};

/* Исключение: ошибка при записи в файл дешифрованных данных */
class ExceptionUnableToWriteDecryptedTextToFile : public CustomExceptions {
public:
    ExceptionUnableToWriteDecryptedTextToFile() : CustomExceptions(EXCEPTION_UNABLE_TO_WRITE_DECRYPTEDTEXT_TO_FILE, "Inable to write decrypted text to file!") {}
};


/*
 *   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
 *  =   =   =   =  Исключения от OpenSSL функций  =   =   =   =
 *  =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
*/


/* Исключение: ошибка в функции PKCS5_PBKDF2_HMAC() библиотеки OpenSSL */
class ExceptionOpensslHMAC : public CustomExceptions {
public:
    ExceptionOpensslHMAC() : CustomExceptions(EXCEPTION_OPENSSL_PKCS5_PBKDF2_HMAC, "Error: PKCS5_PBKDF2_HMAC() completed with problem!") {}
};

/* Исключение: невозможно дописать "соль" в файл */
class ExceptionOpensslWriteSaltToFile : public CustomExceptions {
public:
    ExceptionOpensslWriteSaltToFile() : CustomExceptions(EXCEPTION_OPENSSL_UNABLE_TO_WRITE_SALT, "Unable ti write salt to file!") {}
};

/* Исключение: ошибка функции  EVP_CIPHER_CTX_new() библиотеки OpenSSL */
class ExceptionOpensslCipherCTXnew : public CustomExceptions {
public:
    ExceptionOpensslCipherCTXnew() : CustomExceptions(EXCEPTION_OPENSSL_EVP_CIPHER_CTX_new, "Error: EVP_CIPHER_CTX_new() completed with problem!") {}
};

/* Исключение: ошибка функции EVP_EncryptInit_ex()  библиотеки OpenSSL */
class ExceptionOpensslEncryptInit : public CustomExceptions {
public:
    ExceptionOpensslEncryptInit() : CustomExceptions(EXCEPTION_OPENSSL_EVP_EncryptInit_ex, "Error: EVP_EncryptInit_ex() completed with problem!") {}
};

/* Исключение: ошибка функции EVP_EncryptInit_ex()  библиотеки OpenSSL */
class ExceptionOpensslDecryptInit : public CustomExceptions {
public:
    ExceptionOpensslDecryptInit() : CustomExceptions(EXCEPTION_OPENSSL_EVP_DecryptInit_ex, "Error: EVP_DecryptInit_ex() completed with problem") {}
};

/* Исключение: ошибка функции EVP_EncryptUpdate()  библиотеки OpenSSL */
class ExceptionOpensslEncryptUpdate : public CustomExceptions {
public:
    ExceptionOpensslEncryptUpdate() : CustomExceptions(EXCEPTION_OPENSSL_EVP_EncryptUpdate, "Error: EVP_EncryptUpdate() completed with problem") {}
};

/* Исключение: ошибка функции EVP_EncryptUpdate()  библиотеки OpenSSL */
class ExceptionOpensslDecryptUpdate : public CustomExceptions {
public:
    ExceptionOpensslDecryptUpdate() : CustomExceptions(EXCEPTION_OPENSSL_EVP_DecryptUpdate, "Error: EVP_DecryptUpdate() completed with problem!") {}
};

/* Исключение: ошибка функции EVP_EncryptFinal_ex()  библиотеки OpenSSL */
class ExceptionOpensslEncryptFinal : public CustomExceptions {
public:
    ExceptionOpensslEncryptFinal() : CustomExceptions(EXCEPTION_OPENSSL_EVP_EncryptFinal_ex, "Error: EVP_EncryptFinal_ex() completed with problem!") {}
};

/* Исключение: ошибка функции EVP_EncryptFinal_ex()  библиотеки OpenSSL */
class ExceptionOpensslDecryptFinal : public CustomExceptions {
public:
    ExceptionOpensslDecryptFinal() : CustomExceptions(EXCEPTION_OPENSSL_EVP_DecryptFinal_ex, "Error: EVP_DecryptFinal_ex() completed with problem!") {}
};

/* Исключение: ошибка функции RAND_bytes()  библиотеки OpenSSL */
class ExceptionOpensslRandbytes : public CustomExceptions {
public:
    ExceptionOpensslRandbytes() : CustomExceptions(EXCEPTION_OPENSSL_RAND_bytes, "Error: RAND_bytes() completed with problem!") {}
};

/* Исключение: файл может быть повреждён, файл слишком мал, для наличия сигнатуры (соли) для дешфирования */
class ExceptionFileTooSmall : public CustomExceptions {
public:
    ExceptionFileTooSmall() : CustomExceptions(EXCEPTION_FILE_TOO_SMALL, "File might be corrupted: file is too small!") {}
};


/*
 *   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
 *  =   =   =   =   Исключения во время работы функций         =
 *  =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
*/



/* Исключение: файл уже зашифрован */
class ExceptionFileIsAlreadyEncrypted : public CustomExceptions {
public:
    ExceptionFileIsAlreadyEncrypted() : CustomExceptions(EXCEPTION_FILE_IS_ALREADY_ENCRYPTED, "File is already encrypted") {}
};

/* Исключение: файл уже дешифрован */
class ExceptionFileIsAlreadyDecrypted : public CustomExceptions {
public:
    ExceptionFileIsAlreadyDecrypted() : CustomExceptions(EXCEPTION_FILE_IS_ALREADY_DECRYPTED, "File is already decrypted!") {}
};


/* Исключение: невозможно прочитать сигнатуру */
class ExceptionUnableToReadSign : public CustomExceptions {
public:
    ExceptionUnableToReadSign() : CustomExceptions(EXCEPTION_UNABLE_TO_READ_SIGN_FROM_FILE, "Unable to read signature from file!") {}
};

/* Исключение: неверный пароль (для дешифрования) */
class ExceptionIncorrectPassword : public CustomExceptions {
public:
    ExceptionIncorrectPassword() : CustomExceptions(EXCEPTION_INCORRECT_PASSWORD, "Incorrect password!") {}
};

/* Исключение: файл пуст */
class ExceptionFileIsEmpty : public CustomExceptions {
public:
    ExceptionFileIsEmpty() : CustomExceptions(EXCEPTION_FILE_IS_EMPTY, "Warning: file is empty!") {}
};

