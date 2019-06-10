
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
}

SOURCES += \
    ../../Public/Element.cpp \
    ElementPicture.cpp \
    PicturePlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementPicture.h \
    PicturePlugin.h

DISTFILES += EPicture.json

RESOURCES += \
    images.qrc

FORMS +=


