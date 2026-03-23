#include <QCoreApplication>
#include <QDir>
#include <QDebug>
//#include <QTextStream>
#include <windows.h>

#include <QVector>

// Присоединилась openssl!!!
#include <openssl/crypto.h>


#include "FolderTraveler.h"

QTextStream input(stdin);
QTextStream output(stdout);


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // вывод версии openssl
    output<<OPENSSL_VERSION_TEXT<<'\n';


    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);


    output << "Enter path to folder: ";
    output.flush();
    // H:\Documents\Secure_tools_AndreevaVV\TRUE_REPO
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try3_gitclone
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try3_gitclone\Libraries\cryptopp(defeat)
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try7_gitclone\papki

    QString folderPath;
    folderPath = input.readLine().trimmed();

    output << "\nContent of  " << folderPath << ":\n";
    output << "====================================\n";
    output.flush();

    FolderTraveler Folderr(folderPath);
    Folderr.TravelFolder();
    Folderr.OutputList();




    return 0;
}
