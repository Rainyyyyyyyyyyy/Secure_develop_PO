#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#endif // CRYPTOLIB_H

#include <InterfaceCryptoLib.h>


#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>

// class singleton для шифрования и дешифрования файлов
class CryptoActions final : public InterfaceCryptoActions {

private:
    CryptoActions() = default;
    ~CryptoActions() = default;

public:

    // функция-getter объекта класса (ссылка & на объект)
    static CryptoActions &Get_Object();

    bool EncryptFile(const QString &filePath, const QString &password) override;

    bool DecryptFile(const QString &filePath, const QString &password) override;
};

// из ключа получить хэш (SHA-256)
QByteArray MakeKeyFromPassword(const QString &password);
