
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

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


