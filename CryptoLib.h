#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#endif // CRYPTOLIB_H

#include "InterfaceCryptoLib.h"


#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>


#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
//#include <QCryptographicHash>

// class singleton для шифрования и дешифрования файлов
class CryptoActions: public InterfaceCryptoActions {

private:
    // для вывода
    static QTextStream input;
    static QTextStream output;
// =    =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =


    // запрет на копирование
    CryptoActions(const CryptoActions&) = delete;
    CryptoActions() {}    // конструктор без реализации
    ~CryptoActions() {}    // и деструктор
    CryptoActions& operator=(CryptoActions const&); // и присваивание

public:

    // функция-getter объекта класса (ссылка & на объект)
    static CryptoActions &Instance(){
        static CryptoActions s;
        return s;
    }


    bool Encrypt_File(const QString &filePath, const QString &password) override;
    bool Decrypt_File(const QString &filePath, const QString &password) override;
// =    =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
};

// из ключа получить хэш (SHA-256)
QByteArray MakeKeyFromPassword(const QString &password);
