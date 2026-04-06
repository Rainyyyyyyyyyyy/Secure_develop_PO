#include "CryptoLib.h"






// Константы для алгоритма
const int KEY_LEN = 32;      // 256 бит для AES-256
const int IV_LEN = 16;       // 16 байт для AES
const int SALT_LEN = 16;     // 16 байт соли
const int ITERATIONS = 10000; // 10000 итераций PBKDF2
const int SIGN_LEN = 16;    // 16 байт для сигнатуры
const QByteArray Signature_Sequence = "A66B06F945C9B57E";

bool CryptoActions::IsFileEncrypted(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        throw new ExceptionUnableToOpenFile;
        //return false;
    }

    // Проверяем, достаточно ли размера для сигнатуры
    if (file.size() < SIGN_LEN) {
        file.close();
        throw new ExceptionFileTooSmall;
        //return false;
    }

    // Читаем сигнатуру
    char magic[SIGN_LEN];
    if (file.read(magic, SIGN_LEN) != SIGN_LEN) {
        file.close();
        throw new ExceptionUnableToReadSign;
        //return false;
    }

    file.close();

    // Сравниваем с нашей сигнатурой
    return memcmp(magic, Signature_Sequence, SIGN_LEN) == 0;
}


bool CryptoActions::Encrypt_File(const QString &filePath, const QString &password) {
    // 1. Проверяем, существует ли файл

    QFile inputFile(filePath);
    //if (!inputFile.exists()) {
    //    throw new ExceptionFileNotFound;
    //}
    // проверка на наличие сигнатуры
    if(IsFileEncrypted(filePath)){
        throw new ExceptionFileIsAlreadyEncrypted;
    }

    // 2. Открываем файл для чтения
    if (!inputFile.open(QIODevice::ReadOnly)) {
        throw new ExceptionUnableToOpenFile;
    }

    // 3. Читаем содержимое файла в QByteArray
    QByteArray plainData = inputFile.readAll();
    inputFile.close();

    if (plainData.isEmpty()) {
        //throw new ExceptionEmptyFile;
        qDebug()<<"Warning: Empty file: "<<filePath<<Qt::flush;
    }

    // 4. Генерируем случайную соль
    unsigned char salt[SALT_LEN];
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw new ExceptionOpensslRandbytes;
    }

    // 5. Преобразуем пароль в ключ и IV через PBKDF2
    unsigned char key[KEY_LEN];
    unsigned char iv[IV_LEN];
    QByteArray passwordBytes = password.toUtf8();

    if (!PKCS5_PBKDF2_HMAC(passwordBytes.constData(), passwordBytes.size(),
                           salt, sizeof(salt),
                           ITERATIONS,
                           EVP_sha256(),
                           KEY_LEN + IV_LEN, key)) {
        throw new ExceptionOpensslHMAC;
    }
    memcpy(iv, key + KEY_LEN, IV_LEN);

    // 6. Инициализируем контекст шифрования
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw new ExceptionOpensslCipherCTXnew;
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw new ExceptionOpensslEncryptInit;
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
        throw new ExceptionOpensslEncryptUpdate;
    }
    totalLen = outLen;

    // Финализируем шифрование (добавляем padding)
    if (1 != EVP_EncryptFinal_ex(ctx,
                                 reinterpret_cast<unsigned char*>(cipherData.data() + totalLen),
                                 &outLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw new ExceptionOpensslEncryptFinal;
    }
    totalLen += outLen;

    // Обрезаем QByteArray до реального размера
    cipherData.resize(totalLen);

    EVP_CIPHER_CTX_free(ctx);

    // 8. Формируем итоговый файл: [SIGN][SALT][CIPHERTEXT]
    QByteArray finalData;
    finalData.append(reinterpret_cast<const char*>(Signature_Sequence.data()), SIGN_LEN);
    finalData.append(reinterpret_cast<const char*>(salt), SALT_LEN);
    finalData.append(cipherData);

    // 9. Открываем файл на запись (перезаписываем исходный файл)
    QFile outputFile(filePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw new ExceptionUnableToCreateFile;
    }

    if (outputFile.write(finalData) != finalData.size()) {
        outputFile.close();
        throw new ExceptionUnableToWriteEncryptedTextToFile;
    }

    outputFile.close();
    return 0;
}
/*
bool CryptoActions::Encrypt_File(const QString &filePath, const QString &password){
    //output<<"Encryption Function has called.\n";
    //output.flush();

    //return false;

    // 1. Проверяем, существует ли файл
    QFile inputFile(filePath);
    if (!inputFile.exists()) {
        //qCritical() << "File not found:" << filePath;
        Exceptions *Excp = new ExceptionFileNotFound;
        throw Excp;
        //    return false;
    }

    // 2. Открываем исходный файл для чтения
    if (!inputFile.open(QIODevice::ReadOnly)) {
        //qCritical() << "Unable to open file:" << filePath;
        //return false;
        Exceptions *Excp = new ExceptionUnableToOpenFile;
        throw Excp;
    }

    // 3. Формируем имя для зашифрованного файла
    QString encryptedFilePath = filePath + ".enc";
    QFile outputFile(encryptedFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        //qCritical() << "Unable to create file:" << encryptedFilePath;
        inputFile.close();
        Exceptions *Excp = new ExceptionUnableToCreateFile;
        throw Excp;
        //return false;
    }

    // 4. Генерируем случайную соль
    unsigned char salt[SALT_LEN];
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        //qCritical() << "Error from generation salt";
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionOpensslRandbytes;
        throw Excp;
        //return false;
    }

    // 5. Преобразуем пароль в ключ и вектор инициализации (IV) с помощью PBKDF2
    unsigned char key[KEY_LEN];
    unsigned char iv[IV_LEN];
    QByteArray passwordBytes = password.toUtf8();

    // Функция PKCS5_PBKDF2_HMAC для получения ключа и IV из пароля
    if (!PKCS5_PBKDF2_HMAC(passwordBytes.constData(), passwordBytes.size(),
                           salt, sizeof(salt),
                           ITERATIONS,
                           EVP_sha256(),
                           KEY_LEN + IV_LEN, key)) {
        //qCritical() << "Error from output key PBKDF2!";
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionOpensslHMAC;
        throw Excp;
        //return false;
    }
    // Разделяем полученные данные на ключ и IV
    memcpy(iv, key + KEY_LEN, IV_LEN);

    // 6. Сохраняем соль в начало выходного файла
    if (outputFile.write(reinterpret_cast<const char*>(salt), SALT_LEN) != SALT_LEN) {
        //qCritical() << "Unable to write salt in file!";
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionOpensslWriteSaltToFile;
        throw Excp;
        //return false;
    }

    // 7. Инициализируем контекст шифрования
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        //qCritical() << "Error from creating context!";
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionOpensslCipherCTXnew;
        throw Excp;
        //return false;
    }

    // Выбираем алгоритм AES-256-CBC
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        //qCritical() << "Error from initialization!";
        EVP_CIPHER_CTX_free(ctx);
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionOpensslEncryptInit;
        throw Excp;
        //return false;
    }

    // 8. Читаем и шифруем данные блоками
    const int BUFFER_SIZE = 1024 * 16; // 16KB буфер
    QByteArray inBuffer;
    inBuffer.resize(BUFFER_SIZE);
    unsigned char outBuffer[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];

    int bytesRead = 0;
    int bytesWritten = 0;
    int outLen = 0;

    while ((bytesRead = inputFile.read(inBuffer.data(), BUFFER_SIZE)) > 0) {
        // Шифруем блок
        if (1 != EVP_EncryptUpdate(ctx, outBuffer, &outLen,
                                   reinterpret_cast<unsigned char*>(inBuffer.data()),
                                   bytesRead)) {
            //qCritical() << "Error from encrypting block!";
            EVP_CIPHER_CTX_free(ctx);
            inputFile.close();
            outputFile.close();
            Exceptions *Excp = new ExceptionOpensslEncryptUpdate;
            throw Excp;
            //return false;
        }
        // Записываем зашифрованный блок
        if (outputFile.write(reinterpret_cast<const char*>(outBuffer), outLen) != outLen) {
            //qCritical() << "Error from writing ciphertext!";
            EVP_CIPHER_CTX_free(ctx);
            inputFile.close();
            outputFile.close();
            Exceptions *Excp = new ExceptionUnableToWriteEncryptedTextToFile;
            throw Excp;
            //return false;
        }
        bytesWritten += outLen;
    }

    // 9. Финализируем шифрование (дописываем последний блок с паддингом)
    if (1 != EVP_EncryptFinal_ex(ctx, outBuffer, &outLen)) {
        //qCritical() << "Error from completed encryption!";
        EVP_CIPHER_CTX_free(ctx);
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionOpensslEncryptFinal;
        throw Excp;
        //return false;
    }
    if (outputFile.write(reinterpret_cast<const char*>(outBuffer), outLen) != outLen) {
        //qCritical() << "Error from writing final data!";
        EVP_CIPHER_CTX_free(ctx);
        inputFile.close();
        outputFile.close();
        Exceptions *Excp = new ExceptionUnableToWriteFinalDataToFile;
        throw Excp;
        //return false;
    }
    bytesWritten += outLen;

    // 10. Очищаем память и закрываем файлы
    EVP_CIPHER_CTX_free(ctx);
    inputFile.close();
    outputFile.close();

    output << "Succesfully encrypted!\n";
    output << "  File:" << filePath<<'\n';
    output << "  cipherFile:" << encryptedFilePath<<'\n';
    output << "  Plain text size:" << inputFile.size() << "bytes\n";
    output << "  Cipher text size:" << bytesWritten << "bytes\n";

        return true;
}

*/
bool CryptoActions::Decrypt_File(const QString &filePath, const QString &password) {
    // 1. Открываем файл
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw new ExceptionUnableToOpenFile;
    }

    // 2. Проверяем минимальный размер
    if (file.size() < (SALT_LEN + SIGN_LEN)) {
        file.close();
        throw new ExceptionFileTooSmall;
    }

    if(!IsFileEncrypted(filePath)){
        throw new ExceptionFileIsAlreadyDecrypted;
    }

    // 3. Читаем файл
    QByteArray encryptedData = file.readAll();
    file.close();
    // 4.0 Извлекаем сигнатуру

    // 4. Извлекаем соль и шифротекст
    unsigned char salt[SALT_LEN];
    memcpy(salt, encryptedData.constData() + SIGN_LEN, SALT_LEN);
    QByteArray cipherData = encryptedData.mid(SALT_LEN + SIGN_LEN);

    // 5. Получаем ключ и IV
    unsigned char key[KEY_LEN];
    unsigned char iv[IV_LEN];
    QByteArray passwordBytes = password.toUtf8();

    if (!PKCS5_PBKDF2_HMAC(passwordBytes.constData(), passwordBytes.size(),
                           salt, SALT_LEN, ITERATIONS,
                           EVP_sha256(), KEY_LEN + IV_LEN, key)) {
        throw new ExceptionOpensslHMAC;
    }
    memcpy(iv, key + KEY_LEN, IV_LEN);

    // 6. Расшифровываем
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw new ExceptionOpensslCipherCTXnew;
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw new ExceptionOpensslDecryptInit;
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
        throw new ExceptionOpensslDecryptFinal;
    }
    totalLen += outLen;

    plainData.resize(totalLen);
    EVP_CIPHER_CTX_free(ctx);

    // 7. Определяем выходной файл (убираем .enc, если есть)
    QString outputFilePath = filePath;
    if (outputFilePath.endsWith(".enc")) {
        outputFilePath.chop(4);
    } else {
        //outputFilePath += ".dec";
    }

    // 8. Сохраняем результат
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw new ExceptionUnableToCreateFile;
    }

    if (outputFile.write(plainData) != plainData.size()) {
        outputFile.close();
        outputFile.remove();
        throw new ExceptionUnableToWriteDecryptedTextToFile;
    }

    outputFile.close();

    //output << "Decrypted: " << filePath << " -> " << outputFilePath << "\n";
    //output << "  Size: " << plainData.size() << " bytes\n";

    return true;
}

