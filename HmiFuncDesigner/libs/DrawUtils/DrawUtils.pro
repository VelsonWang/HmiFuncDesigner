
include(../../HmiFuncDesignerLibrary.pri)

QT += core gui xml widgets

CONFIG -= shared dll
CONFIG += staticlib


SOURCES += \
	DrawListUtils.cpp \
	ElementIDHelper.cpp

HEADERS += \
	DrawListUtils.h \
	ElementIDHelper.h

