#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <windows.h>
#include <vector>
#include <QVector>

#include "CryptoLib.h"          // includes CryptoLibExceptions.h and openssl

#include <iostream>
#include <openssl/opensslv.h>
QTextStream input(stdin);
QTextStream output(stdout);


void exitMessage(){
    qDebug()<<"Program has been completed with '.exit'-word";
}

int main(int argc, char *argv[]) {


    // вывод версии openssl
    output<<OPENSSL_VERSION_TEXT<<'\n';
    output.flush();

    // для вывода кириллицы
    SetConsoleCP(12051);
    SetConsoleOutputCP(12051);


    QCoreApplication qcoreappa(argc, argv); // для вывода текущей директории, отн-но которой ориентируется программа
    qDebug()<<"Current relative path: "<<QCoreApplication::applicationDirPath()<<Qt::endl;

    QString folderPath;
    QString current_UI_action = ".reset";

    qDebug()<<"\n\n==========================\nEnter '.exit' on any stage to exit from program.\n=========================="<<Qt::endl;
    // запрос пути к папке
    do{
        qDebug()<<"Enter path to directory: "<<Qt::flush;
        folderPath = input.readLine();
        QDir checker(folderPath);
        checker.refresh();
        qDebug()<<checker.absolutePath();

        qDebug()<<"Enter action ('.reset', or skip)"<<Qt::endl;
        current_UI_action = input.readLine();
        if(current_UI_action == ".exit"){
            exitMessage();
            return 0;
        }
    }while(current_UI_action == ".reset");


    CryptoActionsAES &cry = CryptoActionsAES::Instance();
    QString mode;
    QString MODES[] = {".encrypt", ".decrypt" };
    QString Password;           // example "password";

    // запрос режима
    do{
        qDebug()<<"Enter mode ('.encrypt', '.decrypt'): ";
        mode = input.readLine();
        if(mode == ".exit"){
            exitMessage();
            return 0;
        }
    }while(mode != ".encrypt" && mode != ".decrypt");

    // запрос пароля
    current_UI_action = ".reset";
    do{
        qDebug()<<"Enter password (8-32 characters): ";
        Password = input.readLine();
        Password.detach();

        qDebug()<<"Password: "<<Password<<Qt::endl;
        qDebug()<<"Enter action('.reset' or skip): ";
        current_UI_action = input.readLine();
        if(current_UI_action == ".exit"){
            exitMessage();
            return 0;
        }
    }while(current_UI_action == ".reset");

    if(mode == ".encrypt"){
            try
            {
            cry.Encrypt_Folder(folderPath, Password);
            } catch(const CustomExceptions &excp){
                qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
            }
        }
        else if(mode == ".decrypt"){
            try
            {
                cry.Decrypt_Folder(folderPath, Password);
            } catch(const CustomExceptions &excp){
                qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
            }
        }


    return 0;


}



