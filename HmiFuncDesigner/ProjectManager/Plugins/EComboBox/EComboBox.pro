include(../../DrawApplicationPlugin.pri)
include(../../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

LIBS += -L$$IDE_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjData

SOURCES += \
    ../../Public/Element.cpp \
    editbasicpropertydialog.cpp \
    ElementComboBox.cpp \
    ComboBoxPlugin.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    editbasicpropertydialog.h \
    ElementComboBox.h \
    ComboBoxPlugin.h

DISTFILES += EComboBox.json

RESOURCES += \
    images.qrc

FORMS += \
    editbasicpropertydialog.ui

