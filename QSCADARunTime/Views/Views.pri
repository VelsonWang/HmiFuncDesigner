
QT += core
QT += gui
QT += xml
QT += widgets
QT += sql


INCLUDEPATH += \
    $$PWD/ \
    $$PWD/Public \
    $$PWD/Elements \
    $$PWD/Elements/EArrow \
    $$PWD/Elements/ELine \
    $$PWD/Elements/EEllipse \
    $$PWD/Elements/ERect \
    $$PWD/Elements/EText \
    $$PWD/Elements/EPicture \
    $$PWD/Elements/EInputEdit \
    $$PWD/Elements/EPushButton \
    $$PWD/SoftKeyboard \
    $$PWD/Elements/EIndicationLamp \
    $$PWD/Elements/ESwitchButton \
    $$PWD/Elements/EClock \
    $$PWD/Elements/EMovingText \
    $$PWD/Elements/ETagTextList \
    $$PWD/Elements/EValueStick \
    $$PWD/Elements/EJarShape

SOURCES += \
    $$PWD/GraphPage.cpp \
    $$PWD/GraphPageManager.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/Public/PubTool.cpp \
    $$PWD/Public/Element.cpp \
    $$PWD/Public/ElementGroup.cpp \
    $$PWD/SoftKeyboard/InputMethodAlphabet.cpp \
    $$PWD/SoftKeyboard/InputMethodNumber.cpp

SOURCES += \
    $$PWD/Elements/EArrow/ElementArrow.cpp \
    $$PWD/Elements/EArrow/EArrow.cpp \
    $$PWD/Elements/ELine/ElementLine.cpp \
    $$PWD/Elements/ELine/ELine.cpp \
    $$PWD/Elements/EEllipse/ElementEllipse.cpp \
    $$PWD/Elements/EEllipse/EEllipse.cpp \
    $$PWD/Elements/ERect/ElementRect.cpp \
    $$PWD/Elements/ERect/ERect.cpp \
    $$PWD/Elements/EText/ElementText.cpp \
    $$PWD/Elements/EText/EText.cpp \
    $$PWD/Elements/EPicture/ElementPicture.cpp \
    $$PWD/Elements/EPicture/EPicture.cpp \
    $$PWD/Elements/EInputEdit/ElementInputEdit.cpp \
    $$PWD/Elements/EInputEdit/EInputEdit.cpp \
    $$PWD/Elements/EInputEdit/InputLineEdit.cpp \
    $$PWD/Elements/EPushButton/ElementPushButton.cpp \
    $$PWD/Elements/EPushButton/EPushButton.cpp \
    $$PWD/Elements/EIndicationLamp/ElementIndicationLamp.cpp \
    $$PWD/Elements/EIndicationLamp/EIndicationLamp.cpp \
    $$PWD/Elements/ESwitchButton/ElementSwitchButton.cpp \
    $$PWD/Elements/ESwitchButton/ESwitchButton.cpp \
    $$PWD/Elements/EClock/ElementClock.cpp \
    $$PWD/Elements/EClock/EClock.cpp \
    $$PWD/Elements/EMovingText/ElementMovingText.cpp \
    $$PWD/Elements/EMovingText/EMovingText.cpp \
    $$PWD/Elements/ETagTextList/ElementTagTextList.cpp \
    $$PWD/Elements/ETagTextList/ETagTextList.cpp \
    $$PWD/Elements/EValueStick/ElementValueStick.cpp \
    $$PWD/Elements/EValueStick/EValueStick.cpp \
    $$PWD/Elements/EJarShape/ElementJarShape.cpp \
    $$PWD/Elements/EJarShape/EJarShape.cpp

HEADERS += \
    $$PWD/GraphPage.h \
    $$PWD/GraphPageManager.h \
    $$PWD/MainWindow.h \
    $$PWD/Public/PublicDefine.h \
    $$PWD/Public/PubTool.h \
    $$PWD/IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    $$PWD/Public/Element.h \
    $$PWD/Public/ElementGroup.h \
    $$PWD/SoftKeyboard/InputMethodAlphabet.h \
    $$PWD/SoftKeyboard/InputMethodNumber.h


HEADERS += \
    $$PWD/Elements/EArrow/ElementArrow.h \
    $$PWD/Elements/EArrow/EArrow.h \
    $$PWD/Elements/ELine/ElementLine.h \
    $$PWD/Elements/ELine/ELine.h \
    $$PWD/Elements/EEllipse/ElementEllipse.h \
    $$PWD/Elements/EEllipse/EEllipse.h \
    $$PWD/Elements/ERect/ElementRect.h \
    $$PWD/Elements/ERect/ERect.h \
    $$PWD/Elements/EText/ElementText.h \
    $$PWD/Elements/EText/EText.h \
    $$PWD/Elements/EPicture/ElementPicture.h \
    $$PWD/Elements/EPicture/EPicture.h \
    $$PWD/Elements/EInputEdit/ElementInputEdit.h \
    $$PWD/Elements/EInputEdit/EInputEdit.h \
    $$PWD/Elements/EInputEdit/InputLineEdit.h \
    $$PWD/Elements/EPushButton/ElementPushButton.h \
    $$PWD/Elements/EPushButton/EPushButton.h \
    $$PWD/Elements/EIndicationLamp/ElementIndicationLamp.h \
    $$PWD/Elements/EIndicationLamp/EIndicationLamp.h \
    $$PWD/Elements/ESwitchButton/ElementSwitchButton.h \
    $$PWD/Elements/ESwitchButton/ESwitchButton.h \
    $$PWD/Elements/EClock/ElementClock.h \
    $$PWD/Elements/EClock/EClock.h \
    $$PWD/Elements/EMovingText/ElementMovingText.h \
    $$PWD/Elements/EMovingText/EMovingText.h \
    $$PWD/Elements/ETagTextList/ElementTagTextList.h \
    $$PWD/Elements/ETagTextList/ETagTextList.h \
    $$PWD/Elements/EValueStick/ElementValueStick.h \
    $$PWD/Elements/EValueStick/EValueStick.h \
    $$PWD/Elements/EJarShape/ElementJarShape.h \
    $$PWD/Elements/EJarShape/EJarShape.h

FORMS += \
    $$PWD/SoftKeyboard/InputMethodAlphabet.ui \
    $$PWD/SoftKeyboard/InputMethodNumber.ui



