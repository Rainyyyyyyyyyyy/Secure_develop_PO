#include "CryptoLib.h"
//#include "CryptoLibExceptions.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDir>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

// анонимное namespace известное этому и только этому файлу (FolderTraveler.cpp)
namespace {
/*
 * Windows:
 *      системный файл ((GetFileAttributes() & FILE_ATTRIBUTE_SYSTEM) == true) -> return true
 *      несистемный файл ((GetFileAttributes() & FILE_ATTRIBUTE_SYSTEM) == false) -> return false
 * Unix:
 *      файл в одной из подсистемных папок:
 *      /proc
 *      /sys
 *      /dev
 *      /run
 *      либо содержит в полном пути эти папки -> return true
 *      файл вне подсистемных папок:
 *       /proc
 *      /sys
 *      /dev
 *      /run
 *      и не содержит в полном пути этих папок -> return false
*/
bool isSystemEntry(const QFileInfo &entry){
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




// анонимное namespace известное только и только этому файлу - файлу CryptoLib.cpp
namespace {

    // длина хеша 32+16+32
    constexpr int DERIVED_LEN = KEY_LEN + IV_LEN + HASH_LEN;

    // хешируем ключ password и соль salt
    QByteArray deriveKeyMaterial(const QString &password, const unsigned char salt[SALT_LEN]){
        QByteArray passwordBytes = password.toUtf8();
        QByteArray derived;
        derived.resize(DERIVED_LEN);

        if (!PKCS5_PBKDF2_HMAC(passwordBytes.constData(), passwordBytes.size(),
                               salt, SALT_LEN,
                               ITERATIONS,
                               EVP_sha256(),
                               DERIVED_LEN,
                               reinterpret_cast<unsigned char*>(derived.data()))) {

            return QByteArray();
        }

    return derived;
    }



    void listContents(QString path, QVector <QString> &pathList) {
    QDir dir(path);

    // Проверяем, существует ли папка
    if (!dir.exists()) {
            throw ExceptionFolderNotFould();
    }

    // Получаем список всех файлов и папок (включая скрытые)
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    // Отступ для визуального отображения вложенности
    //QString indentStr(indent * 2, ' ');

    // цикл по содержимому
    for (const QFileInfo &entry : entries) {
            // если папка
            if (entry.isDir()) {
                // проверка на то, что эта папка - на самом деле ярлык на папку
                // если так, то игнорировать её
                if(entry.suffix().toLower() != "lnk"){
                    // Выводим папку
                    /////qDebug() << indentStr + "[Folder]" + entry.fileName();

                    // Рекурсивно обходим содержимое папки
                    listContents(entry.absoluteFilePath(), pathList); //indent + 1);
                }else{
                    // Выводим ярлык на папку
                    /////qDebug() << indentStr + "[Folder.lnk]" + entry.fileName();

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
                //if(entry.suffix().toLower() != "lnk")
                //qDebug() << indentStr + "[File]" + entry.fileName() + " (" + sizeStr + ")";
                //else qDebug() << indentStr + "[File.lnk]" + entry.fileName() + " (" + sizeStr + ")";
            }
    }
    if(pathList.size() == 0){
            throw ExceptionFolderIsEmpty();
    }
}
}




// зашифровать папку
bool CryptoActionsAES::Encrypt_Folder(const QString &folderPath, const QString &password){
    QVector <QString> Paths_to_files;
    listContents(folderPath, Paths_to_files);


    for(auto temp_path : Paths_to_files){
            qDebug()<<temp_path;
            Encrypt_File(temp_path, password);
    }
}
// дешифровать папку
bool CryptoActionsAES::Decrypt_Folder(const QString &folderPath, const QString &password){
    QVector <QString> Paths_to_files;
    listContents(folderPath, Paths_to_files);


    for(auto temp_path : Paths_to_files){
            qDebug()<<temp_path;
            Decrypt_File(temp_path, password);
    }
}





/*
 * Файл имеет сигнатуру в начале файла -> return true
 * Файл имеет сигнатуру не в начале файле -> return false
 * Файл не имеет сигнатуру -> return false
*/
bool CryptoActionsAES::IsFileEncrypted(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            throw ExceptionUnableToOpenFile();
    }

    /*if(file.size() == 0){
            file.close();
            throw ExceptionFileIsEmpty();
    }
    */
    // Проверяем, достаточно ли размера для сигнатуры
    if (file.size() < SIGN_LEN) {
        file.close();
            return false;//throw ExceptionFileTooSmall();
    }

    // Читаем сигнатуру
    char magic[SIGN_LEN];
    if (file.read(magic, SIGN_LEN) != SIGN_LEN) {
        file.close();
         throw ExceptionUnableToReadSign();
        //return false;
    }

    file.close();

    // Сравниваем с нашей сигнатурой
    return memcmp(magic, Signature_Sequence.constData(), SIGN_LEN) == 0;
}

/*
 * Файл пуст -> throw ExceptionFileIsEmpty
 * Файл содержит сигнатуру -> throw ExceptionFileIsAlreadyEncrypted
 * Файл не пуст и не содержит сигнатуры -> файл зашифрован, return true
*/
//  [SIGN][SALT][VERIFIER][CIPHERTEXT]
bool CryptoActionsAES::Encrypt_File(const QString &filePath, const QString &password) {
    // 1. Проверяем, существует ли файл

    QFile inputFile(filePath);
    //if (!inputFile.exists()) {
    //    throw new ExceptionFileNotFound;
    //}
    // проверка на наличие сигнатуры
    if(IsFileEncrypted(filePath)){
         throw ExceptionFileIsAlreadyEncrypted();
    }else{
         if(inputFile.size() == 0){
             throw ExceptionFileIsEmpty();
         }
    }

    // 2. Открываем файл для чтения
    if (!inputFile.open(QIODevice::ReadOnly)) {
         throw ExceptionUnableToOpenFile();
    }

    // 3. Читаем содержимое файла в QByteArray
    QByteArray plainData = inputFile.readAll();
    inputFile.close();

    /*if (plainData.isEmpty()) {
        //throw new ExceptionEmptyFile;
        qDebug()<<"Warning: Empty file: "<<filePath<<Qt::flush;
    }*/

    // 4. Генерируем случайную соль
    unsigned char salt[SALT_LEN];
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw ExceptionOpensslRandbytes();
    }

    // 5. Преобразуем пароль, соль в key/iv/verifier через PBKDF2
    unsigned char key[KEY_LEN];
    unsigned char iv[IV_LEN];
    unsigned char verifier[HASH_LEN];

    QByteArray derived = deriveKeyMaterial(password, salt);
    if (derived.isEmpty()) {
        throw new ExceptionOpensslHMAC;
    }
    memcpy(key, derived.constData(), KEY_LEN);
    memcpy(iv, derived.constData() + KEY_LEN, IV_LEN);
    memcpy(verifier, derived.constData() + KEY_LEN + IV_LEN, HASH_LEN);

    // 6. Инициализируем контекст шифрования
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw ExceptionOpensslCipherCTXnew();
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw ExceptionOpensslEncryptInit();
    }

