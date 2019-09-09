
#message("variable _PRO_FILE_ is: $$_PRO_FILE_")
#message("variable _PRO_FILE_PWD_ is:$$_PRO_FILE_PWD_")

#target.path = $$INSTALL_LIBRARY_PATH
#INSTALLS += target

#DEFINES += QT_NO_DEBUG_OUTPUT

include(HmiFuncDesigner.pri)

# QT Version 5.5.1
#version check qt
!minQtVersion(5, 5, 1) {
    message("Cannot build Qt Creator with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.5.1.")
}

TEMPLATE = subdirs

CONFIG += ordered


SUBDIRS += \
    libs \
    Devices \
    ProjectManager \
    SysTagManager \
    RtdbView \
    DrawApplication \
#    JsEditor
    RegisterTool \    # 通用授权工具
    EDncryptTool      # 加密解密工具



