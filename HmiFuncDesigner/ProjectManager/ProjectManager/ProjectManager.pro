
include(../../HmiFuncDesigner.pri)

QT += core gui xml
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += script
QT += sql

TARGET = project
TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH

INCLUDEPATH += .
INCLUDEPATH += ./widget
INCLUDEPATH += $$PWD/../../common/qtsingleapplication
#INCLUDEPATH += $$PWD/../../common/register
INCLUDEPATH += $$PWD/../../libs/qscintilla

include($$PWD/../../common/qtsingleapplication/qtsingleapplication.pri)
#include($$PWD/../../common/register/register.pri)
include($$PWD/../DrawApplication/DrawApplication.pri)

#DEFINES += QT_NO_DEBUG_OUTPUT


CONFIG(debug, debug|release) { #debug
    win32 {
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lCsvd
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}d
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowserd
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManagerd
        LIBS += -L$$LINK_LIBRARY_PATH -lUtilsd
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtilsd
    }
    unix {
        LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lCsv
        LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
        LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
        LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
        LIBS += -L$$LINK_LIBRARY_PATH -lUtils
        LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
    }
}
else { # release
    LIBS += -L$$LINK_LIBRARY_PATH -lDrawUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lCsv
    LIBS += -L$$LINK_LIBRARY_PATH -lqscintilla2_qt$${QT_MAJOR_VERSION}
    LIBS += -L$$LINK_LIBRARY_PATH -lQtPropertyBrowser
    LIBS += -L$$LINK_LIBRARY_PATH -lTagManager
    LIBS += -L$$LINK_LIBRARY_PATH -lUtils
    LIBS += -L$$LINK_LIBRARY_PATH -lProjectDataUtils
}

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
    SplitterForm.cpp \
    InsertFunctionDialog.cpp \
    InsertTagDialog.cpp \
    ScriptRunConditionEditorDlg.cpp \
    ChildForm.cpp \
    ProjectMgrUtils.cpp \
    widget/QFramelessDockWidget.cpp \
    widget/QIPAddressEdit.cpp \
    widget/QQuickInputLineEdit.cpp

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
    SplitterForm.h \
    InsertFunctionDialog.h \
    InsertTagDialog.h \
    ScriptRunConditionEditorDlg.h \
    FileTansfer.h \
    ChildForm.h \
    ProjectMgrUtils.h \
    widget/QFramelessDockWidget.h \
    widget/QIPAddressEdit.h \
    widget/QQuickInputLineEdit.h

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
    ChildForm.ui \
    widget/QIPAddressEdit.ui

RESOURCES += \
    images.qrc \
    images1.qrc

# windows application icon
RC_FILE = ProjectManager.rc

#or
#RC_ICONS = project.ico
