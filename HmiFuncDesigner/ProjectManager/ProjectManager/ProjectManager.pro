
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
INCLUDEPATH += $$PWD/widget
INCLUDEPATH += $$PWD/ProjData
INCLUDEPATH += $$PWD/../../common/qtsingleapplication
#INCLUDEPATH += $$PWD/../../common/register
INCLUDEPATH += $$PWD/../../libs/qscintilla
INCLUDEPATH += $$PWD/../QtPropertyBrowserEx

include($$PWD/../../common/qtsingleapplication/qtsingleapplication.pri)
#include($$PWD/../../common/register/register.pri)
include($$PWD/../QtPropertyBrowserEx/QtPropertyBrowserEx.pri)
include($$PWD/ProjData/ProjData.pri)


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

SOURCES += main.cpp\
    CommunicationDeviceChild.cpp \
    DevicePluginLoader.cpp \
    GraphPageListWidget.cpp \
    ListViewEx.cpp \
    MainWindow.cpp \
    MdiArea.cpp \
    NewProjectDialog.cpp \
    ProjectTreeView.cpp \
    NetSettingDialog.cpp \
    DatabaseSettingDialog.cpp \
    RealTimeDatabaseChild.cpp \
    ScriptManageChild.cpp \
    SystemParametersChild.cpp \
    TableviewDelegate.cpp \
    TagIOEditDialog.cpp \
    TagManagerChild.cpp \
    TagTmpEditDialog.cpp \
    UserAuthorityDialog.cpp \
    NewComDeviceDialog.cpp \
    NewVariableGroupDialog.cpp \
    DeviceListDialog.cpp \
    SelectProtocolDialog.cpp \
    ProjectDownloadDialog.cpp \
    ProjectUploadDialog.cpp \
    AboutDialog.cpp \
    NewNetDeviceDialog.cpp \
    ScriptConditionConfigForm.cpp \
    ScriptEditorDlg.cpp \
    TagFuncEditDialog.cpp \
    SplitterForm.cpp \
    InsertFunctionDialog.cpp \
    InsertTagDialog.cpp \
    ScriptRunConditionEditorDlg.cpp \
    widget/QFramelessDockWidget.cpp \
    widget/QIPAddressEdit.cpp \
    widget/QQuickInputLineEdit.cpp \
#    DrawMainWindow.cpp \
    GetWidthHeightDialog.cpp \
    GraphPage.cpp \
    GraphPageManager.cpp \
    UndoCommand.cpp \
    ElementLibraryWidget.cpp \
    ElementLibraryListWidget.cpp \
    ElementLibraryListWidgetItem.cpp \
    ElementSimpleListWidget.cpp \
    GraphPageTreeView.cpp \
    ../Public/PubTool.cpp \
    ../Public/Element.cpp \
    ../Public/ElementGroup.cpp \
    PluginManager.cpp

HEADERS  += MainWindow.h \
    ChildInterface.h \
    CommunicationDeviceChild.h \
    DevicePluginLoader.h \
    GraphPageListWidget.h \
    ListViewEx.h \
    MdiArea.h \
    NewProjectDialog.h \
    ../Public/Public.h \
    ProjectTreeView.h \
    NetSettingDialog.h \
    DatabaseSettingDialog.h \
    RealTimeDatabaseChild.h \
    ScriptManageChild.h \
    SystemParametersChild.h \
    TableviewDelegate.h \
    TagIOEditDialog.h \
    TagManagerChild.h \
    TagTmpEditDialog.h \
    UserAuthorityDialog.h \
    NewComDeviceDialog.h \
    NewVariableGroupDialog.h \
    DeviceListDialog.h \
    SelectProtocolDialog.h \
    ProjectDownloadDialog.h \
    ProjectUploadDialog.h \
    AboutDialog.h \
    NewNetDeviceDialog.h \
    ScriptConditionConfigForm.h \
    ScriptEditorDlg.h \
    TagFuncEditDialog.h \
    SplitterForm.h \
    InsertFunctionDialog.h \
    InsertTagDialog.h \
    ScriptRunConditionEditorDlg.h \
    FileTansfer.h \
    widget/QFramelessDockWidget.h \
    widget/QIPAddressEdit.h \
    widget/QQuickInputLineEdit.h \
#    DrawMainWindow.h \
    GetWidthHeightDialog.h \
    GraphPage.h \
    GraphPageManager.h \
    UndoCommand.h \
    ElementLibraryWidget.h \
    ElementLibraryListWidget.h \
    ElementLibraryListWidgetItem.h \
    ElementSimpleListWidget.h \
    GraphPageTreeView.h \
    ../Public/PublicDefine.h \
    ../Public/PubTool.h \
    ../IDrawApplicationPlugin/IDrawApplicationPlugin.h \
    ../Public/Element.h \
    ../Public/ElementGroup.h \
    PluginManager.h


FORMS    += \
    NewProjectDialog.ui \
    NetSettingDialog.ui \
    DatabaseSettingDialog.ui \
    TagIOEditDialog.ui \
    TagTmpEditDialog.ui \
    UserAuthorityDialog.ui \
    NewComDeviceDialog.ui \
    NewVariableGroupDialog.ui \
    DeviceListDialog.ui \
    SelectProtocolDialog.ui \
    ProjectDownloadDialog.ui \
    ProjectUploadDialog.ui \
    AboutDialog.ui \
    NewNetDeviceDialog.ui \
    ScriptConditionConfigForm.ui \
    ScriptEditorDlg.ui \
    InsertFunctionDialog.ui \
    InsertTagDialog.ui \
    ScriptRunConditionEditorDlg.ui \
    widget/QIPAddressEdit.ui \
#    DrawMainWindow.ui \
    ElementLibraryWidget.ui \
    GetWidthHeightDialog.ui

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
