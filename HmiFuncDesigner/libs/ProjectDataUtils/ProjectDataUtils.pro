
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
    $$PWD/orm/core/databaseconfiguration.cpp \
    $$PWD/orm/core/existingobjectsavestate.cpp \
    $$PWD/orm/core/introspection.cpp \
    $$PWD/orm/core/newobjectsavestate.cpp \
    $$PWD/orm/core/qpersistantobject.cpp \
    $$PWD/orm/core/qtpersistence.cpp \
    $$PWD/orm/utils/inflection.cpp \
    $$PWD/../Utils/XMLObject.cpp \
    $$PWD/../Utils/Helper.cpp


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
    $$PWD/orm/core/databaseconfiguration_p.h \
    $$PWD/orm/core/existingobjectsavestate.h \
    $$PWD/orm/core/introspection.h \
    $$PWD/orm/core/newobjectsavestate.h \
    $$PWD/orm/core/objectloader.h \
    $$PWD/orm/core/objectsavestate.h \
    $$PWD/orm/core/qpersistantobject.h \
    $$PWD/orm/core/qpersistantobject_p.h \
    $$PWD/orm/core/qtpersistence.h \
    $$PWD/orm/utils/inflection.h \
    $$PWD/orm/utils/inflection_p.h \
    $$PWD/../Utils/XMLObject.h \
    $$PWD/../Utils/Helper.h



