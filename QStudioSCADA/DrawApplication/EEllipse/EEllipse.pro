
include(../DrawApplicationPlugin.pri)


include(../propertyeditor/propertyeditor.pri)

SOURCES += \
    ElementEllipse.cpp \
    ../Public/Element.cpp \
    EllipsePlugin.cpp

HEADERS += \
    ../Public/PublicDefine.h \
    ElementEllipse.h \
    ../Public/PublicDefine.h \
    ../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../Public/Element.h \
    EllipsePlugin.h

DISTFILES += EEllipse.json

RESOURCES += \
    images.qrc

FORMS +=


