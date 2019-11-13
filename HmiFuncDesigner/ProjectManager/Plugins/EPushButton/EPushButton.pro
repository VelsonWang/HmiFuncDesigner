
include(../../DrawApplicationPlugin.pri)
include(../../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}


SOURCES += \
    ../../Public/Element.cpp \
    ElementPushButton.cpp \
    PushButtonPlugin.cpp \
    ../../Public/PubTool.cpp \
    editbasicpropertydialog.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementPushButton.h \
    PushButtonPlugin.h \
    ../../Public/PubTool.h \
    editbasicpropertydialog.h

DISTFILES += PushButton.json

RESOURCES += \
    images.qrc

FORMS += \
    editbasicpropertydialog.ui


