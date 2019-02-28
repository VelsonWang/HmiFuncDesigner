
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ElementPolygon.cpp \
    PolygonPlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementPolygon.h \
    PolygonPlugin.h

DISTFILES += EPolygon.json

RESOURCES += \
    images.qrc

FORMS +=


