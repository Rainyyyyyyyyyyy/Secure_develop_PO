#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#endif // CRYPTOLIB_H

#include "InterfaceCryptoLib.h"


#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>

// class singleton для шифрования и дешифрования файлов
class CryptoActions: public InterfaceCryptoActions {

private:
    CryptoActions();
    ~CryptoActions();


    // для вывода
    static QTextStream input;
    static QTextStream output;


public:

    // функция-getter объекта класса (ссылка & на объект)
    static CryptoActions &Instance(){
        static CryptoActions s;
        return s;
    }

    CryptoActions(const CryptoActions&) = delete;

    bool EncryptFile(const QString &filePath, const QString &password) override;
    /*{
        output<<"ASD";
    }*/
    bool DecryptFile(const QString &filePath, const QString &password) override;
    /*{
        output<<"ASASDA";
    }*/
};

// из ключа получить хэш (SHA-256)
QByteArray MakeKeyFromPassword(const QString &password);
