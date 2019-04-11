
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
}

SOURCES += \
    ../../Public/Element.cpp \
    ElementText.cpp \
    TextPlugin.cpp \
    ../../Public/PubTool.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementText.h \
    TextPlugin.h \
    ../../Public/PubTool.h

DISTFILES += EText.json

RESOURCES += \
    images.qrc

FORMS +=


