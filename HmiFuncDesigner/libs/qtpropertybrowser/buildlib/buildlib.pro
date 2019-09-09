TEMPLATE=lib
CONFIG += qt dll qtpropertybrowser-buildlib
mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
include(../src/qtpropertybrowser.pri)
TARGET = $$QTPROPERTYBROWSER_LIBNAME
DESTDIR = $$QTPROPERTYBROWSER_LIBDIR

INSTALL_LIBRARY_PATH = $$OUT_PWD/../../../../HmiFuncDesignerBin/bin

win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QTPROPERTYBROWSER_LIBNAME}.dll
}
#target.path = $$DESTDIR
target.path = $$INSTALL_LIBRARY_PATH
INSTALLS += target
