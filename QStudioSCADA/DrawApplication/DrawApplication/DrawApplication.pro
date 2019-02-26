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
    mainwindow.cpp \
    objectstreeview.cpp \
    elementsimplelistwidget.cpp \
    newcomponentdialog.cpp \
    savestrategydialog.cpp \
    previewwindow.cpp \
    elementlibrarylistwidget.cpp \
    elementlibrarylistwidgetitem.cpp \
    GraphPage.cpp \
    GraphPageManager.cpp \
    XmlGraphPageConfigWriter.cpp \
    ProjectTreeView.cpp \
    UndoCommand.cpp \
    ElementLibraryWidget.cpp

HEADERS  += \
    mainwindow.h \
    objectstreeview.h \
    itemtypes.h \
    elementsimplelistwidget.h \
    newcomponentdialog.h \
    savestrategydialog.h \
    previewwindow.h \
    elementlibrarylistwidget.h \
    elementlibrarylistwidgetitem.h \
    GraphPage.h \
    GraphPageManager.h \
    XmlGraphPageConfigWriter.h \
    ProjectTreeView.h \
    UndoCommand.h \
    DrawAppDefs.h \
    ElementLibraryWidget.h

FORMS    += \
    mainwindow.ui \
    newcomponentdialog.ui \
    savestrategydialog.ui \
    previewwindow.ui \
    ElementLibraryWidget.ui

RESOURCES += \
    images.qrc

RC_FILE = application.rc

include(graphicselements/graphicselements.pri)
include(propertyeditor/propertyeditor.pri)
include(indicationeditor/indicationeditor.pri)
