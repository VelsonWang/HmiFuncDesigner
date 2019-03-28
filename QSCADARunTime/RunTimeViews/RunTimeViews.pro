
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network
QT += script
QT += sql

TARGET_BIN_PATH = $$_PRO_FILE_PWD_/../../RuntimeBin
LINK_LIBRARY_PATH = $$_PRO_FILE_PWD_/../../lib

DESTDIR = $$TARGET_BIN_PATH

TARGET = RunTimeViews
TEMPLATE = app

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
}

CONFIG -= app_bundle
CONFIG += C++11

INCLUDEPATH += $$_PRO_FILE_PWD_

LIBRARY_SRC_PATH = $$_PRO_FILE_PWD_/../../QStudioSCADA
INCLUDEPATH += \
    $$LIBRARY_SRC_PATH/libs/edncrypt \
    $$LIBRARY_SRC_PATH/libs/ConfigUtils \
    $$LIBRARY_SRC_PATH/libs/Helper

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$LINK_LIBRARY_PATH -ledncryptd
    LIBS += -L$$LINK_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$LINK_LIBRARY_PATH -lHelperd
}
else { # release
    LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt
    LIBS += -L$$LINK_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lHelper
}

SOURCES += \
    main.cpp


HEADERS  +=


FORMS +=


# copy config files to applicationDirPath
config.path = $$TARGET_BIN_PATH
config.files = ./*.ini
INSTALLS += config

