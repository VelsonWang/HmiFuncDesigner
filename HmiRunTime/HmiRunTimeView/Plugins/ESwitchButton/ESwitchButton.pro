include(../../HmiRunTimeViewPlugin.pri)


SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    ElementSwitchButton.cpp \
    ESwitchButtonPlugin.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    ElementSwitchButton.h \
    ESwitchButtonPlugin.h

DISTFILES += SwitchButton.json



