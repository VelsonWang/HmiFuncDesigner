include(../../QStudioSCADA.pri)

QT += core
QT += gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

CONFIG += c++11

TARGET = DrawApplication
TEMPLATE = app

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
    SaveStrategyDialog.cpp \
    GraphPageTreeView.cpp \
    MainWindow.cpp

HEADERS  += \
    itemtypes.h \
    GraphPage.h \
    GraphPageManager.h \
    XmlGraphPageConfigWriter.h \
    UndoCommand.h \
    DrawAppDefs.h \
    ElementLibraryWidget.h \
    ObjectsTreeView.h \
    ElementLibraryListWidget.h \
    ElementLibraryListWidgetItem.h \
    ElementSimpleListWidget.h \
    PreviewWindow.h \
    SaveStrategyDialog.h \
    GraphPageTreeView.h \
    MainWindow.h

FORMS    += \
    ElementLibraryWidget.ui \
    PreviewWindow.ui \
    SaveStrategyDialog.ui \
    MainWindow.ui

RESOURCES += \
    images.qrc

RC_FILE = application.rc

include(graphicselements/graphicselements.pri)
include(propertyeditor/propertyeditor.pri)

