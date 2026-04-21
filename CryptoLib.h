#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#endif // CRYPTOLIB_H

#include "InterfaceCryptoLib.h"
#include "CryptoLibExceptions.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QCryptographicHash>


#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
//#include <QCryptographicHash>






const int KEY_LEN = 32;      // 256 бит для AES-256
const int IV_LEN = 16;       // 16 байт для AES
const int SALT_LEN = 16;     // 16 байт соли
const int ITERATIONS = 10000; // 10000 итераций PBKDF2
const int SIGN_LEN = 16;    // 16 байт для сигнатуры
const int HASH_LEN = 32;     // 32 байт для хеша ключа (SHA-256)
const QByteArray Signature_Sequence = "A66B06F945C9B57E";


// class singleton для шифрования и дешифрования файлов
class CryptoActionsAES: public InterfaceCryptoActions {

private:





    bool IsFileEncrypted(const QString &filePath);

    // hash функция
    QByteArray hash_from_key(const QString &data);

    // сравнить хеш полученный и хеш из password
    bool checkHashPassword(const QByteArray &gotHash, const QString &password);
    // сравнить хеш полученный и хеш из password
    //bool checkHashPassword(const QByteArray &gotHash, const char *password, const unsigned int &n);
    bool checkHashPassword(const unsigned char gotHash[HASH_LEN], const QString &password);
    //bool checkHashPassword(const char *gotHash, const QString &password);

// =    =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =


    // запрет на копирование
    CryptoActionsAES(const CryptoActionsAES&) = delete;
    CryptoActionsAES() {}    // конструктор без реализации
    ~CryptoActionsAES() {}    // и деструктор
    CryptoActionsAES& operator=(CryptoActionsAES const&); // и присваивание

public:

    // функция-getter объекта класса (ссылка & на объект)
    static CryptoActionsAES &Instance(){
        static CryptoActionsAES s;
        return s;
    }


    bool Encrypt_File(const QString &filePath, const QString &password) override;
    bool Decrypt_File(const QString &filePath, const QString &password) override;



// =    =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
};

// из ключа получить хэш (SHA-256)
//QByteArray MakeKeyFromPassword(const QString &password);

// сигнатура для определения "зашифрован \ не зашифрован"
// если этой сигнатуры нет, то предполагается - "не зашифрован"
// иначе - "зашифрован"


