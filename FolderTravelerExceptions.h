#ifndef FOLDERTRAVELEREXCEPTIONS_H
#define FOLDERTRAVELEREXCEPTIONS_H

#endif // FOLDERTRAVELEREXCEPTIONS_H

#include "IExceptions.h"


#define EXCEPTION_FOLDER_NOT_FOULD 10001
#define EXCEPTION_FOLDER_FROM_DISK_C 10002



/* Исключение: папка не найдена */
class ExceptionFolderNotFould : public CustomExceptions {
public:
    ExceptionFolderNotFould() : CustomExceptions(EXCEPTION_FOLDER_NOT_FOULD, "Error: folder not found!"){}
};

/* Исключение: папка из диска С */
class ExceptionFolderFromDiskC : public CustomExceptions {
public:
    ExceptionFolderFromDiskC() : CustomExceptions(EXCEPTION_FOLDER_FROM_DISK_C, "Warning! Folder from disk C!"){}
};

