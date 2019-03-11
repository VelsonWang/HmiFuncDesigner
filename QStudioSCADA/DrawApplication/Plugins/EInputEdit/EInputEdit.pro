
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
}

SOURCES += \
    ../../Public/Element.cpp \
    ElementInputEdit.cpp \
    InputEditPlugin.cpp \
    ../../Public/PubTool.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementInputEdit.h \
    InputEditPlugin.h \
    ../../Public/PubTool.h

DISTFILES += InputEdit.json

RESOURCES += \
    images.qrc

FORMS +=


