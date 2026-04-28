#ifndef INTERFACECRYPTOLIB_H
#define INTERFACECRYPTOLIB_H

#endif // INTERFACECRYPTOLIB_H

#include <QString>

class InterfaceCryptoActions {
private:


    // функция шифрования для файла (по паролю)
    virtual bool Encrypt_File(const QString &filePath, const QString &password) = 0;
    // функция шифрования для файла (по готовому хешу из пароля)

    // функция дешифрования для файла (по паролю)
    virtual bool Decrypt_File(const QString &filePath, const QString &password) = 0;
    // функция дешифрования для файла (по готовому хешу из пароля)

public:

    virtual bool Encrypt_Folder(const QString &folderPath, const QString &password) = 0;

    virtual bool Decrypt_Folder(const QString &folderPath, const QString &password) = 0;

    virtual ~InterfaceCryptoActions() = default;
};
