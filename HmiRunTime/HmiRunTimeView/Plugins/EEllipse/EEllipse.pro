include(../../HmiRunTimeViewPlugin.pri)


SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/RealTimeDB.cpp \
    ElementEllipse.cpp \
    EllipsePlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    ElementEllipse.h \
    EllipsePlugin.h

DISTFILES += EEllipse.json


