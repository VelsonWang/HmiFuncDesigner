

include(../../QStudioSCADAlibrary.pri)

QT += sql

CONFIG -= shared dll
CONFIG += staticlib

SOURCES += \
    InputMethodAlphabet.cpp \
    InputMethodNumber.cpp

HEADERS += \
    InputMethodAlphabet.h \
    InputMethodNumber.h

FORMS += \
    InputMethodAlphabet.ui \
    InputMethodNumber.ui

DISTFILES += \
    py.db