    // 7. Шифруем данные
    // Вычисляем максимальный размер зашифрованных данных (с учётом padding)
    int maxCipherLen = plainData.size() + EVP_MAX_BLOCK_LENGTH;
    QByteArray cipherData;
    cipherData.resize(maxCipherLen);

    int outLen = 0;
    int totalLen = 0;

    // Шифруем основные данные
    if (1 != EVP_EncryptUpdate(ctx,
                               reinterpret_cast<unsigned char*>(cipherData.data()),
                               &outLen,
                               reinterpret_cast<unsigned char*>(plainData.data()),
                               plainData.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw ExceptionOpensslEncryptUpdate();
    }
    totalLen = outLen;

    // Финализируем шифрование (добавляем padding)
    if (1 != EVP_EncryptFinal_ex(ctx,
                                 reinterpret_cast<unsigned char*>(cipherData.data() + totalLen),
                                 &outLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw ExceptionOpensslEncryptFinal();
    }
    totalLen += outLen;

    cipherData.resize(totalLen);

    EVP_CIPHER_CTX_free(ctx);

    QByteArray finalData;
    finalData.append(Signature_Sequence.constData(), SIGN_LEN);
    finalData.append(reinterpret_cast<const char*>(salt), SALT_LEN);
    finalData.append(reinterpret_cast<const char*>(verifier), HASH_LEN);
    finalData.append(cipherData);

    QFile outputFile(filePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw ExceptionUnableToCreateFile();
    }

    if (outputFile.write(finalData) != finalData.size()) {
        outputFile.close();
        throw ExceptionUnableToWriteEncryptedTextToFile();
    }

    outputFile.close();
    return 0;
}



/*
 * Файл пуст (пароль не важен) -> throw ExceptionFileIsEmpty
 * Файл содержит сигнатуру и получен верный пароль -> файл дешифрован, return true
 * Файл содержит сигнатуру и получен неверный пароль -> throw ExceptionIncorrectPassword
 * Файл не пуст и не содержит сигнатуры (пароль не важен) -> throw ExceptionFileIsAlreadyDecrypted
*/
//  [SIGN][SALT][VERIFIER][CIPHERTEXT]
bool CryptoActionsAES::Decrypt_File(const QString &filePath, const QString &password) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw ExceptionUnableToOpenFile();
    }

