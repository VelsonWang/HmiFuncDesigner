QT += core gui widgets xml script

TARGET = shared
TEMPLATE = lib

BUILD_DIR = $$clean_path($$OUT_PWD)

DESTDIR = $$BUILD_DIR/../../../lib
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiFuncDesignerBin/bin
}

DEFINES += SHARED_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/property
INCLUDEPATH += $$PWD/host
INCLUDEPATH += $$PWD/projdata

HEADERS += \
    confighelper.h \
    host/qimagehost.h \
    host/qindicationlamphost.h \
    host/qjarshapehost.h \
    property/qcolorproperty.h \
    property/qfileproperty.h \
    property/qfunctionproperty.h \
    property/qimageproperty.h \
    property/qtagcolorlistproperty.h \
    property/qtagproperty.h \
    property/qtagtextlistproperty.h \
    sharedlibglobal.h \
    widgets/qimagebox.h \
    widgets/qindicationlamp.h \
    widgets/qjarshape.h \
    xmlobject.h \
    qpropertyfactory.h \
    property/qabstractproperty.h \
    property/qboolproperty.h \
    property/qbytearrayproperty.h \
    property/qintproperty.h \
    property/qstringproperty.h \
    property/qsizeproperty.h \
    property/qpointproperty.h \
    property/qrectproperty.h \
    property/qscriptproperty.h \
    property/qtimeproperty.h \
    property/qdatetimeproperty.h \
    property/qdateproperty.h \
    property/qenumproperty.h \
    qcommonstruct.h \
    property/qcursorproperty.h \
    property/qfloatproperty.h \
    property/qfontproperty.h \
    property/qmemerydataproperty.h \
    property/qalignmentproperty.h \
    host/qabstracthost.h \
    qhostfactory.h \
    qabstractplugin.h \
    pluginloader.h \
    host/qabstractwidgethost.h \
    qprojectcore.h \
    host/qprojecthost.h \
    host/qwidgethost.h \
    qbaseinit.h \
    host/qframehost.h \
    host/qformhost.h \
    qpagemanager.h \
    host/qlabelhost.h \
    host/qlcdnumberhost.h \
    host/qabstractbuttonhost.h \
    host/qpushbuttonhost.h \
    host/qcheckboxhost.h \
    host/qradiobuttonhost.h \
    host/qlineedithost.h \
    host/qabstractsliderhost.h \
    host/qdialhost.h \
    host/qsliderhost.h \
    host/qabstractspinboxhost.h \
    host/qspinboxhost.h \
    host/qdoublespinboxhost.h \
    host/qdatetimeedithost.h \
    host/qtimeedithost.h \
    host/qdateedithost.h \
    host/qscrollbarhost.h \
    host/qprogressbarhost.h \
    host/qcomboboxhost.h \
    qabstractupdateplugin.h \
    property/stylesheetitem/stylesheetstruct.h \
    property/stylesheetitem/qtextitem.h \
    property/stylesheetitem/qstylesheetitemfactory.h \
    property/stylesheetitem/qgradientmaker.h \
    property/stylesheetitem/qgradientitem.h \
    property/stylesheetitem/qborderitem.h \
    property/stylesheetitem/qbackgrounditem.h \
    property/stylesheetitem/qabstractstylesheetitem.h \
    property/qstylesheetproperty.h \
    qstylesheetinfo.h

SOURCES += \
    confighelper.cpp \
    host/qimagehost.cpp \
    host/qindicationlamphost.cpp \
    host/qjarshapehost.cpp \
    property/qcolorproperty.cpp \
    property/qfileproperty.cpp \
    property/qfunctionproperty.cpp \
    property/qimageproperty.cpp \
    property/qtagcolorlistproperty.cpp \
    property/qtagproperty.cpp \
    property/qtagtextlistproperty.cpp \
    widgets/qimagebox.cpp \
    widgets/qindicationlamp.cpp \
    widgets/qjarshape.cpp \
    xmlobject.cpp \
    qpropertyfactory.cpp \
    property/qabstractproperty.cpp \
    property/qboolproperty.cpp \
    property/qbytearrayproperty.cpp \
    property/qintproperty.cpp \
    property/qstringproperty.cpp \
    property/qsizeproperty.cpp \
    property/qpointproperty.cpp \
    property/qrectproperty.cpp \
    property/qscriptproperty.cpp \
    property/qtimeproperty.cpp \
    property/qdatetimeproperty.cpp \
    property/qdateproperty.cpp \
    property/qenumproperty.cpp \
    property/qcursorproperty.cpp \
    property/qfloatproperty.cpp \
    property/qfontproperty.cpp \
    property/qmemerydataproperty.cpp \
    property/qalignmentproperty.cpp \
    host/qabstracthost.cpp \
    qhostfactory.cpp \
    qabstractplugin.cpp \
    pluginloader.cpp \
    host/qabstractwidgethost.cpp \
    qprojectcore.cpp \
    host/qprojecthost.cpp \
    host/qwidgethost.cpp \
    qbaseinit.cpp \
    host/qframehost.cpp \
    host/qformhost.cpp \
    qpagemanager.cpp \
    host/qlabelhost.cpp \
    host/qlcdnumberhost.cpp \
    host/qabstractbuttonhost.cpp \
    host/qpushbuttonhost.cpp \
    host/qcheckboxhost.cpp \
    host/qradiobuttonhost.cpp \
    host/qlineedithost.cpp \
    host/qabstractsliderhost.cpp \
    host/qdialhost.cpp \
    host/qsliderhost.cpp \
    host/qabstractspinboxhost.cpp \
    host/qspinboxhost.cpp \
    host/qdoublespinboxhost.cpp \
    host/qdatetimeedithost.cpp \
    host/qtimeedithost.cpp \
    host/qdateedithost.cpp \
    host/qscrollbarhost.cpp \
    host/qprogressbarhost.cpp \
    host/qcomboboxhost.cpp \
    qabstractupdateplugin.cpp \
    property/stylesheetitem/qtextitem.cpp \
    property/stylesheetitem/qstylesheetitemfactory.cpp \
    property/stylesheetitem/qgradientmaker.cpp \
    property/stylesheetitem/qgradientitem.cpp \
    property/stylesheetitem/qborderitem.cpp \
    property/stylesheetitem/qbackgrounditem.cpp \
    property/stylesheetitem/qabstractstylesheetitem.cpp \
    property/qstylesheetproperty.cpp \
    qstylesheetinfo.cpp

FORMS +=

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
