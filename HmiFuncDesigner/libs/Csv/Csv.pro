
include(../../HmiFuncDesignerLibrary.pri)

#CONFIG -= shared dll
#CONFIG += staticlib

!msvc {
    # flags for gcc-like compiler
    #CONFIG += warn_on
    #QMAKE_CXXFLAGS_WARN_ON += -Werror -Wformat=2 -Wuninitialized -Winit-self \
    #        -Wmissing-include-dirs -Wswitch-enum -Wundef -Wpointer-arith \
    #        -Wdisabled-optimization -Wcast-align -Wcast-qual
}

DEFINES += QTCSV_LIBRARY

SOURCES += \
    writer.cpp \
    variantdata.cpp \
    stringdata.cpp \
    reader.cpp \
    contentiterator.cpp

HEADERS += \
    qtcsv_global.h \
    writer.h \
    variantdata.h \
    stringdata.h \
    reader.h \
    abstractdata.h \
    filechecker.h \
    contentiterator.h \
    symbols.h

unix {
    # settings for command "make install"
#    libheaders.path = /usr/local/include/qtcsv/
#    libheaders.files = $$PWD/include/*.h

#    target.path = /usr/local/lib
#    INSTALLS += target libheaders
}
