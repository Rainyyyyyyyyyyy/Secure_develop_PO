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



int main(int argc, char *argv[]) {


    // вывод версии openssl
    output<<OPENSSL_VERSION_TEXT<<'\n';
    output.flush();

    // для вывода кириллицы
    SetConsoleCP(12051);
    SetConsoleOutputCP(12051);



    QString folderPath;
    QString current_UI_action = ".reset";

    qDebug()<<"\n\n==========================\nEnter '.exit' on any stage to exit from program.\n=========================="<<Qt::endl;
    // запрос пути к папке
    do{
        qDebug()<<"Enter path to directory: "<<Qt::flush;
        folderPath = input.readLine();

        qDebug()<<"Enter action ('.reset', or skip)"<<Qt::endl;
        current_UI_action = input.readLine();
        if(current_UI_action == ".exit"){
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

    //      test:
    //          Disk:\path\to\files\test1
    //          password: "password"

}


// output:
/*
    [Folder]FolderTraveler_tests
        [Folder]papka1
            [File]file1.txt (0 B)
            [File]file2.txt (0 B)
            [File]file3.txt (0 B)
            [Folder]papka4
                [File]file1.txt (17 B)
            [Folder]papka2
                [File]file2.txt.yarl.lnk (0 B)
            [Folder]papka3
    */
// E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1_test_files\Encrypt_Folder_tests





/* Проверка работы класса Exceptions: public std::exception */
/*
    Exceptions *excp;
    // 1001
    excp  = new ExceptionFileNotFound;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 1002
    excp  = new ExceptionUnableToOpenFile;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 1003
    excp  = new ExceptionUnableToCreateFile;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 1004
    excp  = new ExceptionUnableToWriteEncryptedTextToFile;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 1005
    excp  = new ExceptionUnableToWriteFinalDataToFile;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 1006
    excp  = new ExceptionUnableToWriteDecryptedTextToFile;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 1007
    excp  = new ExceptionUnableToReadSalt;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;


    // 2001
    excp  = new ExceptionOpensslHMAC;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2002
    excp = new ExceptionOpensslRandbytes;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2003
    excp  = new ExceptionOpensslWriteSaltToFile;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2004
    excp  = new ExceptionOpensslCipherCTXnew;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2005
    excp  = new ExceptionOpensslEncryptInit;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2006
    excp  = new ExceptionOpensslEncryptUpdate;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2007
    excp  = new ExceptionOpensslEncryptFinal;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2008
    excp  = new ExceptionOpensslDecryptInit;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2009
    excp  = new ExceptionOpensslDecryptUpdate;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    // 2010
    excp  = new ExceptionOpensslDecryptFinal;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;


    // 3001
    excp  = new ExceptionFileTooSmallToDecrypt;
    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
    delete excp;
    */
/* Output:
File not Found!   Code:  1001
Unable to open file!   Code:  1002
Unable to create file!   Code:  1003
Unable to write encrypted text to file!   Code:  1004
Unable to write final data to file!   Code:  1005
Unable to write decrypted text to file!   Code:  1006
Unable to read salt from file!   Code:  1007
Error: PKCS5_PBKDF2_HMAC() completed with problem!   Code:  2001
Error: RAND_bytes() completed with problem!   Code:  2002
Unable to write salt to file!   Code:  2003
Error: EVP_CIPHER_CTX_new() completed with problem!   Code:  2004
Error: EVP_EncryptInit_ex() completed with problem!   Code:  2005
Error: EVP_EncryptUpdate() completed with problem!   Code:  2006
Error: EVP_EncryptFinal_ex() compelted with problem!   Code:  2007
Error: EVP_DecryptInit_ex() completed with problem!   Code:  2008
Error: EVP_DecryptUpdate() completed with problem!   Code:  2009
Error: EVP_DecryptFinal_ex() compelted with problem!   Code:  2010
File might be corrupted: too small to be encrypted!   Code:  3001
*/  // correct


