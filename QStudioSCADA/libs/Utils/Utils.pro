

include(../../QStudioSCADAlibrary.pri)

QT += network

CONFIG -= shared dll
CONFIG += staticlib

SOURCES += MiscUtils.cpp

HEADERS += MiscUtils.h \
    Singleton.h


