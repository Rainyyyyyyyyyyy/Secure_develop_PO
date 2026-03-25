#include <QCoreApplication>
#include <QDir>
#include <QDebug>
//#include <QTextStream>
#include <windows.h>
#include <vector>

#include <QVector>

// Присоединилась openssl!!!
#include <openssl/crypto.h>
#include <openssl/rand.h>

#include "FolderTraveler.h"
#include "CryptoLib.h"

QTextStream input(stdin);
QTextStream output(stdout);


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    /* // проверка работы openssl (например openssl/rand.h и необходимой для неё libeay32.dll
     * // ...\Qt\Tools\mingw810_64\opt\bin\libeay32.dll
    unsigned char chara[4] = {0,0,0,0};
    RAND_bytes(chara, 4);
    for(int i=0; i<4; i++)output<<chara[i]<<' ';
    output.flush();
    input.readLine();
    return 0;
*/

    // вывод версии openssl
    output<<OPENSSL_VERSION_TEXT<<'\n';
    output.flush();

    // для вывода кириллицы
    SetConsoleCP(12051);
    SetConsoleOutputCP(12051);


    //output << "Enter path to folder: ";
    //output.flush();
    // примеры
    // H:\Documents\Secure_tools_AndreevaVV\TRUE_REPO
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try3_gitclone
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try3_gitclone\Libraries\cryptopp(defeat)
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try7_gitclone\papki

    // Проверка прохода папки
    /*QString folderPath;
    folderPath = input.readLine().trimmed();

    output << "\nContent of  " << folderPath << ":\n";
    output << "====================================\n";
    output.flush();

    FolderTraveler Folderr(folderPath);
    Folderr.TravelFolder();
    Folderr.OutputList();
*/

    output<<"Enter path to file: ";
    output.flush();
    QString Path_to_encrypt_file = input.readLine();
    output<<"Enter password to encrypt: ";
    output.flush();
    QString Password_to_encrypt = input.readLine();
    CryptoActions &cry = CryptoActions::Instance();


    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try7_gitclone\papki\asd.txt
    cry.Encrypt_File(Path_to_encrypt_file, Password_to_encrypt);
    cry.Decrypt_File(Path_to_encrypt_file + ".enc", Password_to_encrypt);

    //CryptoActions::Instance().Encrypt_File("abc","abckey");




    return 0;
}
