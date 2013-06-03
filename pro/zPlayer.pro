#-------------------------------------------------
#
# Project created by QtCreator 2013-06-03T10:58:53
#
#-------------------------------------------------

QT       += core gui

CONFIG(debug, debug|release) {
    TARGET = zPlayerd
} else {
    TARGET = zPlayer
}

TEMPLATE = app

LIBS += \
    ../lib/Ffmpeg/libavcodec.dll.a \
    ../lib/Ffmpeg/libavformat.dll.a \
    ../lib/AVIMUXDLL.lib

SOURCES += \
    ../src/main.cpp\
    ../src/mainwindow.cpp

INCLUDEPATH += \
    ../include

HEADERS  += \
    ../include/mainwindow.h

FORMS    += \
    ../ui/mainwindow.ui

DESTDIR       = ../tmp/$$TARGET
OBJECTS_DIR   = ../tmp/$$TARGET/obj
MOC_DIR       = ../tmp/$$TARGET/moc
UI_DIR        = ../tmp/$$TARGET/ui

win32:target_bin.files    = $$DESTDIR/*.exe

target_bin.path     = ../bin

INSTALLS     += target_bin

TRANSLATIONS +=../ts/zh_cn.ts

DEPENDPATH *= $${INCLUDEPATH}
