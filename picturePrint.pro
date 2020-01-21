VERSION = 0.0.1.0
QMAKE_TARGET_COMPANY = WIN-DESIGN
QMAKE_TARGET_PRODUCT = picturePrint
QMAKE_TARGET_DESCRIPTION = picturePrint
QMAKE_TARGET_COPYRIGHT = (c) 2020 WIN-DESIGN

QT       += core gui sql multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

win32-msvc* {
    contains(QT_ARCH, i386) {
        message("msvc 32-bit")
    } else {
        message("msvc 64-bit")
    }
}

win32-g++ {
    message("mingw")
    INCLUDEPATH += C:\dev\3rdParty\exiv2\include C:\dev\3rdParty\libraw C:\dev\3rdParty\libjpeg\include C:\dev\3rdParty\opencv\include dng
    LIBS += -LC:\dev\3rdParty\exiv2\lib -lexiv2.dll -LC:\dev\3rdParty\libraw\lib -LC:\dev\3rdParty\opencv\x64\mingw\lib -lraw -lws2_32 -lz -lopencv_core412.dll -lopencv_imgproc412.dll
}

unix {
    message("*nix")
    LIBS += -lraw -lexiv2
}

QMAKE_CXXFLAGS += -DLIBRAW_NODLL -DLIBRAW_NOTHREADS

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cexif.cpp \
    cfilebrowser.cpp \
    cfileviewer.cpp \
    cimage.cpp \
    common.cpp \
    csplashscreen.cpp \
    main.cpp \
    cmainwindow.cpp

HEADERS += \
    cexif.h \
    cfilebrowser.h \
    cfileviewer.h \
    cimage.h \
    cmainwindow.h \
    common.h \
    csplashscreen.h

FORMS += \
    cfilebrowser.ui \
    cfileviewer.ui \
    cmainwindow.ui

TRANSLATIONS += \
    picturePrint_de_AT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pictureprint.qrc \
    qdarkstyle/style.qrc

DISTFILES += \
    picturePrint.db
