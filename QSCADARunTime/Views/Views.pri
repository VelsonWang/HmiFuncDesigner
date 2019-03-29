
QT += core
QT += gui
QT += xml
QT += widgets


INCLUDEPATH += \
    $$PWD/ \
    $$PWD/Public \
    $$PWD/Elements \
    $$PWD/Elements/EArrow



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
    $$PWD/Elements/EArrow/EArrow.cpp

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
    $$PWD/Elements/EArrow/EArrow.h

FORMS    +=



