
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += ModbusRTU.cpp \
    ../../HmiRunTime/RunTimeTag.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    ModbusRTUImpl.cpp

HEADERS += ModbusRTU.h \
    ../../HmiRunTime/RunTimeTag.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    ModbusRTUImpl.h
DISTFILES += ModbusRTU.json


