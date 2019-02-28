
include(../DrawApplicationPlugin.pri)


include(../propertyeditor/propertyeditor.pri)

SOURCES += EArrowPlugin.cpp \
    ElementArrow.cpp \
    ../Public/Element.cpp

HEADERS += EArrowPlugin.h \
    ../Public/PublicDefine.h \
    ElementArrow.h \
    ../Public/PublicDefine.h \
    ../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../Public/Element.h

DISTFILES += EArrow.json

RESOURCES += \
    images.qrc

FORMS +=


