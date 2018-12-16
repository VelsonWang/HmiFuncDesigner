
include(../DrawApplicationPlugin.pri)

#message(libout_path $$IDE_LIBRARY_PATH)
CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowserd
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowser
}

SOURCES += ELinePlugin.cpp \
    ELine.cpp \
    ../Public/stdelement.cpp \
    ../Public/PubTool.cpp \
    EditElementPropertyDialog.cpp \

HEADERS += ELinePlugin.h \
    ELine.h \
    ../Public/IElement.h \
    ../Public/PublicDefine.h \
    ../Public/PubTool.h \
    EditElementPropertyDialog.h

DISTFILES += ELine.json

RESOURCES += \
    images.qrc

FORMS += \
    EditElementPropertyDialog.ui


