
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
    }
    unix {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
}


SOURCES += \
    ../../Public/Element.cpp \
    ElementPushButton.cpp \
    PushButtonPlugin.cpp \
    ../../Public/PubTool.cpp \
    ../../PropertyEditorEx/FunctionEditorDialog.cpp \
    ../../PropertyEditorEx/functionproperty.cpp \
    ../../PropertyEditorEx/functionpropertyeditor.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementPushButton.h \
    PushButtonPlugin.h \
    ../../Public/PubTool.h \
    ../../PropertyEditorEx/FunctionEditorDialog.h \
    ../../PropertyEditorEx/functionproperty.h \
    ../../PropertyEditorEx/functionpropertyeditor.h

DISTFILES += PushButton.json

RESOURCES += \
    images.qrc

FORMS += \
    ../../PropertyEditorEx/FunctionEditorDialog.ui


