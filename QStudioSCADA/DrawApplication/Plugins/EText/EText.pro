
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ElementText.cpp \
    TextPlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementText.h \
    TextPlugin.h

DISTFILES += EText.json

RESOURCES += \
    images.qrc

FORMS +=


