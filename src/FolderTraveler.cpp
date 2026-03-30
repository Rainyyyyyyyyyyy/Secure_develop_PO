#include "FolderTraveler.h"

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
void FolderTraveler::OutputList(){
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
void FolderTraveler::OutputPath(){
    output<<Folder_Path;
    output.flush();
}





void FolderTraveler::listContents(const QString &path, int indent) {
    QDir dir(path);

    // Проверяем, существует ли папка
    if (!dir.exists()) {
        output << "No Folder:" << path;
        output<<'\n';
        output.flush();
        return;
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
            File_Path_List.push_back(entry.absoluteFilePath());
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
