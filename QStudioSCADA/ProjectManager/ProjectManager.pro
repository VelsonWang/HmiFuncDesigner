
include(../QStudioSCADA.pri)

QT += core gui xml
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += script


TARGET = project
TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH

#CONFIG += debug_and_release
#CONFIG += release
#DEFINES += QT_NO_DEBUG_OUTPUT

#Use the -Lpath command to add a path to the list of directories to search for libraries. 
#Use -llibrary (dash, lower case L,library name) to add a reference to the actual library.

#win32:LIBS += -lmywin32lib
#macx:LIBS += -lmymacxlib
#!win32:!macx:LIBS += -lmyunixlib
#win32:debug:CONFIG += console

#message(libout_path $$IDE_LIBRARY_PATH)
CONFIG(debug, debug|release) { #debug
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtilsd
    LIBS += -L$$IDE_LIBRARY_PATH -lHelperd
    LIBS += -L$$IDE_LIBRARY_PATH -lCsvd
    LIBS += -L$$IDE_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}d
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowserd
    LIBS += -L$$LINK_LIBRARY_PATH -lLuad
    LIBS += -L$$LINK_LIBRARY_PATH -lXmlUtilsd
    #message(LIBS $$LIBS)
}
else { # release
    LIBS += -L$$IDE_LIBRARY_PATH -lConfigUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lDrawListUtils
    LIBS += -L$$IDE_LIBRARY_PATH -lHelper
    LIBS += -L$$IDE_LIBRARY_PATH -lCsv
    LIBS += -L$$IDE_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$LINK_LIBRARY_PATH -lLua
    LIBS += -L$$LINK_LIBRARY_PATH -lXmlUtils
}

UI_DIR=../../build-QStudioSCADA/ProjectManager/ui
MOC_DIR=../../build-QStudioSCADA/ProjectManager/moc
OBJECTS_DIR=../../build-QStudioSCADA/ProjectManager/obj
RCC_DIR=../../build-QStudioSCADA/ProjectManager/rcc

# debug: The project is being built in debug mode.
# release: The project is being built in release mode.
# win32: The project is being built in a Windows environment.
# macx: The project is being built in a Mac OS X environment.
# unix (including Linux): The project is being built in a Unix environment.

# win32 {
# 	LIBS += -lmywin32lib
# } else macx {
# 	LIBS += -lmymacxlib
# } else {
# 	LIBS += -lmyunixlib
# }

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
    DBVarGroup.cpp \
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
    ScriptRunConditionEditorDlg.cpp

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
    DBVarGroup.h \
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
    ScriptRunConditionEditorDlg.h

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
    ScriptRunConditionEditorDlg.ui

RESOURCES += \
    images.qrc \
    images1.qrc

# windows application icon
RC_FILE = ProjectManager.rc

#or
#RC_ICONS = project.ico
