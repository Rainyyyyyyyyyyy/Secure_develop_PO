#include "CryptoLib.h"

QTextStream CryptoActions::input = QTextStream(stdin);
QTextStream CryptoActions::output = QTextStream(stdout);



bool CryptoActions::EncryptFile(const QString &filePath, const QString &password){
    output<<"ASD";
}
bool CryptoActions::DecryptFile(const QString &filePath, const QString &password){
    output<<"ASASDA";
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
