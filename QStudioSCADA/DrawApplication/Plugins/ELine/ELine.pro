
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

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


