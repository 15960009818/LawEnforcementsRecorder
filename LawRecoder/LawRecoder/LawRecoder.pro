#-------------------------------------------------
#
# Project created by QtCreator 2024-09-09T15:58:08
#
#-------------------------------------------------

QT       += core gui\
            multimedia\
            widgets\

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
    common/fdecodeplay.cpp \
    controller/indexcontroller.cpp \
    common/recordingunit.cpp \
    win/indexwin.cpp \
    common/recordthread.cpp \
    win/settingwin.cpp \
    controller/settingcontroller.cpp \
    win/imagecapturewin.cpp \
    service/deviceservice.cpp \
    controller/imagecapturecontroller.cpp \
    dao/picturedao.cpp \
    service/imagecaptureservice.cpp \
    win/imagemessagewin.cpp \
    win/videocapturewin.cpp \
    controller/videocapturecontroller.cpp \
    service/videocaptureservice.cpp \
    dao/videodao.cpp

HEADERS += \
        widget.h \
    common/verificationcodelabel.h \
    common/singleton.h \
    common/sqlite3crud.h \
    win/loginwin.h \
    controller/logincontroller.h \
    service/loginservice.h \
    common/commondefs.h \
    common/fdecodeplay.h \
    controller/indexcontroller.h \
    win/indexwin.h \
    common/recordingunit.h \
    common/recordthread.h \
    win/settingwin.h \
    controller/settingcontroller.h \
    win/imagecapturewin.h \
    service/deviceservice.h \
    controller/imagecapturecontroller.h \
    dao/picturedao.h \
    service/imagecaptureservice.h \
    win/imagemessagewin.h \
    win/videocapturewin.h \
    controller/videocapturecontroller.h \
    service/videocaptureservice.h \
    dao/videodao.h

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
INCLUDEPATH +=D:/QT/opencv_3.4.2_Qt/include
LIBS +=D:/QT/opencv_3.4.2_Qt/x86/bin/libopencv_*.dll \
D:/QT/opencv_3.4.2_Qt/x86/bin/opencv_ffmpeg342.dll
