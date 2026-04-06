#include <QCoreApplication>
#include <QDir>
#include <QDebug>
//#include <QTextStream>
#include <windows.h>
#include <vector>

#include <QVector>

// Присоединилась openssl!!!
//#include <openssl/crypto.h>
#include <openssl/rand.h>

#include "FolderTraveler.h"
#include "CryptoLib.h"          // includes CryptoLibExceptions.h

QTextStream input(stdin);
QTextStream output(stdout);



bool checkMODE(const QString *modes, int size, const QString &s){
    for(int i=0; i<size; i++){
        if(s == modes[i])return true;
    }
    return false;
}

bool checkACTiON(const QString *actions, int size, const QString &s){
    for(int i=0; i<size; i++){
        if(s == actions[i])return true;
    }
    return false;
}
int main(int argc, char *argv[]) {
    //QCoreApplication app(argc, argv);

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
    //return 0;


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

    /* Проверка FolderTraveler */
    /*
    QString folderPathtest = "E:\\Z_vsyakoe_dla_echeby\\4k2sem\\SEcure_Develop_PO(Andreeva)\\laba1_test_files";
    FolderTraveler TestFolder_NoLnk(folderPathtest);
    TestFolder_NoLnk.TravelFolder();
    QVector <QString> testNoLnk = TestFolder_NoLnk.Entries();
    for(int i=0; i<testNoLnk.size(); i++){
        qDebug()<<testNoLnk[i];
    }
    return 0;
*/
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

    QString folderPath;
    FolderTraveler Folderr;
    QString current_UI_action = ".reset";
    do{
        Folderr.clear();
        try {
            qDebug()<<"Enter path to directory: "<<Qt::flush;
            folderPath = input.readLine();
            if(folderPath[0] == 'C' || folderPath[0] == 'c'){
                //qDebug()<<"No disk C!";
                folderPath = "";
                throw new ExceptionFolderFromDiskC; //continue;
            }
            if(folderPath == ""){
                //qDebug()<<"NO";
                //return 0;
                throw new ExceptionFolderNotFould;
            }
        }
        catch (const Exceptions *excp){
            qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
            delete excp;
            continue;
        }
        try{
            Folderr.SetPath(folderPath);
            output << "\nContent of  " << folderPath << ":\n";
            output << "====================================\n";
            output.flush();
            Folderr.TravelFolder();
        }
        catch (const Exceptions *excp){
            qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
            delete excp;

            Folderr.clear();
            continue;
        }

        Folderr.OutputList();

        qDebug()<<"Enter action ('.reset', or skip)"<<Qt::endl;
        current_UI_action = input.readLine();
    }while(current_UI_action == ".reset");

    //qDebug()<<"Ended program."<<Qt::endl;
    //return 0;

    QVector <QString> Folder_entries_list = Folderr.Entries();
    //return 0;
    CryptoActions &cry = CryptoActions::Instance();
    QString mode;
    QString MODES[] = {".encrypt", ".decrypt" };
    //QString action;
    //QString UI_ACTIONS[] = {".exit", ".reset", ".mode", ".password" };
    QString Password;// = "password";

    // запрос режима
    do{
        qDebug()<<"Enter mode ('.encrypt', '.decrypt'): ";
        mode = input.readLine();

    }while(checkMODE(MODES, 2, mode) == false);

    // запрос пароля
    current_UI_action = ".reset";
    do{
        qDebug()<<"Enter password: ";
        Password = input.readLine();

        qDebug()<<"Password: "<<Password<<Qt::endl;
        qDebug()<<"Enter action('.reset' or skip): ";
        current_UI_action = input.readLine();
    }while(current_UI_action == ".reset");


    if(mode == ".encrypt"){
        for(int i=0; i<Folder_entries_list.size(); i++){
            qDebug()<<Folder_entries_list[i]<<Qt::endl;
            try
            {
                cry.Encrypt_File(Folder_entries_list[i], Password);//(Path_to_encrypt_file, Password_to_encrypt);
            } catch(const Exceptions * excp){
                    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
                    delete excp;
            }
                //break;
        }
    }else if(mode == ".decrypt"){
        for(int i=0; i<Folder_entries_list.size(); i++){
            qDebug()<<Folder_entries_list[i]<<Qt::endl;
            try
            {
                cry.Decrypt_File(Folder_entries_list[i], Password);//(Path_to_encrypt_file, Password_to_encrypt);
            } catch(const Exceptions * excp){
                qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
                delete excp;
            }
        }
        //break;
    }


    return 0;

    // tests: E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1_test_files\Encrypt_Files_tests\emptytext.txt
    //          E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1_test_files\Encrypt_Files_tests\plaintext.txt
    //          E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1_test_files\Encrypt_Folder_tests\test1
    // password: "password"
    /*
     * зашифрованный файл - <имя_исходного_файла>.enc
     * дешифрованный файл - <имя_исходного_файла>.dec
     *
     */

    /*
    output<<"Enter path to file: ";
    output.flush();
    QString Path_to_encrypt_file = input.readLine();
    output<<"Enter password to encrypt: ";
    output.flush();
    //QString Password_to_encrypt = input.readLine();
*/
    //CryptoActions &cry = CryptoActions::Instance();

    // идеи для consoleUI:
    /*пароль начинается обязательно с буквы
     *  (и тем более не пустой)
     *  Ограничен например 8-32 символа
     *  Состоит из букв (заглавных и прописных)
     *   английского алфавита и спец. символов
     *  ! * ( ) , . / +
     *
     *  Служебное слово ".exit" для выхода из программы
     *  Служебное слово ".change" для изменения пути к папку\файлу
     *  Служебное слово ".mode" для изменения режима "Encrypt/Decrypt"
     *
     *  Служебные слова применимы на любой стадии (ввод пути, ввод пароля, ввод режима)
     *  и после завершения возвращают на первую стадию (например выбор режима)
     *  (либо оставляют на этой же стадии)
     */
/*
    try{
    // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try7_gitclone\papki\asd.txt
        cry.Encrypt_File(Path_to_encrypt_file, Password_to_encrypt);
        qDebug()<<"\n"<<Qt::flush;
        cry.Decrypt_File(Path_to_encrypt_file + ".enc", Password_to_encrypt);
    }
    catch(const Exceptions *excp){
        qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
        delete excp;
    }
*/
    //CryptoActions::Instance().Encrypt_File("abc","abckey");




    return 0;
}
