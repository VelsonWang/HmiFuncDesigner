
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
    }
    unix {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
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


