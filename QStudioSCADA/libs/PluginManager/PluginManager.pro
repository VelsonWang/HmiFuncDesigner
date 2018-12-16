
include(../../QStudioSCADAlibrary.pri)

DEFINES += PLUGINMANAGER_LIBRARY

SOURCES += pluginmanager.cpp

HEADERS += pluginmanager.h\
        pluginmanager_global.h \
    iplugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
