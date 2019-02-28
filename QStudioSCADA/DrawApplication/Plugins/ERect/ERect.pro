
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ElementRect.cpp \
    RectPlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementRect.h \
    RectPlugin.h

DISTFILES += ERect.json

RESOURCES += \
    images.qrc

FORMS +=


