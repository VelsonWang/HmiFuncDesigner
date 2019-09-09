QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle


INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/libhttp

SOURCES += \
    $$PWD/libhttp/qhttpconnection.cpp \
    $$PWD/libhttp/qhttprequest.cpp \
    $$PWD/libhttp/qhttpresponse.cpp \
    $$PWD/libhttp/qhttpserver.cpp \
#    libhttp/bench.c \
    $$PWD/libhttp/http_parser.c \
#    libhttp/parsertrace.c \
#    libhttp/url_parser.c \
    $$PWD/httpserver.cpp

DISTFILES += \
    $$PWD/libhttp/http_parser.gyp

HEADERS += \
    $$PWD/libhttp/http_parser.h \
    $$PWD/libhttp/qhttpconnection.h \
    $$PWD/libhttp/qhttprequest.h \
    $$PWD/libhttp/qhttpresponse.h \
    $$PWD/libhttp/qhttpserver.h \
    $$PWD/libhttp/qhttpserverapi.h \
    $$PWD/libhttp/qhttpserverfwd.h \
    $$PWD/httpserver.h
