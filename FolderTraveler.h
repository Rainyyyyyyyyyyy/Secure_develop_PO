#ifndef FOLDERTRAVELER_H
#define FOLDERTRAVELER_H

#endif // FOLDERTRAVELER_H

#include <QTextStream>
#include <QDir>

#include "FolderTravelerExceptions.h"



class FolderTraveler{
private:
    // список путей ко всем файлам внутри папки Folder_Path
    QVector <QString> File_Path_List;
    // путь к обозреваемой папке
    QString Folder_Path;


    static QTextStream input; //(stdin);
    static QTextStream output; //(stdout);


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


