include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += ModbusRTU.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    ModbusRTUImpl.cpp

HEADERS += ModbusRTU.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    ModbusRTUImpl.h
DISTFILES += ModbusRTU.json


