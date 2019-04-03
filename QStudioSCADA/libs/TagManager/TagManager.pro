

include(../../QStudioSCADAlibrary.pri)

CONFIG -= shared dll
CONFIG += staticlib

SOURCES += TagManager.cpp \
    DBVarGroup.cpp

HEADERS += TagManager.h \
    DBVarGroup.h


