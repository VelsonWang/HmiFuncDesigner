
include(../QStudioSCADA.pri)

QT += core gui
QT += widgets

TARGET = SysTagManager
TEMPLATE = app


DESTDIR = $$IDE_BIN_PATH

SOURCES += main.cpp \
    SysTag.cpp

HEADERS  += \
    SysTag.h

FORMS    += \
    SysTag.ui

RESOURCES += \
    images.qrc

RC_FILE = SysTagManager.rc
