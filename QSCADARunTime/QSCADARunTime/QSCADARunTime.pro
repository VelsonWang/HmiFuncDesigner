
QT += core
QT += gui
QT += widgets
QT += network
QT += script
QT += sql

TARGET_BIN_PATH = $$_PRO_FILE_PWD_/../../RuntimeBin
LINK_LIBRARY_PATH = $$_PRO_FILE_PWD_/../../lib

DESTDIR = $$TARGET_BIN_PATH

TARGET = QSCADARunTime
TEMPLATE = app

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
}

CONFIG -= app_bundle
CONFIG += C++11

include(../log4qt/log4qt.pri)
include(../HttpServer/HttpServer.pri)
include(../QFtpServer/QFtpServer.pri)
include(../Views/Views.pri)

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

LIBRARY_SRC_PATH = $$_PRO_FILE_PWD_/../../QStudioSCADA
INCLUDEPATH += \
    $$LIBRARY_SRC_PATH/libs/edncrypt \
    $$LIBRARY_SRC_PATH/libs/ConfigUtils \
    $$LIBRARY_SRC_PATH/libs/Helper \
    $$LIBRARY_SRC_PATH/libs/PropertyEditor \
    $$LIBRARY_SRC_PATH/libs/DrawListUtils \
    $$LIBRARY_SRC_PATH/libs/Utils

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$LINK_LIBRARY_PATH -ledncryptd
    LIBS += -L$$LINK_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$LINK_LIBRARY_PATH -lHelperd
    LIBS += -L$$LINK_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawListUtilsd
    LIBS += -L$$LINK_LIBRARY_PATH -lPropertyEditord
    LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
}
else { # release
    LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt
    LIBS += -L$$LINK_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lHelper
    LIBS += -L$$LINK_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawListUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
}


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
    SysRuntimeEvent.cpp \
    Vendor/DataPack.cpp \
    Vendor/Modbus/Modbus.cpp \
    Log/Log.cpp \
    MessageQueue/MessageQueue.cpp \
    DB/Database.cpp \
    DB/MySQLDatabase.cpp \
    DB/SQLiteDatabase.cpp \
    DB/RunTimeMySQLDatabase.cpp \
    Public/Global.cpp


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
    FileTansfer.h \
    SysRuntimeEvent.h \
    Singleton.h \
    Vendor/DataPack.h \
    Vendor/Modbus/Modbus.h \
    Log/Log.h \
    VersionInfo.h \
    MessageQueue/MessageQueue.h \
    DB/Database.h \
    DB/MySQLDatabase.h \
    DB/SQLiteDatabase.h \
    DB/RunTimeMySQLDatabase.h \
    Public/Global.h

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

DISTFILES += \
    setting.ini

# copy config files to applicationDirPath
config.path = $$TARGET_BIN_PATH
config.files = ./*.ini
INSTALLS += config

