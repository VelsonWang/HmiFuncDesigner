
include(../../HmiRunTimeVendorPlugin.pri)

SOURCES += FX2N.cpp \
    ../../HmiRunTime/Tag/IOTag.cpp \
    ../Public/DataPack.cpp \
    MitsubishiImpl.cpp

HEADERS += FX2N.h \
    ../../HmiRunTime/Tag/IOTag.h \
    ../Public/DataPack.h \
    MitsubishiImpl.h
DISTFILES += FX2N.json


