
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += ModbusASCII.cpp \
    ../../HmiRunTimeData/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    ModbusASCIIImpl.cpp

HEADERS += ModbusASCII.h \
    ../../HmiRunTimeData/Tag/IOTag.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    ModbusASCIIImpl.h
DISTFILES += ModbusASCII.json


