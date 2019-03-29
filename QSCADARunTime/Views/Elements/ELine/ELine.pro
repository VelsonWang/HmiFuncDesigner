
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
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

FORMS +=


