#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <windows.h>
#include <vector>

// #include <cryptopp/hex.h>    // successfully found
// #include <openssl/aes.h>      // successfully found


// для шифрования
// #include <библиотека cryptopp.h>
// #include <бибилотеки openssl>

// написать функцию шифрования
/*
 * возвращает std::vector<Qstring> вектор путей к зашифрованным файлам
 * на вход подаётся путь к папке\файлу для шифрования
 *              если вход - папка, то шифруются все файлы внутри (в т.ч. вложенные)
 *              если вход - файл, то шифруется только он
 *
 *              */

std::vector<QString> EncryptFiles(){

}
// написать функицю дешифрования
// написать функцию проверки шифрованности (не шифровать если зашифровано и наоборот)

// обернуть в класс (синглтон)




// indent - отступ (для вывода)
void listContents(const QString &path, int indent = 0) {
    QDir dir(path);

    // Проверяем, существует ли папка
    if (!dir.exists()) {
        qDebug() << "No Folder:" << path;
        return;
    }

    // Получаем список всех файлов и папок (включая скрытые)
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    // Отступ для визуального отображения вложенности
    QString indentStr(indent * 2, ' ');

    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            // Выводим папку
            qDebug().noquote() << indentStr + "[Folder]" + entry.fileName();
            // Рекурсивно обходим содержимое папки
            listContents(entry.absoluteFilePath(), indent + 1);
        } else {
            // Выводим файл с информацией о размере
            QString sizeStr;
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

            qDebug().noquote() << indentStr + "[File]" + entry.fileName() + " (" + sizeStr + ")";
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString folderPath;
    QTextStream input(stdin);
    QTextStream output(stdout);
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    output << "Enter path to folder: ";
    output.flush();
    // TSU:     // H:\Documents\Secure_tools_AndreevaVV\TRUE_REPO
    // obshaga: // E:\Z_vsyakoe_dla_echeby\4k2sem\SEcure_Develop_PO(Andreeva)\laba1\Qt\try3_gitclone
    folderPath = input.readLine().trimmed();

    output << "\nContent of  " << folderPath << ":\n";
            output << "====================================\n";
    output.flush();

    listContents(folderPath);

    return 0;
}
