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
    win32 {
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lPropertyEditord
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
    }
    unix {
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lPropertyEditor
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
}


SOURCES += \
    main.cpp \
    GraphPage.cpp \
    GraphPageManager.cpp \
    UndoCommand.cpp \
    ElementLibraryWidget.cpp \
    ObjectsTreeView.cpp \
    ElementLibraryListWidget.cpp \
    ElementLibraryListWidgetItem.cpp \
    ElementSimpleListWidget.cpp \
    GraphPageTreeView.cpp \
    MainWindow.cpp \
    ../Public/PubTool.cpp \
    ../Public/Element.cpp \
    ../Public/ElementGroup.cpp \
    PluginManager.cpp \
    ../PropertyEditorEx/FunctionEditorDialog.cpp \
    ../PropertyEditorEx/functionproperty.cpp \
    ../PropertyEditorEx/functionpropertyeditor.cpp

HEADERS  += \
    GraphPage.h \
    GraphPageManager.h \
    UndoCommand.h \
    ElementLibraryWidget.h \
    ObjectsTreeView.h \
    ElementLibraryListWidget.h \
    ElementLibraryListWidgetItem.h \
    ElementSimpleListWidget.h \
    GraphPageTreeView.h \
    MainWindow.h \
    ../Public/PublicDefine.h \
    ../Public/PubTool.h \
    ../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../Public/Element.h \
    ../Public/ElementGroup.h \
    PluginManager.h \
    ../PropertyEditorEx/FunctionEditorDialog.h \
    ../PropertyEditorEx/functionproperty.h \
    ../PropertyEditorEx/functionpropertyeditor.h

FORMS    += \
    ElementLibraryWidget.ui \
    MainWindow.ui \
    ../PropertyEditorEx/FunctionEditorDialog.ui

RESOURCES += \
    images.qrc

RC_FILE = application.rc

######################################################

DISTFILES += \
    $$IDE_BIN_PATH/Config/DrawAppJScriptFun.xml \
    ../../../QStudioSCADABin/bin/Config/ElementSupportEvents.xml


