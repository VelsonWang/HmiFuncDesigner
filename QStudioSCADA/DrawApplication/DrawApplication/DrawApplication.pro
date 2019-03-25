include(../../QStudioSCADA.pri)

QT += core
QT += gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

CONFIG += c++11

TARGET = DrawApplication
TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lHelperd
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lHelper
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
}


SOURCES += \
    main.cpp \
    GraphPage.cpp \
    GraphPageManager.cpp \
    XmlGraphPageConfigWriter.cpp \
    UndoCommand.cpp \
    ElementLibraryWidget.cpp \
    ObjectsTreeView.cpp \
    ElementLibraryListWidget.cpp \
    ElementLibraryListWidgetItem.cpp \
    ElementSimpleListWidget.cpp \
    PreviewWindow.cpp \
    GraphPageTreeView.cpp \
    MainWindow.cpp \
    ../Public/PubTool.cpp \
    ../Public/Element.cpp \
    ../Public/ElementGroup.cpp \
    PluginManager.cpp

HEADERS  += \
    GraphPage.h \
    GraphPageManager.h \
    XmlGraphPageConfigWriter.h \
    UndoCommand.h \
    ElementLibraryWidget.h \
    ObjectsTreeView.h \
    ElementLibraryListWidget.h \
    ElementLibraryListWidgetItem.h \
    ElementSimpleListWidget.h \
    PreviewWindow.h \
    GraphPageTreeView.h \
    MainWindow.h \
    ../Public/PublicDefine.h \
    ../Public/PubTool.h \
    ../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../Public/Element.h \
    ../Public/ElementGroup.h \
    PluginManager.h

FORMS    += \
    ElementLibraryWidget.ui \
    PreviewWindow.ui \
    MainWindow.ui

RESOURCES += \
    images.qrc

RC_FILE = application.rc

