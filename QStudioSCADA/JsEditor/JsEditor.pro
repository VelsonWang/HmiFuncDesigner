
include(../QStudioSCADA.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JsEditor
DESTDIR = $$IDE_BIN_PATH
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}d
}
else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
}

UI_DIR=../../build-QStudioSCADA/JsEditor/ui
MOC_DIR=../../build-QStudioSCADA/JsEditor/moc
OBJECTS_DIR=../../build-QStudioSCADA/JsEditor/obj
RCC_DIR=../../build-QStudioSCADA/JsEditor/rcc

SOURCES += main.cpp\
        MainWindow.cpp \
    ScriptGeneratorForm.cpp

HEADERS  += MainWindow.h \
    ScriptGeneratorForm.h

RESOURCES = application.qrc

!macx {
    win32 {
        dlltarget.path = $$DESTDIR
        INSTALLS += dlltarget
    } else {
        target.path = $$DESTDIR
        INSTALLS += target
    }
}

FORMS += \
    ScriptGeneratorForm.ui
