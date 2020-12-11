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
INCLUDEPATH += $$PWD/GraphPage
INCLUDEPATH += $$PWD/../../common/qtsingleapplication
#INCLUDEPATH += $$PWD/../../common/register
INCLUDEPATH += $$PWD/../../libs/qscintilla
INCLUDEPATH += $$PWD/../../libs/ProjData
INCLUDEPATH += $$PWD/../../libs/Core
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

LIBS += -L$$LINK_LIBRARY_PATH -lCsv$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lUtils$${LIB_SUFFIX}
LIBS += -L$$LINK_LIBRARY_PATH -lProjData
LIBS += -L$$LINK_LIBRARY_PATH -lCore

SOURCES += main.cpp\
    ../Public/QAbstractButtonHost.cpp \
    ../Public/QAbstractHost.cpp \
    ../Public/QAbstractWidgetHost.cpp \
    ../Public/QWidgetHost.cpp \
    ElementTreeWidget.cpp \
    GraphPage/widgethost.cpp \
    CommunicationDeviceChild.cpp \
    DevicePluginLoader.cpp \
    GraphPage/GraphPageListWidget.cpp \
    GraphPage/formresizer.cpp \
    GraphPage/qdesignerformhost.cpp \
    GraphPage/qdesignermimedata.cpp \
    GraphPage/qformlistwidget.cpp \
    GraphPage/qformwidgetview.cpp \
    GraphPage/qhostfactory.cpp \
    GraphPage/qpushbuttonhost.cpp \
    GraphPage/qselectwidget.cpp \
    GraphPage/sizehandlerect.cpp \
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
    TagManagerChild.cpp \
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
    VerifyPasswordDialog.cpp \
    widget/QFramelessDockWidget.cpp \
    widget/QIPAddressEdit.cpp \
    widget/QQuickInputLineEdit.cpp \
    GetWidthHeightDialog.cpp \
    GraphPage/GraphPage.cpp \
    GraphPage/GraphPageManager.cpp \
    UndoCommand.cpp \
    ElementLibraryWidget.cpp \
    ElementLibraryListWidget.cpp \
    ElementLibraryListWidgetItem.cpp \
    GraphPage/GraphPageTreeView.cpp \
    ../Public/PubTool.cpp \
    ../Public/Element.cpp \
    ../Public/ElementGroup.cpp \
    PluginManager.cpp

HEADERS  += MainWindow.h \
    ../Public/QAbstractButtonHost.h \
    ../Public/QAbstractHost.h \
    ../Public/QAbstractWidgetHost.h \
    ../Public/QWidgetHost.h \
    ElementTreeWidget.h \
    GraphPage/widgethost.h \
    ChildInterface.h \
    CommunicationDeviceChild.h \
    DevicePluginLoader.h \
    GraphPage/GraphPageListWidget.h \
    GraphPage/formresizer.h \
    GraphPage/qdesignerformhost.h \
    GraphPage/qdesignermimedata.h \
    GraphPage/qformlistwidget.h \
    GraphPage/qformwidgetview.h \
    GraphPage/qhostfactory.h \
    GraphPage/qpushbuttonhost.h \
    GraphPage/qselectwidget.h \
    GraphPage/sizehandlerect.h \
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
    TagManagerChild.h \
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
    VerifyPasswordDialog.h \
    widget/QFramelessDockWidget.h \
    widget/QIPAddressEdit.h \
    widget/QQuickInputLineEdit.h \
    GetWidthHeightDialog.h \
    GraphPage/GraphPage.h \
    GraphPage/GraphPageManager.h \
    UndoCommand.h \
    ElementLibraryWidget.h \
    ElementLibraryListWidget.h \
    ElementLibraryListWidgetItem.h \
    GraphPage/GraphPageTreeView.h \
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

TRANSLATIONS += chinese.ts

