#include "CryptoLib.h"

QTextStream CryptoActions::input = QTextStream(stdin);
QTextStream CryptoActions::output = QTextStream(stdout);



bool CryptoActions::Encrypt_File(const QString &filePath, const QString &password){
    output<<"Encryption Function has called.\n";
    output.flush();

    return false;
}
bool CryptoActions::Decrypt_File(const QString &filePath, const QString &password){
    output<<"Decryption Function has called.\n";
    output.flush();

    return false;
}
/*CryptoActions &CryptoActions::Get_Object(){
    //static CryptoActions Object;
    //return Object;
}
QByteArray MakeKeyFromPassword(const QString &password){

    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
}

*/
/*
bool EncryptFile(const QString &filePath, const QString &password){
    {
        output<<"Encryption...\n";
        output.flush();
    }
}

*/