/*
bool CryptoActions::Decrypt_File(const QString &filePath, const QString &password){
    //output<<"Decryption Function has called.\n";
    //output.flush();

    //return false;

        // 1. Check if encrypted file exists
        QFile inputFile(filePath);
        if (!inputFile.exists()) {
        Exceptions *Excp = new ExceptionFileNotFound;
        throw Excp;
        //qCritical() << "Error: Encrypted file not found:" << filePath;
        //return false;
        }

        // 2. Open encrypted file for reading
        if (!inputFile.open(QIODevice::ReadOnly)) {
        //qCritical() << "Error: Cannot open encrypted file for reading:" << filePath;
        Exceptions *Excp = new ExceptionUnableToOpenFile;
        throw Excp;
        return false;
        }

        // 3. Validate file size (must contain at least salt)
        if (inputFile.size() < SALT_LEN) {
        //qCritical() << "Error: File too small to be a valid encrypted file";
        inputFile.close();
        //return false; // EXCEPTION_INVALID_FILE_TO_DECRYPT
        Exceptions *Excp = new ExceptionFileTooSmallToDecrypt;
        throw Excp;
        }

        // 4. Read salt from the beginning of the file
        unsigned char salt[SALT_LEN];
        if (inputFile.read(reinterpret_cast<char*>(salt), SALT_LEN) != SALT_LEN) {
        //qCritical() << "Error: Failed to read salt from file";
        inputFile.close();
        //return false; // UNABLE_TO_READ_FILE
        Exceptions *Excp = new ExceptionUnableToReadSalt;
        throw Excp;
        }

        // 5. Derive key and IV from password using the same salt
        unsigned char key[KEY_LEN];
        unsigned char iv[IV_LEN];
        QByteArray passwordBytes = password.toUtf8();

        if (!PKCS5_PBKDF2_HMAC(passwordBytes.constData(), passwordBytes.size(),
                               salt, SALT_LEN,
                               ITERATIONS,
                               EVP_sha256(),
                               KEY_LEN + IV_LEN, key)) {
        //qCritical() << "Error: PBKDF2 key derivation failed";
        inputFile.close();
        //return false; // EXCEPTION_OPENSSL_PKCS5_PBKDF2_HMAC
        Exceptions *Excp = new ExceptionOpensslHMAC;
        throw Excp;
        }
        memcpy(iv, key + KEY_LEN, IV_LEN);

        // 6. Determine output filename (remove .enc extension)
        QString outputFilePath = filePath;
        if (outputFilePath.endsWith(".enc")) {
        outputFilePath.chop(4);  // Remove ".enc"
        outputFilePath += ".dec";
        } else {
        outputFilePath += ".dec";  // Fallback if no .enc extension
        }

        QFile outputFile(outputFilePath);
        if (!outputFile.open(QIODevice::WriteOnly)) {
        //qCritical() << "Error: Cannot create decrypted file:" << outputFilePath;
        inputFile.close();
        //return false;   // UNABLE_TO_CREATE_FILE
        Exceptions *Excp = new ExceptionUnableToCreateFile;
        throw Excp;
        }

        // 7. Initialize decryption context
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
        //qCritical() << "Error: Failed to create decryption context";
        inputFile.close();
        outputFile.close();
        //return false;   // EXCEPTION_OPENSSL_EVP_CIPHER_CTX_new
        Exceptions *Excp = new ExceptionOpensslCipherCTXnew;
        throw Excp;
        }

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        //qCritical() << "Error: Failed to initialize decryption";
        EVP_CIPHER_CTX_free(ctx);
        inputFile.close();
        outputFile.close();
        //return false;   //EXCEPTION_OPENSSL_EVP_DecryptInit_ex
        Exceptions *Excp = new ExceptionOpensslDecryptInit;
        throw Excp;
        }

        // 8. Read and decrypt data in chunks
        const int BUFFER_SIZE = 1024 * 16;  // 16KB buffer
        unsigned char inBuffer[BUFFER_SIZE];
        unsigned char outBuffer[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];

        int bytesRead = 0;
        int totalWritten = 0;
        int outLen = 0;

        while ((bytesRead = inputFile.read(reinterpret_cast<char*>(inBuffer), BUFFER_SIZE)) > 0) {
        if (1 != EVP_DecryptUpdate(ctx, outBuffer, &outLen, inBuffer, bytesRead)) {
            //qCritical() << "Error: Decryption failed during update";
            EVP_CIPHER_CTX_free(ctx);
            inputFile.close();
            outputFile.close(); // EXCEPTION_OPENSSL_DecryptUpdate
            Exceptions *Excp = new ExceptionOpensslDecryptUpdate;
            throw Excp;
            //return false;
        }
        if (outputFile.write(reinterpret_cast<const char*>(outBuffer), outLen) != outLen) {
            //qCritical() << "Error: Failed to write decrypted data";
            EVP_CIPHER_CTX_free(ctx);
            inputFile.close();
            outputFile.close(); // UNABLE_TO_WRITE_TO_FILE
            //return false;
            Exceptions *excp = new ExceptionUnableToWriteDecryptedTextToFile;
            throw excp;
        }
        totalWritten += outLen;
        }

        // 9. Finalize decryption (removes padding)
        if (1 != EVP_DecryptFinal_ex(ctx, outBuffer, &outLen)) {
        //qCritical() << "Error: Decryption finalization failed";
        //qCritical() << "       Possible wrong password or corrupted file";
        EVP_CIPHER_CTX_free(ctx);
        inputFile.close();
        outputFile.close();
        outputFile.remove();  // Remove incomplete output file
        //return false;   //
        Exceptions *Excp = new ExceptionOpensslDecryptFinal;
        throw Excp;
        }

        if (outLen > 0) {
        if (outputFile.write(reinterpret_cast<const char*>(outBuffer), outLen) != outLen) {
            //qCritical() << "Error: Failed to write final decrypted block";
            EVP_CIPHER_CTX_free(ctx);
            inputFile.close();
            outputFile.close();
            outputFile.remove();
            //return false;
            Exceptions *Excp = new ExceptionUnableToWriteFinalDataToFile;
            throw Excp;
        }
        totalWritten += outLen;
        }

        // 10. Cleanup
        EVP_CIPHER_CTX_free(ctx);
        inputFile.close();
        outputFile.close();

        output << "Decryption successful!\n";
        output << "  Encrypted file:" << filePath<<'\n';
        output << "  Decrypted file:" << outputFilePath<<'\n';
        output << "  Output size:" << totalWritten << "bytes\n";

        return true;
}


*/


/*CryptoActions &CryptoActions::Get_Object(){
    //static CryptoActions Object;
    //return Object;
}
QByteArray MakeKeyFromPassword(const QString &password){

    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
}

*/
/*
bool EncryptFile(const QString &filePath, const QString &password){
    {
        output<<"Encryption...\n";
        output.flush();
    }
}

*/
