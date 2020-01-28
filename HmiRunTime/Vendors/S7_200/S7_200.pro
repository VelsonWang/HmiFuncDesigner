
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += S7_200.cpp \
    ../../HmiRunTime/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    SiemensS7_200Impl.cpp

HEADERS += S7_200.h \
    ../../HmiRunTime/Tag/IOTag.h \
    ../Public/DataPack.h \
    SiemensS7_200Impl.h
DISTFILES += S7_200.json


