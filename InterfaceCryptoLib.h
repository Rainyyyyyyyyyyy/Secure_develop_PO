#ifndef INTERFACECRYPTOLIB_H
#define INTERFACECRYPTOLIB_H

#endif // INTERFACECRYPTOLIB_H

#include <QString>
#include <QTextStream>

class InterfaceCryptoActions {
private:

public:

    // функция шифрования для файла (по паролю)
    virtual bool Encrypt_File(const QString &filePath, const QString &password) = 0;
    // функция шифрования для файла (по готовому хешу из пароля)
    //virtual bool EncryptFile(const QString &filePath, const QByteArray &hash) = 0;

    // функция дешифрования для файла (по паролю)
    virtual bool Decrypt_File(const QString &filePath, const QString &password) = 0;
    // функция дешифрования для файла (по готовому хешу из пароля)
    //virtual bool DecryptFile(const QString &filePath, const QByteArray &hash) = 0;


    virtual ~InterfaceCryptoActions() = default;
};
