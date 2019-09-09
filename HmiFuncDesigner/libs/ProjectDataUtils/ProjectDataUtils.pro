
include(../../HmiFuncDesignerLibrary.pri)

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
    $$PWD/ProjectInfoManager.cpp \
    $$PWD/NetSetting.cpp \
    $$PWD/DatabaseSetting.cpp \
    $$PWD/UserAuthority.cpp \
    $$PWD/DeviceInfo.cpp \
    $$PWD/PictureResourceManager.cpp


HEADERS += \ 
    $$PWD/Database.h \
    $$PWD/SQLiteDatabase.h \
    $$PWD/ulog.h \
    $$PWD/ProjectData.h \
    $$PWD/ProjectDataSQLiteDatabase.h \
    $$PWD/ProjectInfoManager.h \
    $$PWD/NetSetting.h \
    $$PWD/DatabaseSetting.h \
    $$PWD/UserAuthority.h \
    $$PWD/DeviceInfo.h \
    $$PWD/PictureResourceManager.h


