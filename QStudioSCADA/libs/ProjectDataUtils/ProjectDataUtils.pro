
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
    ProjectInfoManger.cpp


HEADERS += \ 
    $$PWD/Database.h \
    $$PWD/SQLiteDatabase.h \
    $$PWD/ulog.h \
    $$PWD/ProjectData.h \
    $$PWD/ProjectDataSQLiteDatabase.h \
    ProjectInfoManger.h


