#include($$PWD/../../HmiFuncDesigner.pri)

QT += core
QT += gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

CONFIG += c++11


CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowserd
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
    }
    unix {
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
}

include($$PWD/../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

INCLUDEPATH += \
    $$PWD \
    $$PWD/../QtPropertyBrowserEx

SOURCES += \
    $$PWD/DrawMainWindow.cpp \
    $$PWD/GetWidthHeightDialog.cpp \
#    main.cpp \
    $$PWD/GraphPage.cpp \
    $$PWD/GraphPageManager.cpp \
    $$PWD/UndoCommand.cpp \
    $$PWD/ElementLibraryWidget.cpp \
    $$PWD/ObjectsTreeView.cpp \
    $$PWD/ElementLibraryListWidget.cpp \
    $$PWD/ElementLibraryListWidgetItem.cpp \
    $$PWD/ElementSimpleListWidget.cpp \
    $$PWD/GraphPageTreeView.cpp \
    $$PWD/../Public/PubTool.cpp \
    $$PWD/../Public/Element.cpp \
    $$PWD/../Public/ElementGroup.cpp \
    $$PWD/PluginManager.cpp

HEADERS  += \
    $$PWD/DrawMainWindow.h \
    $$PWD/GetWidthHeightDialog.h \
    $$PWD/GraphPage.h \
    $$PWD/GraphPageManager.h \
    $$PWD/UndoCommand.h \
    $$PWD/ElementLibraryWidget.h \
    $$PWD/ObjectsTreeView.h \
    $$PWD/ElementLibraryListWidget.h \
    $$PWD/ElementLibraryListWidgetItem.h \
    $$PWD/ElementSimpleListWidget.h \
    $$PWD/GraphPageTreeView.h \
    $$PWD/../Public/PublicDefine.h \
    $$PWD/../Public/PubTool.h \
    $$PWD/../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    $$PWD/../Public/Element.h \
    $$PWD/../Public/ElementGroup.h \
    $$PWD/PluginManager.h

FORMS    += \
    $$PWD/DrawMainWindow.ui \
    $$PWD/ElementLibraryWidget.ui \
    $$PWD/GetWidthHeightDialog.ui

RESOURCES += \
    $$PWD/DrawAppImages.qrc

######################################################

DISTFILES += \
    $$IDE_BIN_PATH/Config/DrawAppJScriptFun.xml \
    $$IDE_BIN_PATH/Config/ElementSupportEvents.xml


