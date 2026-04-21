#include "CryptoLib.h"


#include <iostream>
#include <cstring>

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

}




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
    return memcmp(magic, Signature_Sequence.constData(), SIGN_LEN) == 0;
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

    cipherData.resize(totalLen);

    EVP_CIPHER_CTX_free(ctx);

    QByteArray finalData;
    finalData.append(Signature_Sequence.constData(), SIGN_LEN);
    finalData.append(reinterpret_cast<const char*>(salt), SALT_LEN);
    finalData.append(reinterpret_cast<const char*>(verifier), HASH_LEN);
    finalData.append(cipherData);

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
//  [SIGN][SALT][VERIFIER][CIPHERTEXT]
bool CryptoActions::Decrypt_File(const QString &filePath, const QString &password) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw new ExceptionUnableToOpenFile;
    }

    if (file.size() < (SALT_LEN + SIGN_LEN + HASH_LEN)) {
        file.close();
        throw new ExceptionFileTooSmall;
    }

    if(!IsFileEncrypted(filePath)){
        throw new ExceptionFileIsAlreadyDecrypted;
    }

    QByteArray encryptedData = file.readAll();
    file.close();

    unsigned char salt[SALT_LEN];
    unsigned char verifierFromFile[HASH_LEN];
    memcpy(salt, encryptedData.constData() + SIGN_LEN, SALT_LEN);
    memcpy(verifierFromFile, encryptedData.constData() + SIGN_LEN + SALT_LEN, HASH_LEN);

    QByteArray derived = deriveKeyMaterial(password, salt);
    if (derived.isEmpty()) {
        throw new ExceptionOpensslHMAC;
    }

    const unsigned char *derivedBytes = reinterpret_cast<const unsigned char*>(derived.constData());
    const unsigned char *key = derivedBytes;
    const unsigned char *iv = derivedBytes + KEY_LEN;
    const unsigned char *verifier = derivedBytes + KEY_LEN + IV_LEN;

    if (memcmp(verifierFromFile, verifier, HASH_LEN) != 0) {
        throw new ExceptionIncorrectPassword;
    }

    QByteArray cipherData = encryptedData.mid(SIGN_LEN + SALT_LEN + HASH_LEN);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw new ExceptionOpensslCipherCTXnew;
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                const_cast<unsigned char*>(key),
                                const_cast<unsigned char*>(iv))) {
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

    QString outputFilePath = filePath;
    if (outputFilePath.endsWith(".enc")) {
        outputFilePath.chop(4);
    } else {
        //outputFilePath += ".dec";
    }

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
