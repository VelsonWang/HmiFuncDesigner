
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
}

SOURCES += \
    ../../Public/Element.cpp \
    ElementEllipse.cpp \
    EllipsePlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementEllipse.h \
    EllipsePlugin.h

DISTFILES += EEllipse.json

RESOURCES += \
    images.qrc

FORMS +=


