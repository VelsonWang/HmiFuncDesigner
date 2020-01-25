
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += ModbusASCII.cpp \
    ../../HmiRunTime/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    ModbusASCIIImpl.cpp

HEADERS += ModbusASCII.h \
    ../../HmiRunTime/Tag/IOTag.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    ModbusASCIIImpl.h
DISTFILES += ModbusASCII.json


