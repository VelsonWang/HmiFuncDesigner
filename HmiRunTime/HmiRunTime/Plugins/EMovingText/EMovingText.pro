include(../../HmiRunTimeViewPlugin.pri)


SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    ElementMovingText.cpp \
    EMovingTextPlugin.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    ElementMovingText.h \
    EMovingTextPlugin.h

DISTFILES += EMovingText.json



