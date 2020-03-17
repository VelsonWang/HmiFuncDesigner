
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += TCPIPModbus.cpp \
    ../../HmiRunTimeData/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    TCPIPModbusImpl.cpp

HEADERS += TCPIPModbus.h \
    ../../HmiRunTimeData/Tag/IOTag.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    TCPIPModbusImpl.h
DISTFILES += TCPIPModbus.json


