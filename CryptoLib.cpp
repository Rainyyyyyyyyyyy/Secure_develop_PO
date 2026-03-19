#include <CryptoLib.h>

CryptoActions &CryptoActions::Get_Object(){
    static CryptoActions Object;
    return Object;
}
QByteArray MakeKeyFromPassword(const QString &password){

    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
}
