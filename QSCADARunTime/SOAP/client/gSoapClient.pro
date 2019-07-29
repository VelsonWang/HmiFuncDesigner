
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gSoapClient
TEMPLATE = app

win32 {
    LIBS += -lpthread -lwsock32 -lws2_32
} else {

}


INCLUDEPATH += $$PWD

include($$PWD/../public/gsoap_client.pri)

SOURCES += main.cpp \
    client.cpp

HEADERS  += \
    client.h

FORMS    +=
