QT += core gui xml widgets script

TARGET = core
TEMPLATE = lib

CONFIG(debug, debug|release) { #debug
    DEFINES += BUILD_DEBUG
}

BUILD_DIR = $$clean_path($$OUT_PWD)

DESTDIR = $$BUILD_DIR/../../../lib_rt
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiRunTimeBin
}

DEFINES += CORE_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/qpropertylist
INCLUDEPATH += $$PWD/undocommand

LIBS += -L$$clean_path($$DESTDIR) -lshared
LIBS += -L$$clean_path($$DESTDIR) -lgradientEditor

HEADERS += \
    corelibglobal.h \
    qsoftcore.h \
    undocommand/qbaseundocommand.h \
    undocommand/qhostparentchangedundocommand.h \
    undocommand/qaddhostundocommand.h

SOURCES += \
    qsoftcore.cpp \
    undocommand/qbaseundocommand.cpp \
    undocommand/qhostparentchangedundocommand.cpp \
    undocommand/qaddhostundocommand.cpp


