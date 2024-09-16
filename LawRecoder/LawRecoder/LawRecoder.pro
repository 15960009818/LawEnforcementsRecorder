#-------------------------------------------------
#
# Project created by QtCreator 2024-09-09T15:58:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LawRecoder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    common/verificationcodelabel.cpp \
    common/sqlite3crud.cpp \
    win/loginwin.cpp \
    controller/logincontroller.cpp \
    service/loginservice.cpp \
    common/fdecodeplay.cpp

HEADERS += \
        widget.h \
    common/verificationcodelabel.h \
    common/singleton.h \
    common/sqlite3crud.h \
    win/loginwin.h \
    controller/logincontroller.h \
    service/loginservice.h \
    common/commondefs.h \
    common/fdecodeplay.h

INCLUDEPATH +=$$PWD/ffmpeglib/include
LIBS += $$PWD/ffmpeglib/lib/avcodec.lib \
        $$PWD/ffmpeglib/lib/avdevice.lib \
        $$PWD/ffmpeglib/lib/avfilter.lib \
        $$PWD/ffmpeglib/lib/avformat.lib \
        $$PWD/ffmpeglib/lib/avutil.lib \
        $$PWD/ffmpeglib/lib/postproc.lib \
        $$PWD/ffmpeglib/lib/swresample.lib \
        $$PWD/ffmpeglib/lib/swscale.lib
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += $$PWD/lib/sqlite3.lib
