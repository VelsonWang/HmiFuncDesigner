
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += FX2N.cpp \
    ../../HmiRunTimeData/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    MitsubishiImpl.cpp

HEADERS += FX2N.h \
    ../../HmiRunTimeData/Tag/IOTag.h \
    ../Public/DataPack.h \
    MitsubishiImpl.h
DISTFILES += FX2N.json


