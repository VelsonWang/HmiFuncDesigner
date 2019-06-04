
include(../../QStudioSCADAlibrary.pri)

QT += core
QT += sql

CONFIG -= shared dll
CONFIG += staticlib

SOURCES += \ 
    $$PWD/Database.cpp \
    $$PWD/SQLiteDatabase.cpp \
    $$PWD/ulog.cpp \
    $$PWD/ProjectData.cpp \
    $$PWD/ProjectDataSQLiteDatabase.cpp \
    $$PWD/ProjectInfoManger.cpp \
    $$PWD/NetSetting.cpp \
    $$PWD/DatabaseSetting.cpp \
    $$PWD/UserAuthority.cpp


HEADERS += \ 
    $$PWD/Database.h \
    $$PWD/SQLiteDatabase.h \
    $$PWD/ulog.h \
    $$PWD/ProjectData.h \
    $$PWD/ProjectDataSQLiteDatabase.h \
    $$PWD/ProjectInfoManger.h \
    $$PWD/NetSetting.h \
    $$PWD/DatabaseSetting.h \
    $$PWD/UserAuthority.h


