QT += core gui widgets script

TARGET = script
TEMPLATE = lib

CONFIG  += plugin

BUILD_DIR = $$clean_path($$OUT_PWD)
LIB_DIR = $$clean_path($$BUILD_DIR/../../../../lib)
TOP_SRC_DIR = $$clean_path($$BUILD_DIR/../../../..)

DESTDIR = $$clean_path($$LIB_DIR/pages)
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../../HmiFuncDesignerBin/bin/pages
}

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

LIBS += -L$$clean_path($$LIB_DIR) -lshared
LIBS += -L$$clean_path($$LIB_DIR) -lcore
LIBS += -L$$clean_path($$LIB_DIR) -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}

INCLUDEPATH += \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/core \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/shared \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/qscintilla

HEADERS += \
    ../../Public/userevent.h \
    insertfunctiondialog.h \
    inserttagdialog.h \
    scriptconditionconfigform.h \
    scripteditordlg.h \
    scriptplugin.h \
    scriptrunconditioneditordlg.h \
    scriptwin.h \
    splitterform.h

SOURCES += \
    ../../Public/userevent.cpp \
    insertfunctiondialog.cpp \
    inserttagdialog.cpp \
    scriptconditionconfigform.cpp \
    scripteditordlg.cpp \
    scriptplugin.cpp \
    scriptrunconditioneditordlg.cpp \
    scriptwin.cpp \
    splitterform.cpp


RESOURCES += \
    images.qrc

FORMS += \
    insertfunctiondialog.ui \
    inserttagdialog.ui \
    scriptconditionconfigform.ui \
    scripteditordlg.ui \
    scriptrunconditioneditordlg.ui


DISTFILES += \
    scriptplugin.json
