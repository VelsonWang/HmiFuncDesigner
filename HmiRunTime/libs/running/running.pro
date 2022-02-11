QT += core gui xml widgets script

TARGET = running
TEMPLATE = lib

BUILD_DIR = $$clean_path($$OUT_PWD)

DESTDIR = $$BUILD_DIR/../../../lib_rt
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiRunTimeBin
}

DEFINES += RUNNING_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD

LIBS += -L$$clean_path($$DESTDIR) -lshared


HEADERS += \
    runninglibglobal.h \
    qrunningmanager.h \
    qbasedialogwidget.h \
    qbasedialog.h

SOURCES += \
    qrunningmanager.cpp \
    qbasedialogwidget.cpp \
    qbasedialog.cpp
