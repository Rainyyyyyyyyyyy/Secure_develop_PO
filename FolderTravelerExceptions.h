#ifndef FOLDERTRAVELEREXCEPTIONS_H
#define FOLDERTRAVELEREXCEPTIONS_H

#endif // FOLDERTRAVELEREXCEPTIONS_H

#include "IExceptions.h"


#define EXCEPTION_FOLDER_NOT_FOULD 10001
#define EXCEPTION_FOLDER_FROM_DISK_C 10002
#define EXCEPTION_FOLDER_IS_EMPTY 10003
#define EXCEPTION_PATH_DOT_OR_DOTDOT 10004
#define EXCEPTION_PATH_FROM_SYSTEM_ENTRIES 10005


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

/* Исключение: папка пуста */
class ExceptionFolderIsEmpty : public CustomExceptions {
public:
    ExceptionFolderIsEmpty() : CustomExceptions(EXCEPTION_FOLDER_IS_EMPTY, "Warning! Folder is empty!") {}
};

/* Исключение: путь введён как '.' или '..' */
class ExceptionDotOrDotDot : public CustomExceptions {
public:
    ExceptionDotOrDotDot() : CustomExceptions(EXCEPTION_PATH_DOT_OR_DOTDOT, "Warning! Path contains '.' or '..'!") {}
};

/* Исключение: путь к папке внутри системного пространства */
class ExceptionPathFromSystemEntries : public CustomExceptions {
public:
    ExceptionPathFromSystemEntries() : CustomExceptions(EXCEPTION_PATH_FROM_SYSTEM_ENTRIES, "Warning! Path from system space!") {}
};


