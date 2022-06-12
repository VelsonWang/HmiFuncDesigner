QT += core gui widgets

TARGET = systemparameters
TEMPLATE = lib

CONFIG  += plugin

BUILD_DIR = $$clean_path($$OUT_PWD)
LIB_DIR = $$clean_path($$BUILD_DIR/../../../../lib)
TOP_SRC_DIR = $$clean_path($$BUILD_DIR/../../../..)

DESTDIR = $$clean_path($$LIB_DIR/pages)
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../../HmiFuncDesignerBin/bin/pages
}

LIBS += -L$$clean_path($$LIB_DIR) -lshared
LIBS += -L$$clean_path($$LIB_DIR) -lcore

INCLUDEPATH += \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/core \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/shared

HEADERS += \
    systemparameters.h \
    systemparametersplugin.h

SOURCES += \
    systemparameters.cpp \
    systemparametersplugin.cpp


RESOURCES += \
    images.qrc

FORMS +=


DISTFILES += \
    systemparametersplugin.json
