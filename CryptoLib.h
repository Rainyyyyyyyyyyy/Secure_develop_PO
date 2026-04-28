#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#endif // CRYPTOLIB_H

#include "InterfaceCryptoLib.h"
#include "CryptoLibExceptions.h"







const int KEY_LEN = 32;      // 256 бит для AES-256
const int IV_LEN = 16;       // 16 байт для AES
const int SALT_LEN = 16;     // 16 байт соли
const int ITERATIONS = 10000; // 10000 итераций PBKDF2
const int SIGN_LEN = 16;    // 16 байт для сигнатуры
const int HASH_LEN = 32;     // 32 байт для хеша ключа (SHA-256)
const QByteArray Signature_Sequence = "A66B06F945C9B57E";   // сигнатура в начале файла для предпроверки зашифрованности


// class singleton для шифрования и дешифрования файлов
class CryptoActionsAES: public InterfaceCryptoActions {
private:

    bool IsFileEncrypted(const QString &filePath);




    bool Encrypt_File(const QString &filePath, const QString &password);
    bool Decrypt_File(const QString &filePath, const QString &password);
// =    =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =


    // запрет на копирование
    CryptoActionsAES(const CryptoActionsAES&) = delete;
    CryptoActionsAES() {}    // конструктор без реализации
    ~CryptoActionsAES() {}    // и деструктор
    CryptoActionsAES& operator=(CryptoActionsAES const&) = delete; // и присваивание

public:

    // функция-getter объекта класса (ссылка & на объект)
    static CryptoActionsAES &Instance(){
        static CryptoActionsAES s;
        return s;
    }

    // зашифровать папку
    bool Encrypt_Folder(const QString &folderPath, const QString &password);
    // дешифровать папку
    bool Decrypt_Folder(const QString &folderPath, const QString &password);


};


// сигнатура для определения "зашифрован \ не зашифрован"
// если этой сигнатуры нет, то предполагается - "не зашифрован"
// иначе - "зашифрован"


