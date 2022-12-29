TEMPLATE=lib
CONFIG += qt dll qtpropertybrowser-buildlib
mac:CONFIG += absolute_library_soname
#win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
include(../src/qtpropertybrowser.pri)

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

TARGET = $$QTPROPERTYBROWSER_LIBNAME$${LIB_SUFFIX}
DESTDIR = $$QTPROPERTYBROWSER_LIBDIR

INSTALL_LIBRARY_PATH = $$OUT_PWD/../../../../HmiFuncDesignerBin/bin

win32 {
    DLLDESTDIR = $$INSTALL_LIBRARY_PATH
    QMAKE_DISTCLEAN += $$INSTALL_LIBRARY_PATH/$${QTPROPERTYBROWSER_LIBNAME}$${LIB_SUFFIX}.dll
}
unix {
    DESTDIR =  $$INSTALL_LIBRARY_PATH
}
#target.path = $$DESTDIR
target.path = $$INSTALL_LIBRARY_PATH
INSTALLS += target

