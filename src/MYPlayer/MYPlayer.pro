# Project Type
TEMPLATE = app

# Qt modules that are used by your project
QT += qml quick gui widgets multimedia opengl openglextensions

# Project configuration and compiler options
CONFIG += qt warn_on c++11 rtti stl thread exceptions

# Directory where all intermediate objects and moc files should be placed
CONFIG(debug, debug|release) {
    OBJECTS_DIR = ./tmp/debug
    MOC_DIR = ./tmp/debug
} else {
    OBJECTS_DIR = ./tmp/release
    MOC_DIR = ./tmp/release
}

# Directory where all intermediate files from uic should be placed
CONFIG(debug, debug|release) {
    UI_DIR = ./tmp/debug
} else {
    UI_DIR = ./tmp/release
}

# Directory for Qt Resource Compiler output files
CONFIG(debug, debug|release) {
    RCC_DIR = ./tmp/debug
} else {
    RCC_DIR = ./tmp/release
}

# Specifies where to put the target file
CONFIG(debug, debug|release) {
    contains(QMAKE_TARGET.arch, x86_64) {
        DESTDIR = $$_PRO_FILE_/../../../bin/debug/x64
    } else {
        DESTDIR = $$_PRO_FILE_/../../../bin/debug/x86
    }
} else {
    contains(QMAKE_TARGET.arch, x86_64) {
        DESTDIR = $$_PRO_FILE_/../../../bin/release/x64
    } else {
        DESTDIR = $$_PRO_FILE_/../../../bin/release/x86
    }
}

# Name of the target file
TARGET = MYPlayer

# Precompiled header file(PCH)
#PRECOMPILED_HEADER = stdafx.h

# Name of the resource collection files (qrc) for the target
RESOURCES += resource/MYPlayer.qrc
#RESOURCES += qml.qrc

# Codec configuration
CODECFORTR = UTF-8
CODECFORSRC = UTF-8

# Source files which contains strings for i18n
lupdate_only {
    SOURCES += resource/ui/qml/*.qml \
              resource/ui/qml/*.js
}

# Translation file path
TRANSLATIONS += ./resource/ui/translation/MYPlayer_zh_CN.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) {
#    DEFINES +=
} else {
#    DEFINES +=
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Source files in the project
SOURCES += \
        MYAudioPlay.cpp \
        MYAudioThread.cpp \
        MYDecode.cpp \
        MYDecodeThread.cpp \
        MYDemux.cpp \
        MYDemuxThread.cpp \
        MYResample.cpp \
        MYVideoThread.cpp \
        MYVideoOutput.cpp \
        MYPlay.cpp \
        main.cpp \
        MainApp.cpp \
        MYSubTitle.cpp \

# Header files for the project
HEADERS += MainApp.h \
    IVideoCall.h \
    MYAudioPlay.h \
    MYAudioThread.h \
    MYDecode.h \
    MYDecodeThread.h \
    MYDemux.h \
    MYDemuxThread.h \
    MYResample.h \
    MYVideoThread.h \
    MYVideoOutput.h \
    MYPlay.h \
    MYSubTitle.h \

# Include path
INCLUDEPATH += ../../include

# Libaray path and libaray
CONFIG(debug, debug|release) {
    contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L"$$PWD/../../lib/debug/x64/"
    } else {
        LIBS += -L"$$PWD/../../lib/debug/x86/"
    }
#    win32:LIBS += libqrencode.lib\
#                  libzint.lib
#    unix:LIBS += -lqrencode\
#                 -lzint
} else {
    contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L"$$PWD/../../lib/release/x64/"
    } else {
        LIBS += -L"$$PWD/../../lib/release/x86/"
    }
#    win32:LIBS += libqrencode.lib\
#                  libzint.lib
#    unix:LIBS += -lqrencode\
#                 -lzint
}
win32:LIBS += avformat.lib\
              avcodec.lib\
              avutil.lib\
              swresample.lib\
              swscale.lib

################################################################################
# Windows platform
win32 {
    RC_FILE = win32/MYPlayer.rc
    HEADERS += win32/targetver.h \
               win32/resource.h
    OTHER_FILES += win32/MYPlayer.rc
}

################################################################################
# Linux platform
linux {
}

################################################################################
# Mac OS X platform
macx {
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
