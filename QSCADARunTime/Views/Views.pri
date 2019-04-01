
QT += core
QT += gui
QT += xml
QT += widgets


INCLUDEPATH += \
    $$PWD/ \
    $$PWD/Public \
    $$PWD/Elements \
    $$PWD/Elements/EArrow \
    $$PWD/Elements/ELine \
    $$PWD/Elements/EEllipse \
    $$PWD/Elements/ERect \
    $$PWD/Elements/EText


SOURCES += \
    $$PWD/GraphPage.cpp \
    $$PWD/GraphPageManager.cpp \
    $$PWD/UndoCommand.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/Public/PubTool.cpp \
    $$PWD/Public/Element.cpp \
    $$PWD/Public/ElementGroup.cpp \
    $$PWD/ProjectInfoManger.cpp

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
    $$PWD/Elements/EText/EText.cpp

HEADERS  += \
    $$PWD/GraphPage.h \
    $$PWD/GraphPageManager.h \
    $$PWD/UndoCommand.h \
    $$PWD/MainWindow.h \
    $$PWD/Public/PublicDefine.h \
    $$PWD/Public/PubTool.h \
    $$PWD/IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    $$PWD/Public/Element.h \
    $$PWD/Public/ElementGroup.h \
    $$PWD/ProjectInfoManger.h

HEADERS  += \
    $$PWD/Elements/EArrow/ElementArrow.h \
    $$PWD/Elements/EArrow/EArrow.h \
    $$PWD/Elements/ELine/ElementLine.h \
    $$PWD/Elements/ELine/ELine.h \
    $$PWD/Elements/EEllipse/ElementEllipse.h \
    $$PWD/Elements/EEllipse/EEllipse.h \
    $$PWD/Elements/ERect/ElementRect.h \
    $$PWD/Elements/ERect/ERect.h \
    $$PWD/Elements/EText/ElementText.h \
    $$PWD/Elements/EText/EText.h

FORMS    +=



