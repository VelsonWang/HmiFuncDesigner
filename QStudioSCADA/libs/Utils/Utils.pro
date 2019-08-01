
include(../../QStudioSCADAlibrary.pri)

QT += network
QT += widgets
QT += network
QT += core
QT += sql

CONFIG -= shared dll
CONFIG += staticlib

SOURCES += \
    ConfigUtils.cpp \
    Helper.cpp \
    MiscUtils.cpp \
    StyleSheetHelper.cpp \
    TimeElapsed.cpp \
    XMLObject.cpp

HEADERS += \
    ConfigUtils.h \
    Helper.h \
    MiscUtils.h \
    ObjectCreator.h \
    Singleton.h \
    StyleSheetHelper.h \
    TimeElapsed.h \
    XMLObject.h


