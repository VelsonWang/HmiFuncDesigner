QT += core
QT += gui
QT += widgets
QT += network
QT += script
QT += sql
QT += xml

TARGET_BIN_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../HmiRunTimeBin)
LINK_LIBRARY_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../lib)
#message("TARGET_BIN_PATH: "$$TARGET_BIN_PATH)
#message("LINK_LIBRARY_PATH: "$$LINK_LIBRARY_PATH)

DESTDIR = $$TARGET_BIN_PATH

TARGET = HmiRunTimeView
TEMPLATE = app

# turn on console
CONFIG(debug, debug|release) { #debug
    CONFIG += console
    DEFINES += BUILD_BY_DEBUG
}


CONFIG -= app_bundle
CONFIG += C++11


#去除UNICODE字符编码
DEFINES -= UNICODE

include(../../log4qt/log4qt.pri)

INCLUDEPATH += \
    $$_PRO_FILE_PWD_ \
    Vendor \
    Port \
    qextserial \
    Public \
    DB \
    Tag \
    Event \
    Script \
    Log \
    Socket \
    Service \
    ../Public

LIBRARY_SRC_PATH = $$clean_path($$_PRO_FILE_PWD_/../../../HmiFuncDesigner)
#message("LIBRARY_SRC_PATH: "$$LIBRARY_SRC_PATH)

INCLUDEPATH += \
    $$LIBRARY_SRC_PATH/libs/edncrypt \
    $$LIBRARY_SRC_PATH/libs/DrawUtils \
    $$LIBRARY_SRC_PATH/libs/Utils \
    $$LIBRARY_SRC_PATH/libs/ProjectDataUtils

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

LIBS += -L$$LINK_LIBRARY_PATH -ledncrypt$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}

win32 {
    LIBS += -lpthread -lwsock32 -lws2_32
} else {

}

SOURCES += \
    ../Public/Element.cpp \
    ../Public/ElementGroup.cpp \
    ../Public/PubTool.cpp \
    GraphPage.cpp \
    GraphPageManager.cpp \
    MainWindow.cpp \
    ViewElementPluginManager.cpp \
    main.cpp \
    MessageTransfer.cpp \
    ../Public/RealTimeDB.cpp \
    HmiRunTime.cpp \
    Tag/Function.cpp \
    Event/Event.cpp \
    Script/JavaScript.cpp \
    Script/RunScript.cpp \
    SysRuntimeEvent.cpp \
    Log/Log.cpp \
    Public/Global.cpp


HEADERS  += \
    ../Public/Element.h \
    ../Public/ElementGroup.h \
    ../Public/PubTool.h \
    ../Public/PublicDefine.h \
    GraphPage.h \
    GraphPageManager.h \
    IVendor.h \
    MainWindow.h \
    MessageTransfer.h \
    ../Public/RealTimeDB.h \
    Public/public.h \
    HmiRunTime.h \
    Public/PublicFunction.h \
    Event/Event.h \
    Tag/Function.h \
    Script/JavaScript.h \
    Script/RunScript.h \
    FileTansfer.h \
    SysRuntimeEvent.h \
    Singleton.h \
    Log/Log.h \
    VersionInfo.h \
    Public/Global.h \
    ViewElementPluginManager.h

win32 {
     SOURCES +=
}

unix {
     SOURCES +=
}


DISTFILES += \
    setting.ini

# copy config files to applicationDirPath
config.path = $$TARGET_BIN_PATH
config.files = ./*.ini
INSTALLS += config

