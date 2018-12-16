
include(../QStudioSCADA.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SysVariableManager
TEMPLATE = app


DESTDIR = $$IDE_BIN_PATH

#CONFIG += debug_and_release
CONFIG += release

CONFIG(debug, debug|release) { #debug
    #LIBS += -L../../build-QStudioSCADA/bin -lQtPropertyBrowserd
} else { # release
    #LIBS += -L../../build-QStudioSCADA/bin -lQtPropertyBrowser
}

UI_DIR=../../build-QStudioSCADA/SysVariableManager/ui
MOC_DIR=../../build-QStudioSCADA/SysVariableManager/moc
OBJECTS_DIR=../../build-QStudioSCADA/SysVariableManager/obj
RCC_DIR=../../build-QStudioSCADA/SysVariableManager/rcc


SOURCES += main.cpp \
    SysVariable.cpp

HEADERS  += \
    SysVariable.h

FORMS    += \
    SysVariable.ui

RESOURCES += \
    images.qrc

RC_FILE = SysVariableManager.rc
