
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/projdata
INCLUDEPATH += $$PWD/widgets

include(picoc/picoc.pri)

HEADERS += \
    $$PWD/confighelper.h \
    $$PWD/public.h \
    $$PWD/publicfunction.h \
    $$PWD/pubtool.h \
    $$PWD/qwidgetfactory.h \
    $$PWD/realtimedb.h \
    $$PWD/sharedlibglobal.h \
    $$PWD/widgets/iloader.h \
    $$PWD/widgets/qformwidget.h \
    $$PWD/widgets/qimagebox.h \
    $$PWD/widgets/qindicationlamp.h \
    $$PWD/widgets/qjarshape.h \
    $$PWD/widgets/qlabelex.h \
    $$PWD/widgets/qlineeditex.h \
    $$PWD/widgets/qpushbuttonex.h \
    $$PWD/widgets/qswitchbutton.h \
    $$PWD/widgets/qvaluestick.h \
    $$PWD/xmlobject.h \
    $$PWD/qcommonstruct.h \
    $$PWD/qabstractplugin.h \
    $$PWD/pluginloader.h \
    $$PWD/qprojectcore.h \
    $$PWD/qbaseinit.h \
    $$PWD/qpagemanager.h \
    $$PWD/qblinkwidgettimer.h \
    $$PWD/qabstractupdateplugin.h \
    $$PWD/qstylesheetinfo.h \
    $$PWD/mainwindow.h \
    $$PWD/keyboardnumber.h

SOURCES += \
    $$PWD/confighelper.cpp \
    $$PWD/pubtool.cpp \
    $$PWD/qwidgetfactory.cpp \
    $$PWD/realtimedb.cpp \
    $$PWD/widgets/iloader.cpp \
    $$PWD/widgets/qformwidget.cpp \
    $$PWD/widgets/qimagebox.cpp \
    $$PWD/widgets/qindicationlamp.cpp \
    $$PWD/widgets/qjarshape.cpp \
    $$PWD/widgets/qlabelex.cpp \
    $$PWD/widgets/qlineeditex.cpp \
    $$PWD/widgets/qpushbuttonex.cpp \
    $$PWD/widgets/qswitchbutton.cpp \
    $$PWD/widgets/qvaluestick.cpp \
    $$PWD/xmlobject.cpp \
    $$PWD/qabstractplugin.cpp \
    $$PWD/pluginloader.cpp \
    $$PWD/qprojectcore.cpp \
    $$PWD/qbaseinit.cpp \
    $$PWD/qpagemanager.cpp \
    $$PWD/qblinkwidgettimer.cpp \
    $$PWD/qabstractupdateplugin.cpp \
    $$PWD/qstylesheetinfo.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/keyboardnumber.cpp

FORMS += \
    $$PWD/keyboardnumber.ui


SOURCES += \
    $$PWD/projdata/dataaes.cpp \
    $$PWD/projdata/tag.cpp \
    $$PWD/projdata/projectinfomanager.cpp \
    $$PWD/projdata/netsetting.cpp \
    $$PWD/projdata/databasesetting.cpp \
    $$PWD/projdata/userauthority.cpp \
    $$PWD/projdata/deviceinfo.cpp \
    $$PWD/projdata/pictureresourcemanager.cpp \
    $$PWD/projdata/script.cpp \
    $$PWD/projdata/helper.cpp


HEADERS += \
    $$PWD/projdata/dataaes.h \
    $$PWD/projdata/tag.h \
    $$PWD/projdata/projectinfomanager.h \
    $$PWD/projdata/netsetting.h \
    $$PWD/projdata/databasesetting.h \
    $$PWD/projdata/userauthority.h \
    $$PWD/projdata/deviceinfo.h \
    $$PWD/projdata/pictureresourcemanager.h \
    $$PWD/projdata/script.h \
    $$PWD/projdata/helper.h

################################################################################

HEADERS += \
    $$PWD/qrunningmanager.h \
    $$PWD/qbasedialogwidget.h \
    $$PWD/qbasedialog.h

SOURCES += \
    $$PWD/qrunningmanager.cpp \
    $$PWD/qbasedialogwidget.cpp \
    $$PWD/qbasedialog.cpp
