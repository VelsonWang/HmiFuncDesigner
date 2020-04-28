QT += core
QT += gui
QT += widgets
QT += network
QT += sql

TARGET_BIN_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../HmiRunTimeBin)
LINK_LIBRARY_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../lib)
#message("TARGET_BIN_PATH: "$$TARGET_BIN_PATH)
#message("LINK_LIBRARY_PATH: "$$LINK_LIBRARY_PATH)

DESTDIR = $$TARGET_BIN_PATH

TARGET = HmiRunTimeData
TEMPLATE = app

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
    DEFINES += BUILD_BY_DEBUG
}


CONFIG -= app_bundle
CONFIG += C++11

# 开启SOAP服务
#DEFINES += USE_SOAP_SERVICE

#去除UNICODE字符编码
DEFINES -= UNICODE

include(../../log4qt/log4qt.pri)
include(../../HttpServer/HttpServer.pri)
#include(../../SOAP/server/SOAPServer.pri)

INCLUDEPATH += \
    $$_PRO_FILE_PWD_ \
    Vendor \
    Port \
    qextserial \
    Public \
    DB \
    Tag \
    Event \
    Script \
    Log \
    Socket \
    Service \
    ../Vendors/IVendorPlugin

LIBRARY_SRC_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../HmiFuncDesigner)
#message("LIBRARY_SRC_PATH: "$$LIBRARY_SRC_PATH)

INCLUDEPATH += \
    $$LIBRARY_SRC_PATH/libs/edncrypt \
    $$LIBRARY_SRC_PATH/libs/DrawUtils \
    $$LIBRARY_SRC_PATH/libs/Utils \
    $$LIBRARY_SRC_PATH/libs/ProjectDataUtils

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}

win32 {
    LIBS += -lpthread -lwsock32 -lws2_32
} else {

}

SOURCES += \
    main.cpp \
    Port/ComPort.cpp \
    SerialPortReMapping.cpp \
    Vendor.cpp \
    VendorPluginManager.cpp \
    qextserial/qextserialport.cpp \
    DB/DBTagObject.cpp \
    DB/RealTimeDB.cpp \
    Tag/Tag.cpp \
    HmiRunTime.cpp \
    Tag/IOTag.cpp \
    TimerTask.cpp \
    PortThread.cpp \
    Port/NetPort.cpp \
    Tag/Function.cpp \
    Event/Event.cpp \
    SysRuntimeEvent.cpp \
    Log/Log.cpp \
    MessageQueue/MessageQueue.cpp \
    Public/Global.cpp


HEADERS  += \
    Vendor.h \
    IVendor.h \
    Port/ComPort.h \
    Port/IPort.h \
    SerialPortReMapping.h \
    VendorPluginManager.h \
    qextserial/qextserialport_global.h \
    qextserial/qextserialport.h \
    DB/DBTagObject.h \
    DB/RealTimeDB.h \
    Tag/Tag.h \
    Public/public.h \
    HmiRunTime.h \
    Tag/IOTag.h \
    TimerTask.h \
    PortThread.h \
    Public/PublicFunction.h \
    Port/NetPort.h \
    Event/Event.h \
    Tag/Function.h \
    FileTansfer.h \
    SysRuntimeEvent.h \
    Singleton.h \
    Log/Log.h \
    VersionInfo.h \
    MessageQueue/MessageQueue.h \
    Public/Global.h \
    Socket/xsocket.hpp

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}


DISTFILES += \
    setting.ini

# copy config files to applicationDirPath
config.path = $$TARGET_BIN_PATH
config.files = ./*.ini
INSTALLS += config

