
include(../../HmiFuncDesignerLibrary.pri)

QT += core
QT += sql

CONFIG -= shared dll
CONFIG += staticlib

INCLUDEPATH += orm/core \
    orm/utils

SOURCES += \
    $$PWD/TagSys.cpp \
    $$PWD/TagTmp.cpp \
    $$PWD/TagIO.cpp \
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
    $$PWD/PictureResourceManager.cpp \
    orm/core/databaseconfiguration.cpp \
    orm/core/existingobjectsavestate.cpp \
    orm/core/introspection.cpp \
    orm/core/newobjectsavestate.cpp \
    orm/core/qpersistantobject.cpp \
    orm/core/qtpersistence.cpp \
    orm/utils/inflection.cpp


HEADERS += \
    $$PWD/DBTag.h \
    $$PWD/TagSys.h \
    $$PWD/TagTmp.h \
    $$PWD/TagIO.h \
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
    $$PWD/PictureResourceManager.h \
    orm/core/databaseconfiguration_p.h \
    orm/core/existingobjectsavestate.h \
    orm/core/introspection.h \
    orm/core/newobjectsavestate.h \
    orm/core/objectloader.h \
    orm/core/objectsavestate.h \
    orm/core/qpersistantobject.h \
    orm/core/qpersistantobject_p.h \
    orm/core/qtpersistence.h \
    orm/utils/inflection.h \
    orm/utils/inflection_p.h



