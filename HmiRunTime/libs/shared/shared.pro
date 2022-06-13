QT += core gui widgets xml script sql

TARGET = shared
TEMPLATE = lib

BUILD_DIR = $$clean_path($$OUT_PWD)

DESTDIR = $$BUILD_DIR/../../../lib_rt
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiRunTimeBin
}

DEFINES += SHARED_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/projdata

include(picoc/picoc.pri)

HEADERS += \
    confighelper.h \
    public.h \
    publicfunction.h \
    pubtool.h \
    qwidgetfactory.h \
    realtimedb.h \
    sharedlibglobal.h \
    widgets/iloader.h \
    widgets/keyboard/inputmethodnumber.h \
    widgets/qformwidget.h \
    widgets/qimagebox.h \
    widgets/qindicationlamp.h \
    widgets/qjarshape.h \
    widgets/qlabelex.h \
    widgets/qlineeditex.h \
    widgets/qpushbuttonex.h \
    widgets/qswitchbutton.h \
    widgets/qvaluestick.h \
    xmlobject.h \
    qcommonstruct.h \
    qabstractplugin.h \
    pluginloader.h \
    qprojectcore.h \
    qbaseinit.h \
    qpagemanager.h \
    qabstractupdateplugin.h \
    qstylesheetinfo.h

SOURCES += \
    confighelper.cpp \
    pubtool.cpp \
    qwidgetfactory.cpp \
    realtimedb.cpp \
    widgets/iloader.cpp \
    widgets/keyboard/inputmethodnumber.cpp \
    widgets/qformwidget.cpp \
    widgets/qimagebox.cpp \
    widgets/qindicationlamp.cpp \
    widgets/qjarshape.cpp \
    widgets/qlabelex.cpp \
    widgets/qlineeditex.cpp \
    widgets/qpushbuttonex.cpp \
    widgets/qswitchbutton.cpp \
    widgets/qvaluestick.cpp \
    xmlobject.cpp \
    qabstractplugin.cpp \
    pluginloader.cpp \
    qprojectcore.cpp \
    qbaseinit.cpp \
    qpagemanager.cpp \
    qabstractupdateplugin.cpp \
    qstylesheetinfo.cpp

FORMS += \
    widgets/keyboard/inputmethodnumber.ui

RESOURCES += \
    shared.qrc

#DISTFILES += qabstractplugin.json


################################################################################

INCLUDEPATH += $$PWD/projdata

SOURCES += \
    projdata/dataaes.cpp \
    projdata/tag.cpp \
    projdata/ulog.cpp \
    projdata/projectinfomanager.cpp \
    projdata/netsetting.cpp \
    projdata/databasesetting.cpp \
    projdata/userauthority.cpp \
    projdata/deviceinfo.cpp \
    projdata/pictureresourcemanager.cpp \
    projdata/script.cpp \
    projdata/helper.cpp


HEADERS += \
    projdata/dataaes.h \
    projdata/tag.h \
    projdata/ulog.h \
    projdata/projectinfomanager.h \
    projdata/netsetting.h \
    projdata/databasesetting.h \
    projdata/userauthority.h \
    projdata/deviceinfo.h \
    projdata/pictureresourcemanager.h \
    projdata/script.h \
    projdata/helper.h

################################################################################

HEADERS += \
    qrunningmanager.h \
    qbasedialogwidget.h \
    qbasedialog.h

SOURCES += \
    qrunningmanager.cpp \
    qbasedialogwidget.cpp \
    qbasedialog.cpp
