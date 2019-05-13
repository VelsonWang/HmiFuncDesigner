
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
    Database.cpp \
    Helper.cpp \
    MiscUtils.cpp \
    MySQLDatabase.cpp \
    SQLiteDatabase.cpp \
    StyleSheetHelper.cpp \
    ulog.cpp \
    XMLObject.cpp

HEADERS += \
    ConfigUtils.h \
    Database.h \
    Helper.h \
    MiscUtils.h \
    MySQLDatabase.h \
    ObjectCreator.h \
    Singleton.h \
    SQLiteDatabase.h \
    StyleSheetHelper.h \
    ulog.h \
    XMLObject.h


