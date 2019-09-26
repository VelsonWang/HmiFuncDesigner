
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowserd
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}d
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
    }
    unix {
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    }
} else { # release
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
}

SOURCES += EArrowPlugin.cpp \
    ../../Public/Element.cpp \
    ElementArrow.cpp


HEADERS += EArrowPlugin.h \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementArrow.h

DISTFILES += EArrow.json

RESOURCES += \
    images.qrc

FORMS +=


