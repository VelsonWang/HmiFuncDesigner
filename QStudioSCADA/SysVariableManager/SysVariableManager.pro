
include(../QStudioSCADA.pri)

QT += core gui
QT += widgets

TARGET = SysVariableManager
TEMPLATE = app


DESTDIR = $$IDE_BIN_PATH

SOURCES += main.cpp \
    SysVariable.cpp

HEADERS  += \
    SysVariable.h

FORMS    += \
    SysVariable.ui

RESOURCES += \
    images.qrc

RC_FILE = SysVariableManager.rc
