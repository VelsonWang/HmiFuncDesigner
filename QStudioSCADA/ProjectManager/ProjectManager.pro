
include(../QStudioSCADA.pri)

QT += core gui xml
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += script


TARGET = project
TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH


INCLUDEPATH += .
INCLUDEPATH += $$PWD/../common/qtsingleapplication
INCLUDEPATH += $$PWD/../common/register

include($$PWD/../common/qtsingleapplication/qtsingleapplication.pri)
include($$PWD/../common/register/register.pri)

#CONFIG += debug_and_release
#CONFIG += release
#DEFINES += QT_NO_DEBUG_OUTPUT

#Use the -Lpath command to add a path to the list of directories to search for libraries. 
#Use -llibrary (dash, lower case L,library name) to add a reference to the actual library.

#message(libout_path $$IDE_LIBRARY_PATH)
CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -ledncryptd
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lHelperd
    LIBS += -L$$IDE_LIBRARY_PATH -lCsvd
    LIBS += -L$$IDE_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}d
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowserd
    LIBS += -L$$LINK_LIBRARY_PATH -lLuad
    LIBS += -L$$LINK_LIBRARY_PATH -lXmlUtilsd
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
    #message(LIBS $$LIBS)
}
else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -ledncrypt
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lHelper
    LIBS += -L$$IDE_LIBRARY_PATH -lCsv
    LIBS += -L$$IDE_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$LINK_LIBRARY_PATH -lLua
    LIBS += -L$$LINK_LIBRARY_PATH -lXmlUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
}

# debug: The project is being built in debug mode.
# release: The project is being built in release mode.
# win32: The project is being built in a Windows environment.
# macx: The project is being built in a Mac OS X environment.
# unix (including Linux): The project is being built in a Unix environment.


SOURCES += main.cpp\
        MainWindow.cpp \
    NewProjectDialog.cpp \
    SystemParametersWin.cpp \
    NetSettingDialog.cpp \
    DatabaseSettingDialog.cpp \
    UserAuthorityDialog.cpp \
    CommunicationDeviceWin.cpp \
    NewComDeviceDialog.cpp \
    ChildBase.cpp \
    LinkManager.cpp \
    DeviceBase.cpp \
    VariableManagerWin.cpp \
    VariableEditDialog.cpp \
    CommentsDialog.cpp \
    NewVariableGroupDialog.cpp \
    DeviceListDialog.cpp \
    SelectProtocolDialog.cpp \
    DrawPageWin.cpp \
    ProjectDownloadDialog.cpp \
    ProjectUploadDialog.cpp \
    AboutDialog.cpp \
    RealTimeDatabaseWin.cpp \
    NewNetDeviceDialog.cpp \
    ScriptManageWin.cpp \
    ScriptConditionConfigForm.cpp \
    ScriptEditorDlg.cpp \
    TagFuncEditDialog.cpp \
    ../Public/PubTool.cpp \
    SplitterForm.cpp \
    InsertFunctionDialog.cpp \
    InsertTagDialog.cpp \
    ScriptRunConditionEditorDlg.cpp \
    ChildForm.cpp \
    ProjectMgrUtils.cpp \
    ProjectInfoManger.cpp

HEADERS  += MainWindow.h \
    NewProjectDialog.h \
    ../Public/Public.h \
    SystemParametersWin.h \
    NetSettingDialog.h \
    DatabaseSettingDialog.h \
    UserAuthorityDialog.h \
    CommunicationDeviceWin.h \
    NewComDeviceDialog.h \
    ChildBase.h \
    LinkManager.h \
    DeviceBase.h \
    VariableManagerWin.h \
    VariableEditDialog.h \
    CommentsDialog.h \
    NewVariableGroupDialog.h \
    DeviceListDialog.h \
    SelectProtocolDialog.h \
    DrawPageWin.h \
    ProjectDownloadDialog.h \
    ProjectUploadDialog.h \
    AboutDialog.h \
    RealTimeDatabaseWin.h \
    NewNetDeviceDialog.h \
    ScriptManageWin.h \
    ScriptConditionConfigForm.h \
    ScriptEditorDlg.h \
    TagFuncEditDialog.h \
    ../Public/PubTool.h \
    SplitterForm.h \
    InsertFunctionDialog.h \
    InsertTagDialog.h \
    ScriptRunConditionEditorDlg.h \
    FileTansfer.h \
    ChildForm.h \
    ProjectMgrUtils.h \
    ProjectInfoManger.h

FORMS    += MainWindow.ui \
    NewProjectDialog.ui \
    SystemParametersWin.ui \
    NetSettingDialog.ui \
    DatabaseSettingDialog.ui \
    UserAuthorityDialog.ui \
    CommunicationDeviceWin.ui \
    NewComDeviceDialog.ui \
    VariableManagerWin.ui \
    VariableEditDialog.ui \
    CommentsDialog.ui \
    NewVariableGroupDialog.ui \
    DeviceListDialog.ui \
    SelectProtocolDialog.ui \
    DrawPageWin.ui \
    ProjectDownloadDialog.ui \
    ProjectUploadDialog.ui \
    AboutDialog.ui \
    RealTimeDatabaseWin.ui \
    NewNetDeviceDialog.ui \
    ScriptConditionConfigForm.ui \
    ScriptEditorDlg.ui \
    InsertFunctionDialog.ui \
    InsertTagDialog.ui \
    ScriptRunConditionEditorDlg.ui \
    ChildForm.ui

RESOURCES += \
    images.qrc \
    images1.qrc

# windows application icon
RC_FILE = ProjectManager.rc

#or
#RC_ICONS = project.ico
