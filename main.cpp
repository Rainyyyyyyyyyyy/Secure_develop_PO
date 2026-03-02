#include <QCoreApplication>
#include <QTextStream>
#include <QDirIterator>
//#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

#include <iostream>

QTextStream cin(stdin);
QTextStream cout(stdout);

void print_attribs(QFileInfo& info, QTextStream& stream) {
    if (info.isReadable())
        cout << "R";
    if (info.isWritable())
        cout << "W";
    if (info.isHidden())
        cout << "H";
    if (info.isExecutable())
        cout << "E";
}

QString fileSize(qint64 nSize) {
    qint64 i = 0;
    for (; nSize > 1023; nSize /= 1024, ++i) { }
    return QString().setNum(nSize) + "BKMGT"[i];
}

void folderTraverse(QString& path) {
    QDirIterator itDirs(path, QDir::Dirs);
    while (itDirs.hasNext()) {
        itDirs.next();

        if (itDirs.fileName() == ".") continue;

        QFileInfo info = itDirs.fileInfo();
        cout << "D | " << info.fileName() << " | " << fileSize(info.size())
             << " | " << info.lastModified().toString() << " | ";
        print_attribs(info, cout);
        cout << Qt::endl;
    }
}

QString getValidDir() {
    QString path;
    cout << "Enter path for folder to encrypt: ";
    cout.flush();
    path = cin.readLine();

    QDir folder(path);
    bool valid = false;

    while (!folder.exists()) {
        cout << "Path is incorrect or there is no such path :(" << Qt::endl;
        cout << "Try again: ";
        cout.flush();
        path = cin.readLine();
        folder = QDir(path);
    }

    cout << "Path found :)" << Qt::endl;

    return path;
}

int main()
{
    QString path = getValidDir();
    folderTraverse(path);

    return 0;
    // /Users/liza/Desktop/ТехнЧтение/
    //return a.exec();
}
/*
void print_attribs(QFileInfo& info, QTextStream& stream) {
    if (info.isReadable())
        stream << "R";
    if (info.isWritable())
        stream << "W";
    if (info.isHidden())
        stream << "H";
    if (info.isExecutable())
        stream << "E";
}


QString fileSize(qint64 nSize) {
    qint64 i = 0;
    for (; nSize > 1023; nSize /= 1024, ++i) { }
    return QString().setNum(nSize) + "BKMGT"[i];
}

void print_all(QString path, QTextStream& stream) {
    stream<<path;
    QDirIterator itDirs(path, QDir::Dirs);
    while (itDirs.hasNext()) {
        itDirs.next();
        if (itDirs.fileName() == ".")
            continue;
        QFileInfo info = itDirs.fileInfo();
        stream << "D | " << info.fileName() << " | " << fileSize(info.size()) << " | "
               << info.lastModified().toString() << " | ";
        print_attribs(info, stream);
        stream << "\n";
    }
    QDirIterator itFiles(path, QDir::Files);
    while (itFiles.hasNext()) {
        itFiles.next();
        QFileInfo info = itFiles.fileInfo();
        stream << "F | " << info.fileName() << " | " << fileSize(info.size()) << " | "
               << info.lastModified().toString() << " | ";
        print_attribs(info, stream);
        stream << "\n";
    }
}

*/

/*
void recursiveFolderTraversal(const QString &path, int depth = 0)
{
    QDirIterator it(path, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QString filePath = it.filePath();
        QFileInfo fileInfo = it.fileInfo();

        // Вывод информации о файле с отступами для наглядности структуры
        QString indent = QString(depth * 2, ' ');
        qDebug().noquote() << indent << "Файл:" << fileInfo.fileName()
                           << "Размер:" << fileInfo.size() << "байт";
    }
}


#include <clocale>
#include <Windows.h>

int main(int argc, char *argv[])
{

    setlocale(LC_ALL, "PL_pl.UTF-8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    QCoreApplication a(argc, argv);

    QString startPath = QDir(""//QDir::currentPath(); // Текущая директория
    qDebug() << "Начало обхода папки:" << startPath;
    qDebug() << "----------------------------------------";

    recursiveFolderTraversal(startPath);

    return 0;
}
*/


//int main(int argc, char *argv[])
//{
    /*
    char * str = new char;
    std::cin>>str;
    std::cout<<"\nEntered: "<<str<<'.'<<'\n';
    QTextStream stream;

    QString qpath = "H:/Documents/Secure_tools_AndreevaVV/papka";
    QDir("H:/Documents/Secure_tools_AndreevaVV/papka");
    print_all(str, stream);
    return 0;
    */



    /*

    QString dir_path;
    char *dirr = new char;
    QTextStream Qst;
    Qst>>dir_path;
    Qst<<'\n'<<dir_path;
    //std::cin.getline(dirr, 1);
    //std::cout<<dirr<<'\n';
    // H:\Documents\Secure_tools_AndreevaVV\papka
    // H:/Documents/Secure_tools_AndreevaVV/papka
    print_all(dirr, Qst);

    //QCoreApplication a(argc, argv);
    std::cout<<"Hello from Qt and GitKraken\n";
    std::cout<<"GitCloned\n";
    return 0;
    //return a.exec();
*/
//}
