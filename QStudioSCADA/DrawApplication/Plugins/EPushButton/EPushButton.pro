
include(../../DrawApplicationPlugin.pri)


include(../../propertyeditor/propertyeditor.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ElementPushButton.cpp \
    PushButtonPlugin.cpp \
    ../../Public/PubTool.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementPushButton.h \
    PushButtonPlugin.h \
    ../../Public/PubTool.h

DISTFILES += PushButton.json

RESOURCES += \
    images.qrc

FORMS +=