    if (file.size() < (SALT_LEN + SIGN_LEN + HASH_LEN)) {
        file.close();
        throw ExceptionFileTooSmall();
    }

    if(!IsFileEncrypted(filePath)){
        throw ExceptionFileIsAlreadyDecrypted();
    }

    QByteArray encryptedData = file.readAll();
    file.close();

    unsigned char salt[SALT_LEN];
    unsigned char verifierFromFile[HASH_LEN];
    memcpy(salt, encryptedData.constData() + SIGN_LEN, SALT_LEN);
    memcpy(verifierFromFile, encryptedData.constData() + SIGN_LEN + SALT_LEN, HASH_LEN);

    QByteArray derived = deriveKeyMaterial(password, salt);
    if (derived.isEmpty()) {
        throw ExceptionOpensslHMAC();
    }

    const unsigned char *derivedBytes = reinterpret_cast<const unsigned char*>(derived.constData());
    const unsigned char *key = derivedBytes;
    const unsigned char *iv = derivedBytes + KEY_LEN;
    const unsigned char *verifier = derivedBytes + KEY_LEN + IV_LEN;

    if (memcmp(verifierFromFile, verifier, HASH_LEN) != 0) {
        throw ExceptionIncorrectPassword();
    }

    QByteArray cipherData = encryptedData.mid(SIGN_LEN + SALT_LEN + HASH_LEN);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw ExceptionOpensslCipherCTXnew();
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                const_cast<unsigned char*>(key),
                                const_cast<unsigned char*>(iv))) {
        EVP_CIPHER_CTX_free(ctx);
        throw ExceptionOpensslDecryptInit();
    }

    QByteArray plainData;
    plainData.resize(cipherData.size() + EVP_MAX_BLOCK_LENGTH);

    int outLen = 0;
    int totalLen = 0;

    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(plainData.data()),
                      &outLen,
                      reinterpret_cast<unsigned char*>(cipherData.data()),
                      cipherData.size());
    totalLen = outLen;

    if (1 != EVP_DecryptFinal_ex(ctx,
                                 reinterpret_cast<unsigned char*>(plainData.data() + totalLen),
                                 &outLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw ExceptionOpensslDecryptFinal();
    }
    totalLen += outLen;

    plainData.resize(totalLen);
    EVP_CIPHER_CTX_free(ctx);

    QString outputFilePath = filePath;
    if (outputFilePath.endsWith(".enc")) {
        outputFilePath.chop(4);
    } else {
        //outputFilePath += ".dec";
    }

    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw ExceptionUnableToCreateFile();
    }

    if (outputFile.write(plainData) != plainData.size()) {
        outputFile.close();
        outputFile.remove();
        throw ExceptionUnableToWriteDecryptedTextToFile();
    }

    outputFile.close();

    //output << "Decrypted: " << filePath << " -> " << outputFilePath << "\n";
    //output << "  Size: " << plainData.size() << " bytes\n";

    return true;
}
