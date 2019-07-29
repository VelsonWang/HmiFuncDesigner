
include(../../DrawApplicationPlugin.pri)

CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditord
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
    }
    unix {
        LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    }
} else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lPropertyEditor
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
}

SOURCES += \
    ../../Public/Element.cpp \
    ElementEllipse.cpp \
    EllipsePlugin.cpp \
    ../../PropertyEditorEx/tagcolorlisteditordialog.cpp \
    ../../PropertyEditorEx/tagcolorlistproperty.cpp \
    ../../PropertyEditorEx/tagcolorlistpropertyeditor.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementEllipse.h \
    EllipsePlugin.h \
    ../../PropertyEditorEx/tagcolorlisteditordialog.h \
    ../../PropertyEditorEx/tagcolorlistproperty.h \
    ../../PropertyEditorEx/tagcolorlistpropertyeditor.h

DISTFILES += EEllipse.json

RESOURCES += \
    images.qrc

FORMS += \
    ../../PropertyEditorEx/tagcolorlisteditordialog.ui


