
include(../../HmiFuncDesigner.pri)

QT += core gui xml
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += script
QT += sql

TARGET = ProjectManager
TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH

INCLUDEPATH += .
INCLUDEPATH += ./widget
INCLUDEPATH += $$PWD/../../common/qtsingleapplication
#INCLUDEPATH += $$PWD/../../common/register
INCLUDEPATH += $$PWD/../../libs/qscintilla
INCLUDEPATH += $$PWD/../QtPropertyBrowserEx

include($$PWD/../../common/qtsingleapplication/qtsingleapplication.pri)
#include($$PWD/../../common/register/register.pri)
include($$PWD/../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)

#DEFINES += QT_NO_DEBUG_OUTPUT

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lCsv$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils$${LIB_SUFFIX}

SOURCES += main.cpp\
    DevicePluginLoader.cpp \
    MainWindow.cpp \
    NewProjectDialog.cpp \
    SystemParametersWin.cpp \
    NetSettingDialog.cpp \
    DatabaseSettingDialog.cpp \
    TableviewDelegate.cpp \
    TagIOEditDialog.cpp \
    TagManagerWin.cpp \
    TagTmpEditDialog.cpp \
    UserAuthorityDialog.cpp \
    CommunicationDeviceWin.cpp \
    NewComDeviceDialog.cpp \
    ChildBase.cpp \
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
    SplitterForm.cpp \
    InsertFunctionDialog.cpp \
    InsertTagDialog.cpp \
    ScriptRunConditionEditorDlg.cpp \
    ChildForm.cpp \
    ProjectMgrUtils.cpp \
    widget/QFramelessDockWidget.cpp \
    widget/QIPAddressEdit.cpp \
    widget/QQuickInputLineEdit.cpp \
#    $$PWD/DrawMainWindow.cpp \
    $$PWD/GetWidthHeightDialog.cpp \
    $$PWD/GraphPage.cpp \
    $$PWD/GraphPageManager.cpp \
    $$PWD/UndoCommand.cpp \
    $$PWD/ElementLibraryWidget.cpp \
    $$PWD/ObjectsTreeView.cpp \
    $$PWD/ElementLibraryListWidget.cpp \
    $$PWD/ElementLibraryListWidgetItem.cpp \
    $$PWD/ElementSimpleListWidget.cpp \
    $$PWD/GraphPageTreeView.cpp \
    $$PWD/../Public/PubTool.cpp \
    $$PWD/../Public/Element.cpp \
    $$PWD/../Public/ElementGroup.cpp \
    $$PWD/PluginManager.cpp

HEADERS  += MainWindow.h \
    DevicePluginLoader.h \
    NewProjectDialog.h \
    ../Public/Public.h \
    SystemParametersWin.h \
    NetSettingDialog.h \
    DatabaseSettingDialog.h \
    TableviewDelegate.h \
    TagIOEditDialog.h \
    TagManagerWin.h \
    TagTmpEditDialog.h \
    UserAuthorityDialog.h \
    CommunicationDeviceWin.h \
    NewComDeviceDialog.h \
    ChildBase.h \
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
    SplitterForm.h \
    InsertFunctionDialog.h \
    InsertTagDialog.h \
    ScriptRunConditionEditorDlg.h \
    FileTansfer.h \
    ChildForm.h \
    ProjectMgrUtils.h \
    widget/QFramelessDockWidget.h \
    widget/QIPAddressEdit.h \
    widget/QQuickInputLineEdit.h \
#    $$PWD/DrawMainWindow.h \
    $$PWD/GetWidthHeightDialog.h \
    $$PWD/GraphPage.h \
    $$PWD/GraphPageManager.h \
    $$PWD/UndoCommand.h \
    $$PWD/ElementLibraryWidget.h \
    $$PWD/ObjectsTreeView.h \
    $$PWD/ElementLibraryListWidget.h \
    $$PWD/ElementLibraryListWidgetItem.h \
    $$PWD/ElementSimpleListWidget.h \
    $$PWD/GraphPageTreeView.h \
    $$PWD/../Public/PublicDefine.h \
    $$PWD/../Public/PubTool.h \
    $$PWD/../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    $$PWD/../Public/Element.h \
    $$PWD/../Public/ElementGroup.h \
    $$PWD/PluginManager.h


FORMS    += MainWindow.ui \
    NewProjectDialog.ui \
    SystemParametersWin.ui \
    NetSettingDialog.ui \
    DatabaseSettingDialog.ui \
    TagIOEditDialog.ui \
    TagManagerWin.ui \
    TagTmpEditDialog.ui \
    UserAuthorityDialog.ui \
    CommunicationDeviceWin.ui \
    NewComDeviceDialog.ui \
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
    ChildForm.ui \
    widget/QIPAddressEdit.ui \
#    $$PWD/DrawMainWindow.ui \
    $$PWD/ElementLibraryWidget.ui \
    $$PWD/GetWidthHeightDialog.ui

RESOURCES += \
    images.qrc \
    images1.qrc \
    DrawAppImages.qrc

# windows application icon
RC_FILE = ProjectManager.rc

#or
#RC_ICONS = project.ico


######################################################

DISTFILES += \
    $$IDE_BIN_PATH/Config/DrawAppJScriptFun.xml \
    $$IDE_BIN_PATH/Config/ElementSupportEvents.xml
