
include(../../DrawApplicationPlugin.pri)
include(../../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowserd
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}d
    }
    unix {
        LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowser
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
}


SOURCES += \
    ../../Public/Element.cpp \
    ElementSwitchButton.cpp \
    SwitchButtonPlugin.cpp \
    ../../Public/PubTool.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementSwitchButton.h \
    SwitchButtonPlugin.h \
    ../../Public/PubTool.h

DISTFILES += SwitchButton.json

RESOURCES += \
    images.qrc




