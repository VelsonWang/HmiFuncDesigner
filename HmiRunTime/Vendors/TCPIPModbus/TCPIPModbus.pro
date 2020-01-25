
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += TCPIPModbus.cpp \
    ../../HmiRunTime/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    TCPIPModbusImpl.cpp

HEADERS += TCPIPModbus.h \
    ../../HmiRunTime/Tag/IOTag.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    TCPIPModbusImpl.h
DISTFILES += TCPIPModbus.json


