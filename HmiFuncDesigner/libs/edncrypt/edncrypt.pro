
include(../../HmiFuncDesignerLibrary.pri)

CONFIG -= shared dll
CONFIG += staticlib

	
HEADERS += \
    aes.h \
    crc32.h \
    d3des.h \
    edncrypt.h \
    md5.h \
    sha1.h \
    sha256.h 

SOURCES += \
    aes.cpp \
    crc32.cpp \
    d3des.cpp \
    edncrypt.cpp \
    md5.cpp \
    sha1.cpp \
    sha256.cpp 

