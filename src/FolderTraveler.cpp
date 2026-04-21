#include "FolderTraveler.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <sys/stat.h>
#endif

namespace {

bool isSystemEntry(const QFileInfo &entry)
{
#ifdef Q_OS_WIN
    const std::wstring path = entry.absoluteFilePath().toStdWString();
    const DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (attrs & FILE_ATTRIBUTE_SYSTEM) != 0;
#elif defined(Q_OS_UNIX)
    const QString p = entry.absoluteFilePath();
    if (p == "/proc" || p == "/sys" || p == "/dev" || p == "/run" ||
        p.startsWith("/proc/") || p.startsWith("/sys/") ||
        p.startsWith("/dev/") || p.startsWith("/run/")) {
        return true;
    }

    struct stat st {};
    const QByteArray nativePath = p.toLocal8Bit();
    if (::stat(nativePath.constData(), &st) != 0) {
        return false;
    }

    // Не добавляем специальные узлы ФС (device/fifo/socket).
    return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) ||
           S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode);
#else
    Q_UNUSED(entry);
    return false;
#endif
}

}

// для вывода
QTextStream FolderTraveler:: input = QTextStream(stdin);
QTextStream FolderTraveler::output = QTextStream(stdout);


void FolderTraveler::TravelFolder(){ //const QString &path){
    listContents(Folder_Path);
}

// конструктор по QString
FolderTraveler::FolderTraveler(QString &path){
    Folder_Path = path;
}


// вывод списка путей к файлам в обозреваемой папке
void FolderTraveler::OutputList() const{
    for(int i=0; i<File_Path_List.size(); i++){
        output<<File_Path_List[i]<<'\n';
    }
    output.flush();
}



// getter пути к папке
const QString FolderTraveler::Path() const {
    return Folder_Path;
}

// getter внутренних файлов
const QVector <QString> FolderTraveler::Entries() const {
    return File_Path_List;
}


// вывод пути к обозреваемой папке
void FolderTraveler::OutputPath() const{
    output<<Folder_Path;
    output.flush();
}



// очистить список
void FolderTraveler::clear(){
    Folder_Path = "";
    File_Path_List.clear();
}


void FolderTraveler::listContents(const QString &path, int indent) {
    QDir dir(path);

    // Проверяем, существует ли папка
    if (!dir.exists()) {
        //output << "No Folder:" << path;
        //output<<'\n';
        //output.flush();
        //return;
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
                output << indentStr + "[Folder]" + entry.fileName();
                output<<'\n';
                output.flush();
                // Рекурсивно обходим содержимое папки
                listContents(entry.absoluteFilePath(), indent + 1);
            }else{
                // Выводим ярлык на папку
                output << indentStr + "[Folder.lnk]" + entry.fileName();
                output<<'\n';
                output.flush();
                // и НЕ продолжаем рекурсию
            }
        } else {
            // добавление файла в список файлов
            if(entry.suffix().toLower() != "lnk" && !isSystemEntry(entry)){
                File_Path_List.push_back(entry.absoluteFilePath());
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
            output << indentStr + "[File]" + entry.fileName() + " (" + sizeStr + ")";
            else output << indentStr + "[File.lnk]" + entry.fileName() + " (" + sizeStr + ")";
            output<<'\n';
            output.flush();
        }
    }
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
*/



// setter нового пути к папке
void FolderTraveler::SetPath(const QString &s){
    Folder_Path = s;    // копирование
}
