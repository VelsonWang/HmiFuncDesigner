
include(../../QStudioSCADA.pri)

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport


TARGET = DrawApplication
TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH

#CONFIG += debug_and_release
#CONFIG += release
#message(libout_path $$IDE_LIBRARY_PATH)
CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowserd
    LIBS += -L$$IDE_LIBRARY_PATH -lHelperd
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$IDE_LIBRARY_PATH -lHelper
}


UI_DIR=../../../build-QStudioSCADA/DrawApplication/ui
MOC_DIR=../../../build-QStudioSCADA/DrawApplication/moc
OBJECTS_DIR=../../../build-QStudioSCADA/DrawApplication/obj
RCC_DIR=../../../build-QStudioSCADA/DrawApplication/rcc

# debug: The project is being built in debug mode.
# release: The project is being built in release mode.
# win32: The project is being built in a Windows environment.
# macx: The project is being built in a Mac OS X environment.
# unix (including Linux): The project is being built in a Unix environment.

# win32 {
# 	LIBS += -lmywin32lib
# } else macx {
# 	LIBS += -lmymacxlib
# } else {
# 	LIBS += -lmyunixlib
# }





SOURCES += main.cpp\
        MainWindow.cpp \
    GraphPage.cpp \
    ChildGraphWin.cpp \
    PaintArea.cpp \
    ../Public/StdElement.cpp \
    ToolButton.cpp \
    ../Public/PubTool.cpp

HEADERS  += MainWindow.h \
    ../Public/IElement.h \
    PublicDefine.h \
    GraphPage.h \
    ChildGraphWin.h \
    PaintArea.h \
    ../Public/PublicDefine.h \
    ToolButton.h \
    ../Public/PubTool.h \
    ../Public/DrawAppEvent.h

FORMS    += MainWindow.ui \
    ChildGraphWin.ui

RESOURCES += \
    images.qrc








