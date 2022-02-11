include(../../HmiRunTimeViewPlugin.pri)

SOURCES += \
    ../../Public/Element.cpp \
    ../../Public/PubTool.cpp \
    ../../Public/RealTimeDB.cpp \
    ../../SoftKeyboard/InputMethodAlphabet.cpp \
    ../../SoftKeyboard/InputMethodNumber.cpp \
    ElementInputEdit.cpp \
    EInputEditPlugin.cpp \
    InputLineEdit.cpp

HEADERS += \
    ../../Public/PubTool.h \
    ../../Public/PublicDefine.h \
    ../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h \
    ../../Public/Element.h \
    ../../Public/RealTimeDB.h \
    ../../SoftKeyboard/InputMethodAlphabet.h \
    ../../SoftKeyboard/InputMethodNumber.h \
    ElementInputEdit.h \
    EInputEditPlugin.h \
    InputLineEdit.h

DISTFILES += InputEdit.json

FORMS += \
    ../../SoftKeyboard/InputMethodAlphabet.ui \
    ../../SoftKeyboard/InputMethodNumber.ui



