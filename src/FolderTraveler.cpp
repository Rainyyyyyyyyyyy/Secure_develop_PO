#include "FolderTraveler.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <sys/stat.h>
#endif


#include <QDebug>
#include <QDir>




//QTextStream FolderTraveler::output = QTextStream(stdout);


void FolderTraveler::TravelFolder(){ //const QString &path){
    listContents(folderPath);
}

// конструктор по QString
FolderTraveler::FolderTraveler(QString &path){
    folderPath = path;
}


// вывод списка путей к файлам в обозреваемой папке
void FolderTraveler::OutputList() const{
    for(int i=0; i<pathList.size(); i++){
        qDebug()<<pathList[i];
    }
}



// getter пути к папке
const QString FolderTraveler::Path() const {
    return folderPath;
}

// getter внутренних файлов
const QVector <QString> FolderTraveler::Entries() const {
    return pathList;
}


// вывод пути к обозреваемой папке
void FolderTraveler::OutputPath() const{
    qDebug()<<folderPath;
}



// очистить список
void FolderTraveler::clear(){
    folderPath = "";
    pathList.clear();
}

/*
 * Пустая папка: File_Path_List пустой и throw ExceptionFolderIsEmpty()
 */
void FolderTraveler::listContents(const QString &path, int indent) {
    QDir dir(path);

    // Проверяем, существует ли папка
    if (!dir.exists()) {
        throw ExceptionFolderNotFould();
    }

    // Получаем список всех файлов и папок (включая скрытые)
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    // Отступ для визуального отображения вложенности
    QString indentStr(indent * 2, ' ');

    // цикл по содержимому
    for (const QFileInfo &entry : entries) {
        // если папка
        if (entry.isDir()) {
            // проверка на то, что эта папка - на самом деле ярлык на папку
            // если так, то игнорировать её
            if(entry.suffix().toLower() != "lnk"){
                // Выводим папку
                qDebug() << indentStr + "[Folder]" + entry.fileName();

                // Рекурсивно обходим содержимое папки
                listContents(entry.absoluteFilePath(), indent + 1);
            }else{
                // Выводим ярлык на папку
                qDebug() << indentStr + "[Folder.lnk]" + entry.fileName();

                // и НЕ продолжаем рекурсию
            }
        } else {
            // добавление файла в список файлов
            // с проверкой на ярлык.lnk и системные атрибуты
            try{
                if(isSystemEntry(entry)) throw ExceptionPathFromSystemEntries();
                    if(entry.suffix().toLower() != "lnk"){
                        pathList.push_back(entry.absoluteFilePath());
                    }
            }catch (ExceptionPathFromSystemEntries &excp){
                    qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
            }

            // Выводим файл с информацией о размере
            QString sizeStr;    // для вывода
            qint64 size = entry.size();

            if (size < 1024) {
                sizeStr = QString::number(size) + " B";
            } else if (size < 1024 * 1024) {
                sizeStr = QString::number(size / 1024.0, 'f', 2) + " KB";
            } else if (size < 1024 * 1024 * 1024) {
                sizeStr = QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
            } else {
                sizeStr = QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
            }
            if(entry.suffix().toLower() != "lnk")
            qDebug() << indentStr + "[File]" + entry.fileName() + " (" + sizeStr + ")";
            else qDebug() << indentStr + "[File.lnk]" + entry.fileName() + " (" + sizeStr + ")";
        }
    }
    if(pathList.size() == 0){
        throw ExceptionFolderIsEmpty();
    }
}


// setter нового пути к папке
void FolderTraveler::SetPath(const QString &s){
    folderPath = s;    // копирование
    pathList.clear();

}


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
/* Вывод:
[Folder]Encrypt_Files_tests
  [File]emptytext.txt (0 B)
  [File]plaintext.txt (129 B)
[Folder]Encrypt_Folder_tests
  [Folder]test1
    [Folder]papka1
      [File]file1.txt (45 B)
      [File]file2.txt (51 B)
    [Folder]papka2
      [File]file3.txt (61 B)
      [Folder.lnk]papka1-yarl.lnk
    [Folder]papka3
      [File]file4.txt (42 B)
      [File]file5.txt (60 B)
      [Folder]papka4
        [File]fiel7.txt (0 B)
        [Folder]papka6
          [File]file9.txt (0 B)
          [Folder]papka7
            [File]file10.txt (0 B)
      [Folder]papka5
        [File]file8.txt (0 B)
[Folder]FolderTraveler_tests
  [Folder]papka1
    [File]file1.txt (0 B)
    [File]file2.txt (0 B)
    [File]file3.txt (0 B)
    [Folder]papka4
      [File]file1.txt (17 B)
  [Folder]papka2
    [File.lnk]file2.txt.yarl.lnk (0 B)
    [Folder.lnk]papka4.lnk
  [Folder]papka3
*/
/* содержимое QVector <QString> File_Path_List :
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Files_tests/emptytext.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Files_tests/plaintext.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka1/file1.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka1/file2.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka2/file3.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka3/file4.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka3/file5.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka3/papka4/fiel7.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka3/papka4/papka6/file9.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka3/papka4/papka6/papka7/file10.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/Encrypt_Folder_tests/test1/papka3/papka5/file8.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/FolderTraveler_tests/papka1/file1.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/FolderTraveler_tests/papka1/file2.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/FolderTraveler_tests/papka1/file3.txt"
"E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1_test_files/FolderTraveler_tests/papka1/papka4/file1.txt"

* Вывоится всё содержимое (в том числе ярлыки, но рекурсия не углубляется по адресам ярлыков)
* Список заполняется в соответствии с выводом, КРОМЕ ярлыков (все ярлыки игнорируются)
*       Системные пути и файлы также отслеживаются и генерируется исключение при их обнаружении
*/




