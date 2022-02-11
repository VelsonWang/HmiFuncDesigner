include(../../DrawApplicationPlugin.pri)
include(../../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjData

SOURCES += \
    ../../Public/Element.cpp \
    ElementSwitchButton.cpp \
    SwitchButtonPlugin.cpp \
    ../../Public/PubTool.cpp \
    editbasicpropertydialog.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementSwitchButton.h \
    SwitchButtonPlugin.h \
    ../../Public/PubTool.h \
    editbasicpropertydialog.h

DISTFILES += SwitchButton.json

RESOURCES += \
    images.qrc

FORMS += \
    editbasicpropertydialog.ui


