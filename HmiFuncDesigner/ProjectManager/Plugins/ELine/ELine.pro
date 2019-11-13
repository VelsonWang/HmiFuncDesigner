
include(../../DrawApplicationPlugin.pri)

LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}


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



