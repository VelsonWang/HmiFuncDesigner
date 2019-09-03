
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowserd
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
    }
    unix {
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    }
} else { # release
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
}

SOURCES += ELinePlugin.cpp \
    ../../Public/Element.cpp \
    ElementLine.cpp

HEADERS += ELinePlugin.h \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementLine.h

DISTFILES += ELine.json

RESOURCES += \
    images.qrc



