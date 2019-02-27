
include(../DrawApplicationPlugin.pri)


include(../propertyeditor/propertyeditor.pri)

SOURCES += ELinePlugin.cpp \
    ElementLine.cpp \
    ../Public/Element.cpp

HEADERS += ELinePlugin.h \
    ../Public/PublicDefine.h \
    ElementLine.h \
    ../Public/PublicDefine.h \
    ../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../Public/Element.h

DISTFILES += ELine.json

RESOURCES += \
    images.qrc

FORMS +=


