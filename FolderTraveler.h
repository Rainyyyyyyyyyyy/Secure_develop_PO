#ifndef FOLDERTRAVELER_H
#define FOLDERTRAVELER_H



#include "FolderTravelerExceptions.h"

#include <QVector>
#include <QString>



class FolderTraveler{
private:
    QVector <QString> pathList;     // список путей ко всем файлам внутри папки Folder_Path
    QString folderPath;     // путь к обозреваемой папке

    void listContents(const QString &path, int indent = 0);


public:

    void TravelFolder();

    //QVector <QString> TravelFolder(QString path){
    // конструктор по QString
    FolderTraveler(QString &path);

    // конструктор по умолчанию
    FolderTraveler() {}

    // вывод списка путей к файлам в обозреваемой папке
    void OutputList() const;


    // вывод пути к обозреваемой папке
    void OutputPath() const;

    // getter пути к папке
    const QString Path() const;


    // getter внутренних файлов
    const QVector <QString> Entries() const;


    // setter нового пути к папке
    void SetPath(const QString &s);

    // очистить список
    void clear();
};


#endif // FOLDERTRAVELER_H


