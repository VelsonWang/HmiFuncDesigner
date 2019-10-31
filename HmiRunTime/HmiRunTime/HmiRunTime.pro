
QT += core
QT += gui
QT += widgets
QT += network
QT += script
QT += sql

TARGET_BIN_PATH = $$_PRO_FILE_PWD_/../../HmiRunTimeBin
LINK_LIBRARY_PATH = $$_PRO_FILE_PWD_/../../lib

DESTDIR = $$TARGET_BIN_PATH

TARGET = HmiRunTime
TEMPLATE = app

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
}

CONFIG -= app_bundle
CONFIG += C++11

# 开启SOAP服务
#DEFINES += USE_SOAP_SERVICE

# 开启ftp服务
#DEFINES += USE_FTP_SERVICE

include(../log4qt/log4qt.pri)
include(../HttpServer/HttpServer.pri)
#include(../QFtpServer/QFtpServer.pri)
include(../Views/Views.pri)
#include(../SOAP/server/SOAPServer.pri)

INCLUDEPATH += $$_PRO_FILE_PWD_ \
               Vendor \
               Port \
               qextserial \
               Public \
               DB \
               Tag \
               Vendor \
               Vendor/Modbus \
               Vendor/Mitsubishi \
               Vendor/SIEMENS_S7_200 \
               Event \
               Script \
               Log \
               Socket \
               Service

LIBRARY_SRC_PATH = $$_PRO_FILE_PWD_/../../HmiFuncDesigner
INCLUDEPATH += \
    $$LIBRARY_SRC_PATH/libs/edncrypt \
    $$LIBRARY_SRC_PATH/libs/DrawUtils \
    $$LIBRARY_SRC_PATH/libs/Utils \
    $$LIBRARY_SRC_PATH/libs/ProjectDataUtils

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$LINK_LIBRARY_PATH -ledncryptd
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
    }
    unix {
        LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
    }
}
else { # release
    LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
}

win32 {
    LIBS += -lpthread -lwsock32 -lws2_32
} else {

}

SOURCES += \
    MessageTransfer.cpp \
    SerialPortReMapping.cpp \
    Vendor/SIEMENS_S7_200/S7_200.cpp \
    Vendor/SIEMENS_S7_200/S7_200Device.cpp \
    qextserial/qextserialport.cpp \
    DB/DBTagObject.cpp \
    DB/RealTimeDB.cpp \
    Tag/Tag.cpp \
    HmiRunTime.cpp \
    Vendor/Modbus/ModbusRTU.cpp \
    Vendor/IOTag.cpp \
    Vendor/Modbus/ModbusRTUDevice.cpp \
    Vendor/VendorPrivate.cpp \
    TimerTask.cpp \
    PortThread.cpp \
    Vendor/Modbus/ModbusASCII.cpp \
    Vendor/Modbus/ModbusASCIIDevice.cpp \
    Vendor/Modbus/TCPIPModbus.cpp \
    Vendor/Modbus/TCPIPModbusDevice.cpp \
    Port/NetPort.cpp \
    Tag/Function.cpp \
    Event/Event.cpp \
    Script/JavaScript.cpp \
    Script/RunScript.cpp \
    SysRuntimeEvent.cpp \
    Vendor/DataPack.cpp \
    Vendor/Modbus/Modbus.cpp \
    Log/Log.cpp \
    MessageQueue/MessageQueue.cpp \
    Public/Global.cpp \
    Vendor/Mitsubishi/MitsubishiDevice.cpp \
    Vendor/Mitsubishi/Mitsubishi.cpp


HEADERS  += \
    MessageTransfer.h \
    SerialPortReMapping.h \
    Vendor/SIEMENS_S7_200/S7_200.h \
    Vendor/SIEMENS_S7_200/S7_200Device.h \
    qextserial/qextserialport_global.h \
    qextserial/qextserialport.h \
    DB/DBTagObject.h \
    DB/RealTimeDB.h \
    Tag/Tag.h \
    Public/public.h \
    HmiRunTime.h \
    Vendor/Modbus/ModbusRTU.h \
    Vendor/IOTag.h \
    Vendor/Modbus/ModbusRTUDevice.h \
    Vendor/VendorPrivate.h \
    TimerTask.h \
    PortThread.h \
    Vendor/Modbus/ModbusASCII.h \
    Vendor/Modbus/ModbusASCIIDevice.h \
    Public/PublicFunction.h \
    Vendor/Modbus/TCPIPModbus.h \
    Vendor/Modbus/TCPIPModbusDevice.h \
    Port/NetPort.h \
    Event/Event.h \
    Tag/Function.h \
    Script/JavaScript.h \
    Script/RunScript.h \
    FileTansfer.h \
    SysRuntimeEvent.h \
    Singleton.h \
    Vendor/DataPack.h \
    Vendor/Modbus/Modbus.h \
    Log/Log.h \
    VersionInfo.h \
    MessageQueue/MessageQueue.h \
    Public/Global.h \
    Vendor/Mitsubishi/MitsubishiDevice.h \
    Vendor/Mitsubishi/Mitsubishi.h \
    Socket/xsocket.hpp

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

SOURCES += $$PWD/main.cpp\
    $$PWD/tcpserver.cpp \
    $$PWD/tcpsocket.cpp \
    $$PWD/threadhandle.cpp \
    $$PWD/Port/ComPort.cpp \


HEADERS  += \
    $$PWD/tcpserver.h \
    $$PWD/tcpsocket.h \
    $$PWD/threadhandle.h \
    $$PWD/Vendor/IVendor.h \
    $$PWD/Port/ComPort.h \
    $$PWD/Port/IPort.h

FORMS +=

DISTFILES += \
    setting.ini

# copy config files to applicationDirPath
config.path = $$TARGET_BIN_PATH
config.files = ./*.ini
INSTALLS += config

