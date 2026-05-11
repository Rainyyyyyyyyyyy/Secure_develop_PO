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

    /* === P0 === */
    /* testcase 0.1 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    //QString password_test = "123456789012345678901234567890123";
    //QString password_test = "123";
    QString password_test = "1234567890";
    QString path_test = "../Tests/P1/1";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 0.2 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P0/0.2/file.txt";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 0.3 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P0/0.3";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 0.4 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test;
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 0.5 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "..";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P1 === */
    /* testcase 1 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P1/1";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 2 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P1/2";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 3 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P1/3";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
    /* testcase 4 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P1/4";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P2 === */
    /* testcase 5 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P2/5";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */ /* testcase 6 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P2/6";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P3 === */
    /* testcase 7 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P3/7";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */    /* testcase 8 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P3/8";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P4 === */
    /* testcase 9 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P4/9";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */    /* testcase 10 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P4/10";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P5 === */
    /* testcase 11 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P5/11";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */    /* testcase 12 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P5/12";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P6 === */
    /* testcase 13 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P6/13";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */    /* testcase 14 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P6/14";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */

    /* === P7 === */
    /* testcase 15 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P7/15";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */    /* testcase 16 */ /*
    CryptoActionsAES &crypto_test =  CryptoActionsAES::Instance();
    QString password_test = "password";
    QString path_test = "../Tests/P7/16";
    crypto_test.Encrypt_Folder(path_test, password_test);
    */
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
        qDebug()<<"Confirm? (press Enter or write '.reset'): "<<Qt::endl;
        current_UI_action = input.readLine();
        if(current_UI_action == ".exit"){
            exitMessage();
            return 0;
        }
        if(current_UI_action != "" && current_UI_action != ".reset")current_UI_action=".reset";
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
        qDebug()<<"Confirm? (press Enter or write '.reset'): ";
        current_UI_action = input.readLine();
        if(current_UI_action == ".exit"){
            exitMessage();
            return 0;
        }
        if(current_UI_action != "" && current_UI_action!= ".reset")current_UI_action = ".reset";
    }while(current_UI_action == ".reset");

    if(mode == ".encrypt"){
            try{
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

    qDebug()<<"Program has been completed.";
    return 0;


}



