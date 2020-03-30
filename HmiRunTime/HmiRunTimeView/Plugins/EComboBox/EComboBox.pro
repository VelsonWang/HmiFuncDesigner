include(../../HmiRunTimeViewPlugin.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    EComboBoxPlugin.cpp \
    ElementComboBox.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    EComboBoxPlugin.h \
    ElementComboBox.h

DISTFILES += EComboBox.json


