
include(../QStudioSCADA.pri)

QT += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RtdbView
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
    #LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtilsd
    #LIBS += -L$$IDE_LIBRARY_PATH -lHelperd
    #LIBS += -L$$IDE_LIBRARY_PATH -lCsvd
    #message(LIBS $$LIBS)
}
else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    #LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtils
    #LIBS += -L$$IDE_LIBRARY_PATH -lHelper
    #LIBS += -L$$IDE_LIBRARY_PATH -lCsv
}

UI_DIR=../../build-QStudioSCADA/RtdbView/ui
MOC_DIR=../../build-QStudioSCADA/RtdbView/moc
OBJECTS_DIR=../../build-QStudioSCADA/RtdbView/obj
RCC_DIR=../../build-QStudioSCADA/RtdbView/rcc

SOURCES += main.cpp\
        MainWindow.cpp \
    MdiChildWindow.cpp \
    Tag.cpp \
    RtdbConnectDialog.cpp \
    CommandLineParser.cpp \
    GetRtdbWriteValueDialog.cpp

HEADERS  += MainWindow.h \
    MdiChildWindow.h \
    Tag.h \
    ../Public/Public.h \
    RtdbConnectDialog.h \
    CommandLineParser.h \
    GetRtdbWriteValueDialog.h

FORMS    += MainWindow.ui \
    RtdbConnectDialog.ui \
    GetRtdbWriteValueDialog.ui




target.path = $$DESTDIR
INSTALLS += target


# windows application icon
RC_FILE = RtdbView.rc

RESOURCES += \
    images.qrc


