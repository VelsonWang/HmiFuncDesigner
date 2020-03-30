include(../../HmiRunTimeViewPlugin.pri)


SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    EAnalogClockPlugin.cpp \
    ElementAnalogClock.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    EAnalogClockPlugin.h \
    ElementAnalogClock.h

DISTFILES += EAnalogClock.json





