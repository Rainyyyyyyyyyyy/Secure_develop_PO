QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/FolderTraveler.cpp \
        src/CryptoLib.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1/lib_Cryptopp_unpacked/cryptopp
INCLUDEPATH += E:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1/lib_Cryptopp_unpacked/cryptopp
# INCLUDEPATH += "C:\msys64\ucrt64\include\cryptopp"
#LIBS += -LC:/msys64/ucrt64/lib -lcryptopp
LIBS += -LE:/Z_vsyakoe_dla_echeby/4k2sem/SEcure_Develop_PO(Andreeva)/laba1/lib_Cryptopp_unpacked/cryptopp -lcryptopp
# Путь к заголовкам Crypto++ (установленным через MSYS2)
#INCLUDEPATH += C:/msys64/mingw64/include/cryptopp





# Путь к заголовкам OpenSSL
INCLUDEPATH += E:/Qt/Qt/Tools/mingw810_64/opt/include
# Путь к заголовкам OpenSSL
# INCLUDEPATH += C:/msys64/ucrt64/include

# Путь к библиотекам
#LIBS += -LC:/Qt/Tools/mingw810_64/opt/lib -lcrypto -lssl
LIBS +=  -LE:/Qt/Qt/Tools/mingw810_64/opt/lib -lcrypto -lssl

HEADERS += \
    CryptoLibExceptions.h \
    InterfaceCryptoLib.h \
    CryptoLib.h \
    FolderTraveler.h
