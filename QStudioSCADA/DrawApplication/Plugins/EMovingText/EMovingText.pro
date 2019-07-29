
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
    }
    unix {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
}

SOURCES += \
    ../../PropertyEditorEx/tagtextlisteditordialog.cpp \
    ../../PropertyEditorEx/tagtextlistproperty.cpp \
    ../../PropertyEditorEx/tagtextlistpropertyeditor.cpp \
    ../../Public/Element.cpp \
    ElementMovingText.cpp \
    MovingTextPlugin.cpp \
    ../../PropertyEditorEx/FunctionEditorDialog.cpp \
    ../../PropertyEditorEx/functionproperty.cpp \
    ../../PropertyEditorEx/functionpropertyeditor.cpp

HEADERS += \
    ../../PropertyEditorEx/tagtextlisteditordialog.h \
    ../../PropertyEditorEx/tagtextlistproperty.h \
    ../../PropertyEditorEx/tagtextlistpropertyeditor.h \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementMovingText.h \
    MovingTextPlugin.h \
    ../../PropertyEditorEx/FunctionEditorDialog.h \
    ../../PropertyEditorEx/functionproperty.h \
    ../../PropertyEditorEx/functionpropertyeditor.h

DISTFILES += EMovingText.json

RESOURCES += \
    images.qrc

FORMS += \
    ../../PropertyEditorEx/FunctionEditorDialog.ui \
    ../../PropertyEditorEx/tagtextlisteditordialog.ui


