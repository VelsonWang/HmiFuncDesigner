
include(../../QStudioSCADAlibrary.pri)

QT += network

CONFIG -= shared dll
CONFIG += staticlib

SOURCES += qftp.cpp qurlinfo.cpp

HEADERS += qftp.h qurlinfo.h


