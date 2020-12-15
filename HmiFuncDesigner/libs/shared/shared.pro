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
INCLUDEPATH += $$PWD/resource
INCLUDEPATH += $$PWD/driver


HEADERS += \
    sharedlibglobal.h \
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
    resource/qresourceregister.h \
    resource/qresourcefile.h \
    resource/qrccmaker.h \
    host/qwidgethost.h \
    qbaseinit.h \
    host/qframehost.h \
    host/qformhost.h \
    qpagemanager.h \
    qlanguage.h \
    qlanguagemanager.h \
    qusermanager.h \
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
    qdatamanager.h \
    qdata.h \
    property/stylesheetitem/stylesheetstruct.h \
    property/stylesheetitem/qtextitem.h \
    property/stylesheetitem/qstylesheetitemfactory.h \
    property/stylesheetitem/qgradientmaker.h \
    property/stylesheetitem/qgradientitem.h \
    property/stylesheetitem/qborderitem.h \
    property/stylesheetitem/qbackgrounditem.h \
    property/stylesheetitem/qabstractstylesheetitem.h \
    property/qstylesheetproperty.h \
    resource/qresourcemanager.h \
    qstylesheetinfo.h \
    driver/qabstractdriver.h \
    driver/qcommdriver.h \
    driver/qtcpdriver.h \
    qdriverfactory.h \
    qdrivermanager.h

SOURCES += \
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
    resource/qresourceregister.cpp \
    resource/qresourcefile.cpp \
    resource/qrccmaker.cpp \
    host/qwidgethost.cpp \
    qbaseinit.cpp \
    host/qframehost.cpp \
    host/qformhost.cpp \
    qpagemanager.cpp \
    qlanguage.cpp \
    qlanguagemanager.cpp \
    qusermanager.cpp \
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
    qdatamanager.cpp \
    qdata.cpp \
    property/stylesheetitem/qtextitem.cpp \
    property/stylesheetitem/qstylesheetitemfactory.cpp \
    property/stylesheetitem/qgradientmaker.cpp \
    property/stylesheetitem/qgradientitem.cpp \
    property/stylesheetitem/qborderitem.cpp \
    property/stylesheetitem/qbackgrounditem.cpp \
    property/stylesheetitem/qabstractstylesheetitem.cpp \
    property/qstylesheetproperty.cpp \
    resource/qresourcemanager.cpp \
    qstylesheetinfo.cpp \
    driver/qabstractdriver.cpp \
    driver/qcommdriver.cpp \
    driver/qtcpdriver.cpp \
    qdriverfactory.cpp \
    qdrivermanager.cpp

FORMS +=

RESOURCES += \
    shared.qrc

#DISTFILES += qabstractplugin.json


################################################################################

INCLUDEPATH += $$PWD/projdata

SOURCES += \
    projdata/DataAES.cpp \
    projdata/Tag.cpp \
    projdata/ulog.cpp \
    projdata/ProjectData.cpp \
    projdata/ProjectInfoManager.cpp \
    projdata/NetSetting.cpp \
    projdata/DatabaseSetting.cpp \
    projdata/UserAuthority.cpp \
    projdata/DeviceInfo.cpp \
    projdata/PictureResourceManager.cpp \
    projdata/Script.cpp \
    projdata/Helper.cpp


HEADERS += \
    projdata/DataAES.h \
    projdata/IGraphPageSaveLoad.h \
    projdata/Tag.h \
    projdata/ulog.h \
    projdata/ProjectData.h \
    projdata/ProjectInfoManager.h \
    projdata/NetSetting.h \
    projdata/DatabaseSetting.h \
    projdata/UserAuthority.h \
    projdata/DeviceInfo.h \
    projdata/PictureResourceManager.h \
    projdata/Script.h \
    projdata/Helper.h
