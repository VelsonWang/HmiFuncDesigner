include(../HmiFuncDesigner.pri)

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NativeRtdbView
DESTDIR = $$IDE_BIN_PATH
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#去除UNICODE字符编码
DEFINES -= UNICODE

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}


SOURCES += main.cpp\
        MainWindow.cpp \
    GetRtdbWriteValueDialog.cpp \
    RtdbTag.cpp \
    RealTimeDB.cpp

HEADERS  += MainWindow.h \
    ../Public/Public.h \
    GetRtdbWriteValueDialog.h \
    RtdbTag.h \
    RealTimeDB.h

FORMS    += MainWindow.ui \
    GetRtdbWriteValueDialog.ui




target.path = $$DESTDIR
INSTALLS += target


# windows application icon
RC_FILE = RtdbView.rc

RESOURCES += \
    images.qrc


