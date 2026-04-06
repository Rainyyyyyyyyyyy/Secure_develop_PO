#ifndef FOLDERTRAVELEREXCEPTIONS_H
#define FOLDERTRAVELEREXCEPTIONS_H

#endif // FOLDERTRAVELEREXCEPTIONS_H

#include "IExceptions.h"


#define EXCEPTION_FOLDER_NOT_FOULD 10001
#define EXCEPTION_FOLDER_FROM_DISK_C 10002



/* Исключение: папка не найдена */
class ExceptionFolderNotFould : public Exceptions {
public:
    ExceptionFolderNotFould() : Exceptions(EXCEPTION_FOLDER_NOT_FOULD){}

    const char * what() const noexcept override{
        return "Error: folder not found!";
    }
};

/* Исключение: папка из диска С */
class ExceptionFolderFromDiskC : public Exceptions {
public:
    ExceptionFolderFromDiskC() : Exceptions(EXCEPTION_FOLDER_FROM_DISK_C){}

    const char * what() const noexcept override{
        return "Warning! Folder from disk C!";
    }
};

