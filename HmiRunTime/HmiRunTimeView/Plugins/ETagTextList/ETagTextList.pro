include(../../HmiRunTimeViewPlugin.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    ElementTagTextList.cpp \
    ETagTextListPlugin.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    ElementTagTextList.h \
    ETagTextListPlugin.h

DISTFILES += ETagTextList.json


