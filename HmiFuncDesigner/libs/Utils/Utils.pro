
include(../../HmiFuncDesignerLibrary.pri)

QT += network
QT += widgets
QT += network
QT += core
QT += sql

CONFIG -= shared dll
CONFIG += staticlib

#去除UNICODE字符编码
DEFINES -= UNICODE

SOURCES += \
    ConfigUtils.cpp \
    Helper.cpp \
    MiscUtils.cpp \
    ShareMemory.cpp \
    StyleSheetHelper.cpp \
    TimeElapsed.cpp \
    XMLNodeObject.cpp

HEADERS += \
    ConfigUtils.h \
    Helper.h \
    MiscUtils.h \
    ObjectCreator.h \
    ShareMemory.h \
    Singleton.h \
    StyleSheetHelper.h \
    TimeElapsed.h \
    XMLNodeObject.h


