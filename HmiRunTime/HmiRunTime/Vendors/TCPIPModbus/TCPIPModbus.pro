include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += TCPIPModbus.cpp \
    ../Public/DataPack.cpp \
    ../Public/Modbus.cpp \
    TCPIPModbusImpl.cpp

HEADERS += TCPIPModbus.h \
    ../Public/DataPack.h \
    ../Public/Modbus.h \
    TCPIPModbusImpl.h
DISTFILES += TCPIPModbus.json


