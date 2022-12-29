QT += core gui widgets

TARGET = tagmanager
TEMPLATE = lib

CONFIG  += plugin

BUILD_DIR = $$clean_path($$OUT_PWD)
LIB_DIR = $$clean_path($$BUILD_DIR/../../../../lib)
TOP_SRC_DIR = $$clean_path($$BUILD_DIR/../../../..)

DESTDIR = $$clean_path($$LIB_DIR/pages)
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../../HmiFuncDesignerBin/bin/pages
}
unix {
    DESTDIR = $$BUILD_DIR/../../../../HmiFuncDesignerBin/bin/pages
    LIB_DIR = $$clean_path($$BUILD_DIR/../../../../HmiFuncDesignerBin/bin)
}

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

LIBS += -L$$clean_path($$LIB_DIR) -lshared
LIBS += -L$$clean_path($$LIB_DIR) -lcore
LIBS += -L$$clean_path($$LIB_DIR) -lcsv$${LIB_SUFFIX}

INCLUDEPATH += \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/core \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/shared \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/csv

HEADERS += \
    ../../Public/userevent.h \
    tagmanagerplugin.h \
    tagmanagerwin.h

SOURCES += \
    ../../Public/userevent.cpp \
    tagmanagerplugin.cpp \
    tagmanagerwin.cpp


RESOURCES += \
    images.qrc

FORMS +=


DISTFILES += \
    tagmanagerplugin.json
