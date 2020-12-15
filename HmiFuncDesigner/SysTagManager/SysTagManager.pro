
include(../HmiFuncDesigner.pri)

QT += core gui
QT += widgets

TARGET = SysTagManager
TEMPLATE = app


DESTDIR = $$IDE_BIN_PATH

LIBS += -L$$LINK_LIBRARY_PATH -lProjData
LIBS += -L$$LINK_LIBRARY_PATH -lcore

SOURCES += main.cpp \
    SysTag.cpp \
    qtablewidgetex.cpp

HEADERS  += \
    SysTag.h \
    qtablewidgetex.h

FORMS    +=

RESOURCES += \
    images.qrc

RC_FILE = SysTagManager.rc
