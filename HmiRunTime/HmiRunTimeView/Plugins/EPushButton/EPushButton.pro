include(../../HmiRunTimeViewPlugin.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    ElementPushButton.cpp \
    EPushButtonPlugin.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    ElementPushButton.h \
    EPushButtonPlugin.h

DISTFILES += PushButton.json



