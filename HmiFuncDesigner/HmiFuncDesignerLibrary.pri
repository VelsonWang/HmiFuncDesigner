include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$QTC_LIB_NAME

include(HmiFuncDesigner.pri)


win32 {
    DLLDESTDIR = $$IDE_APP_PATH
}

DESTDIR = $$IDE_LIBRARY_PATH

TARGET = $$qtLibraryName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

!macx {
    win32 {
        dlltarget.path = $$INSTALL_BIN_PATH
        INSTALLS += dlltarget
    } else {
        target.path = $$INSTALL_LIBRARY_PATH
        INSTALLS += target
    }
}
