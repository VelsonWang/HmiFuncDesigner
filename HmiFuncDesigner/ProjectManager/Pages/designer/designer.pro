QT += core gui widgets

TARGET = designer
TEMPLATE = lib

CONFIG  += plugin

BUILD_DIR = $$clean_path($$OUT_PWD)
LIB_DIR = $$clean_path($$BUILD_DIR/../../../../lib)
TOP_SRC_DIR = $$clean_path($$BUILD_DIR/../../../..)
#message(BUILD_DIR:$$clean_path($$BUILD_DIR))
#message(LIB_DIR:$$clean_path($$LIB_DIR))
#message(TOP_SRC_DIR:$$clean_path($$TOP_SRC_DIR))

DESTDIR = $$clean_path($$LIB_DIR/pages)
#message(DESTDIR:$$clean_path($$DESTDIR))
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../../HmiFuncDesignerBin/bin/pages
    #message(DLLDESTDIR:$$clean_path($$DLLDESTDIR))
}

LIBS += -L$$clean_path($$LIB_DIR) -lshared
LIBS += -L$$clean_path($$LIB_DIR) -lcore


INCLUDEPATH += \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/core \
    $$TOP_SRC_DIR/HmiFuncDesigner/libs/shared

HEADERS += \
    ../../Public/userevent.h \
    form/formnamelistwidget.h \
    qdesignerwidget.h \
    qdesignerplugin.h \
    widgetbox/widgetboxtreewidget.h \
    widgetbox/qwidgetboxview.h \
    qdesignermimedata.h \
    property/qpropertyview.h \
    form/widgethost.h \
    form/formresizer.h \
    form/sizehandlerect.h \
    form/qformwidgetview.h \
    form/qformlistwidget.h \
    form/qdesignerformhost.h \
    form/qselectwidget.h \
    form/qnewpagedialog.h \
    form/qtemplatetreewidget.h \
    form/qtemplatenamewidget.h \
    objectlist/qobjectlistwidget.h \
    objectlist/qobjectlistview.h \
    objectlist/qlistviewwidget.h \
    form/qnewpagetable.h \
    form/qtempwidget.h \
    form/qpagewidget.h

SOURCES += \
    ../../Public/userevent.cpp \
    form/formnamelistwidget.cpp \
    qdesignerwidget.cpp \
    qdesignerplugin.cpp \
    widgetbox/widgetboxtreewidget.cpp \
    widgetbox/qwidgetboxview.cpp \
    qdesignermimedata.cpp \
    property/qpropertyview.cpp \
    form/widgethost.cpp \
    form/formresizer.cpp \
    form/sizehandlerect.cpp \
    form/qformwidgetview.cpp \
    form/qformlistwidget.cpp \
    form/qdesignerformhost.cpp \
    form/qselectwidget.cpp \
    form/qnewpagedialog.cpp \
    form/qtemplatetreewidget.cpp \
    form/qtemplatenamewidget.cpp \
    objectlist/qobjectlistwidget.cpp \
    objectlist/qobjectlistview.cpp \
    objectlist/qlistviewwidget.cpp \
    form/qnewpagetable.cpp \
    form/qtempwidget.cpp \
    form/qpagewidget.cpp

RESOURCES += \
    designer.qrc

FORMS += \
    form/qnewpagedialog.ui \
    form/qtemplatenamewidget.ui \
    objectlist/qlistviewwidget.ui \
    form/qtempwidget.ui

DISTFILES += qdesignerplugin.json
