#include <QCoreApplication>
#include <QTextStream>
#include <QDirIterator>
//#include <QFileDialog>
#include <QDateTime>

#include <iostream>

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


int main(int argc, char *argv[])
{
    QString dir_path;
    char *dirr = new char;
    QTextStream Qst;
    //Qst>>dir_path;
    std::cin.getline(dirr, 1);
    std::cout<<dirr<<'\n';
    // H:\Documents\Secure_tools_AndreevaVV\papka
    // H:/Documents/Secure_tools_AndreevaVV/papka
    print_all(dirr, Qst);

    //QCoreApplication a(argc, argv);
    std::cout<<"Hello from Qt and GitKraken\n";
    std::cout<<"GitCloned\n";
    return 0;
    //return a.exec();
}
