QT += core gui widgets script xml

TARGET = ProjData
TEMPLATE = lib

BUILD_DIR = $$clean_path($$OUT_PWD)
#message("ProjData BUILD_DIR: $$BUILD_DIR")

DESTDIR = $$BUILD_DIR/../../../lib
#message("ProjData DESTDIR: $$clean_path($$DESTDIR)")
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiFuncDesignerBin/bin
    #message("ProjData DLLDESTDIR: $$clean_path($$DLLDESTDIR)")
}

DEFINES += PROJDATA_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD

SOURCES += \
    DataAES.cpp \
    Tag.cpp \
    ulog.cpp \
    ProjectData.cpp \
    ProjectInfoManager.cpp \
    NetSetting.cpp \
    DatabaseSetting.cpp \
    UserAuthority.cpp \
    DeviceInfo.cpp \
    PictureResourceManager.cpp \
    Script.cpp \
    XMLObject.cpp \
    Helper.cpp


HEADERS += \
    DataAES.h \
    projdata_global.h \
    IGraphPageSaveLoad.h \
    Tag.h \
    ulog.h \
    ProjectData.h \
    ProjectInfoManager.h \
    NetSetting.h \
    DatabaseSetting.h \
    UserAuthority.h \
    DeviceInfo.h \
    PictureResourceManager.h \
    Script.h \
    XMLObject.h \
    Helper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

