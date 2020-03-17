
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += S7_200.cpp \
    ../../HmiRunTimeData/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    SiemensS7_200Impl.cpp

HEADERS += S7_200.h \
    ../../HmiRunTimeData/Tag/IOTag.h \
    ../Public/DataPack.h \
    SiemensS7_200Impl.h
DISTFILES += S7_200.json


