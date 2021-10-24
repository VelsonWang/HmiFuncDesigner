QT += core
QT += gui
QT += widgets
QT += network
QT += script
QT += sql
QT += xml

TARGET_BIN_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../HmiRunTimeBin)
LINK_LIBRARY_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../lib_rt)
message("TARGET_BIN_PATH: "$$TARGET_BIN_PATH)
message("LINK_LIBRARY_PATH: "$$LINK_LIBRARY_PATH)

DESTDIR = $$TARGET_BIN_PATH

TARGET = HmiRunTime
TEMPLATE = app

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
    DEFINES += BUILD_BY_DEBUG
}


CONFIG -= app_bundle
CONFIG += C++11

QMAKE_CXXFLAGS += -std=c++11

# 开启SOAP服务
#DEFINES += USE_SOAP_SERVICE

#去除UNICODE字符编码
DEFINES -= UNICODE

INCLUDEPATH += \
    $$_PRO_FILE_PWD_ \
    Vendor \
    Port \
    qextserial \
    Public \
    Event \
    Script \
    Log \
    Socket \
    Service \
    ../Public

LIBRARY_SRC_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../HmiRunTime)
message("LIBRARY_SRC_PATH: "$$LIBRARY_SRC_PATH)

INCLUDEPATH += \
    $$LIBRARY_SRC_PATH/HmiRunTime/Public \
    $$LIBRARY_SRC_PATH/HmiRunTime/Vendors/IVendorPlugin \
    $$LIBRARY_SRC_PATH/libs/edncrypt \
    $$LIBRARY_SRC_PATH/libs/core \
    $$LIBRARY_SRC_PATH/libs/Utils \
    $$LIBRARY_SRC_PATH/libs/gradienteditor \
    $$LIBRARY_SRC_PATH/libs/qtsingleapplication \
    $$LIBRARY_SRC_PATH/libs/running \
    $$LIBRARY_SRC_PATH/libs/shared

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

#LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt$${LIB_SUFFIX}
#LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils$${LIB_SUFFIX}
#LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}

LIBS += -L$$LINK_LIBRARY_PATH -lcore
LIBS += -L$$LINK_LIBRARY_PATH -lshared
LIBS += -L$$LINK_LIBRARY_PATH -lrunning
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}

win32 {
    LIBS += -lpthread -lwsock32 -lws2_32
} else {

}

include(../../HttpServer/HttpServer.pri)
#include(../../SOAP/server/SOAPServer.pri)

SOURCES += \
#    ../Public/Element.cpp \
#    ../Public/ElementGroup.cpp \
#    ../Public/PubTool.cpp \
#    GraphPage.cpp \
#    GraphPageManager.cpp \
#    MainWindow.cpp \
#    ViewElementPluginManager.cpp \
    Function.cpp \
    Public/Global.cpp \
    RunTimeTag.cpp \
    qextserial/qextserialport.cpp \
    RealTimeDB.cpp \
    Event/Event.cpp \
    HmiRunTime.cpp \
    Port/ComPort.cpp \
    Port/NetPort.cpp \
    PortThread.cpp \
#    Script/JavaScript.cpp \
#    Script/RunScript.cpp \
    SerialPortReMapping.cpp \
    TimerTask.cpp \
    Vendor.cpp \
    VendorPluginManager.cpp \
    log/ulog.cpp \
    main.cpp \
#    MessageTransfer.cpp \
#    HmiRunTime.cpp \
#    Event/Event.cpp \
#    SysRuntimeEvent.cpp \
#    Public/Global.cpp


HEADERS  += \
#    ../Public/Element.h \
#    ../Public/ElementGroup.h \
#    ../Public/PubTool.h \
#    ../Public/PublicDefine.h \
#    GraphPage.h \
#    GraphPageManager.h \
#    IVendor.h \
#    MainWindow.h \
#    MessageTransfer.h \
#    Public/public.h \
#    HmiRunTime.h \
#    Public/PublicFunction.h \
#    Event/Event.h \
#    FileTansfer.h \
#    SysRuntimeEvent.h \
    ../Vendors/IVendorPlugin/IVendorPlugin.h \
    Public/Global.h \
    Public/PublicFunction.h \
    Public/public.h \
    Socket/xsocket.hpp \
    Function.h \
    RunTimeTag.h \
    qextserial/qextserialport_global.h \
    qextserial/qextserialport.h \
    RealTimeDB.h \
    Event/Event.h \
    HmiRunTime.h \
    Port/ComPort.h \
    Port/IPort.h \
    Port/NetPort.h \
    PortThread.h \
#    Script/JavaScript.h \
#    Script/RunScript.h \
    SerialPortReMapping.h \
    Singleton.h \
    TimerTask.h \
    Vendor.h \
    VendorPluginManager.h \
    VersionInfo.h \
    log/ulog.h

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

