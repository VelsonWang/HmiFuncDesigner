
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

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


