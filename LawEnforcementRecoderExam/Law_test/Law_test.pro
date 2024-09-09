QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_law_test.cpp \
    ../LawEnforcementRecoder/LoginWin/loginwin.cpp \
    ../LawEnforcementRecoder/common/sqlite3crud.cpp \
    ../LawEnforcementRecoder/common/verificationcodelabel.cpp
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += $$PWD/lib/sqlite3.lib

HEADERS += \
    ../LawEnforcementRecoder/LoginWin/loginwin.h \
    ../LawEnforcementRecoder/common/singleton.h \

