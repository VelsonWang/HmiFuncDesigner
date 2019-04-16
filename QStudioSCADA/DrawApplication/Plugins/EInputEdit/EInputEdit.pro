
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
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


