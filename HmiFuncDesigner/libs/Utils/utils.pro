
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
    configutils.cpp \
    helper.cpp \
    miscutils.cpp \
    sharememory.cpp \
    stylesheethelper.cpp \
    timeelapsed.cpp \
    xmlnodeobject.cpp

HEADERS += \
    configutils.h \
    helper.h \
    miscutils.h \
    objectcreator.h \
    sharememory.h \
    singleton.h \
    stylesheethelper.h \
    timeelapsed.h \
    xmlnodeobject.h


