
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network
QT += script
QT += sql

DESTDIR = $$_PRO_FILE_PWD_/../../RuntimeBin

TARGET = QSCADARunTime
TEMPLATE = app

QT -= gui

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
}

CONFIG -= app_bundle
CONFIG += C++11

include(../log4qt/log4qt.pri)
include(../HttpServer/HttpServer.pri)
include(../QFtpServer/QFtpServer.pri)

INCLUDEPATH += $$_PRO_FILE_PWD_ \
               Vendor \
               Port \
               qextserial \
               Public \
               DB \
               Tag \
               Vendor \
               Vendor/Modbus \
               Event \
               Script \
               Log

SOURCES += \
        qextserial/qextserialport.cpp \
    DB/DBTagObject.cpp \
    DB/RealTimeDB.cpp \
    Tag/Tag.cpp \
    SCADARunTime.cpp \
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
    DB/DatabaseHelper.cpp \
    SysRuntimeEvent.cpp \
    Vendor/DataPack.cpp \
    Vendor/Modbus/Modbus.cpp \
    Log/Log.cpp


HEADERS  += \
        qextserial/qextserialport_global.h \
        qextserial/qextserialport.h \
    DB/DBTagObject.h \
    DB/RealTimeDB.h \
    Tag/Tag.h \
    Public/public.h \
    SCADARunTime.h \
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
    DB/DatabaseHelper.h \
    FileTansfer.h \
    SysRuntimeEvent.h \
    Singleton.h \
    Vendor/DataPack.h \
    Vendor/Modbus/Modbus.h \
    Log/Log.h \
    VersionInfo.h

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


unix:include($$PWD/eventdispatcher_libev/eventdispatcher_libev.pri)

FORMS +=
