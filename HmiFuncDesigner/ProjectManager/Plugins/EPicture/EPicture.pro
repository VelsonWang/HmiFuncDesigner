include(../../DrawApplicationPlugin.pri)
include(../../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjData

SOURCES += \
    ../../Public/Element.cpp \
    ElementPicture.cpp \
    PicturePlugin.cpp \
    editbasicpropertydialog.cpp

HEADERS += \
    ../../Public/PublicDefine.h \
    ../../Public/PublicDefine.h \
    ../../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../../Public/Element.h \
    ElementPicture.h \
    PicturePlugin.h \
    editbasicpropertydialog.h

DISTFILES += EPicture.json

RESOURCES += \
    images.qrc

FORMS += \
    editbasicpropertydialog.ui


