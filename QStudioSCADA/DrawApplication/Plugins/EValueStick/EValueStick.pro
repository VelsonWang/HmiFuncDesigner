
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
    }
    unix {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
}

SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ElementValueStick.cpp \
    ValueStickPlugin.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementValueStick.h \
    ValueStickPlugin.h

DISTFILES += EValueStick.json

RESOURCES += \
    images.qrc

FORMS +=


