#include "childform.h"
#include "ui_childform.h"
#include <QDebug>

ChildForm::ChildForm(QWidget *parent, const QString & projName) :
    QWidget(parent),
    ui(new Ui::ChildForm),
    m_bModifiedFlag(false),
    m_strProjectName(projName)
{
    ui->setupUi(this);
    setMinimumSize(600,400);
    qRegisterMetaType<PAGE_FLOWTYPE>("PAGE_FLOWTYPE");

    // 系统参数设置页面
    m_sysParamWinPtr = new SystemParametersWin(this);
    m_sysParamWinPtr->setProjectName(m_strProjectName);
    ui->stackedWidget->addWidget(m_sysParamWinPtr);

    // 通讯设备页面
    m_communicationDeviceWinPtr = new CommunicationDeviceWin(this);
    m_communicationDeviceWinPtr->setProjectName(m_strProjectName);
    ui->stackedWidget->addWidget(m_communicationDeviceWinPtr);

    // 变量管理
    m_variableManagerWinPtr = new VariableManagerWin(this);
    m_variableManagerWinPtr->setProjectName(m_strProjectName);
    ui->stackedWidget->addWidget(m_variableManagerWinPtr);

    // 画面管理
    m_drawPageWinPtr = new DrawPageWin(this);
    m_drawPageWinPtr->setProjectName(m_strProjectName);
    ui->stackedWidget->addWidget(m_drawPageWinPtr);

    // 实时数据库
    m_rtdbWinPtr = new RealTimeDatabaseWin(this);
    m_rtdbWinPtr->setProjectName(m_strProjectName);
    ui->stackedWidget->addWidget(m_rtdbWinPtr);

}

ChildForm::~ChildForm()
{
    if(m_sysParamWinPtr != nullptr) {
        delete m_sysParamWinPtr;
        m_sysParamWinPtr = nullptr;
    }

    if(m_communicationDeviceWinPtr != nullptr) {
        delete m_communicationDeviceWinPtr;
        m_communicationDeviceWinPtr = nullptr;
    }

    if(m_variableManagerWinPtr != nullptr) {
        delete m_variableManagerWinPtr;
        m_variableManagerWinPtr = nullptr;
    }

    if(m_drawPageWinPtr != nullptr) {
        delete m_drawPageWinPtr;
        m_drawPageWinPtr = nullptr;
    }

    if(m_rtdbWinPtr != nullptr) {
        delete m_rtdbWinPtr;
        m_rtdbWinPtr = nullptr;
    }

    delete ui;
}

/**
 * @brief ChildForm::getProjectName 获取工程文件名称
 * @return 工程文件名称
 */
QString ChildForm::getProjectName() const
{
    return m_strProjectName;
}

/**
 * @brief ChildForm::setProjectName 设置工程文件名称
 * @param s 工程文件名称
 */
void ChildForm::setProjectName(const QString & s)
{
    m_strProjectName = s;
}

/**
 * @brief ChildForm::setModifiedFlag 设置工程文件修改标志
 * @param b 工程文件修改标志，true-已修改，false-未修改
 */
void ChildForm::setModifiedFlag(bool b)
{
    this->m_bModifiedFlag = b;
}

/**
 * @brief ChildForm::getModifiedFlag 获取文件修改标志
 * @return 工程文件修改标志
 */
bool ChildForm::getModifiedFlag()
{
    return this->m_bModifiedFlag;
}

void ChildForm::SetTitle(const QString &title)
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        m_variableManagerWinPtr->SetTitle(title);
    }
    this->setWindowTitle(title);
}

/**
 * @brief ChildForm::addVariableTag 增加变量标签
 */
void ChildForm::addVariableTag()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->VariableAdd();
    }
}

/**
 * @brief ChildForm::appendVariableTag 追加变量标签
 */
void ChildForm::appendVariableTag()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->VariableAppend();
    }
}


/**
 * @brief ChildForm::rowCopyVariableTag 行拷贝变量标签
 */
void ChildForm::rowCopyVariableTag()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->VariableRowCopy();
    }
}


/**
 * @brief ChildForm::columnCopyVariableTag 列拷贝变量标签
 */
void ChildForm::columnCopyVariableTag()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->VariableColCopy();
    }
}


/**
 * @brief ChildForm::modifyVariableTag 修改变量标签
 */
void ChildForm::modifyVariableTag()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->VariableModify();
    }
}

/**
 * @brief ChildForm::deleteVariableTag 删除变量标签
 */
void ChildForm::deleteVariableTag()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->VariableDelete();
    }
}

/**
 * @brief ChildForm::variableTagExportToCsv 变量标签导出csv
 * @param path 导出路径
 * @param item 标量标签类型
 */
void ChildForm::variableTagExportToCsv(const QString &path,  const QString &item)
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->exportToCsv(path, item);
    }
}


/**
 * @brief ChildForm::variableTagImportFromCsv 从csv导入变量标签
 * @param file csv文件
 */
void ChildForm::variableTagImportFromCsv(const QString &file)
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) {
        m_variableManagerWinPtr->importFromCsv(file);
    }
}

/**
 * @brief MainWindow::switchPage 切换页面
 * @param page
 */
void ChildForm::switchPage(PAGE_FLOWTYPE page)
{
    m_currPageFlow = page;

    if(m_currPageFlow == PAGE_NONE) {
        ui->stackedWidget->setCurrentWidget(NULL);
    } else if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        ui->stackedWidget->setCurrentWidget(m_sysParamWinPtr);
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        ui->stackedWidget->setCurrentWidget(m_communicationDeviceWinPtr);
    } else if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        ui->stackedWidget->setCurrentWidget(m_variableManagerWinPtr);
    } else if(m_currPageFlow == PAGE_DRAW_PAGE) { // 画面管理
        ui->stackedWidget->setCurrentWidget(m_drawPageWinPtr);
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        ui->stackedWidget->setCurrentWidget(m_rtdbWinPtr);
    }


}


/*
 * 打开文件
 */
void ChildForm::open()
{
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->open();
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->open();
    } else if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        m_variableManagerWinPtr->open();
    } else if(m_currPageFlow == PAGE_DRAW_PAGE) { // 画面管理
        m_drawPageWinPtr->open();
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        m_rtdbWinPtr->open();
    }


}

/*
 * 保存文件
 */
void ChildForm::save()
{
    m_sysParamWinPtr->save();
    m_communicationDeviceWinPtr->save();
    m_variableManagerWinPtr->save();
    m_drawPageWinPtr->save();
    m_rtdbWinPtr->save();


}

/*
* 显示大图标
*/
void ChildForm::ShowLargeIcon()
{
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->ShowLargeIcon();
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->ShowLargeIcon();
    } else if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        m_variableManagerWinPtr->ShowLargeIcon();
    } else if(m_currPageFlow == PAGE_DRAW_PAGE) { // 画面管理
        m_drawPageWinPtr->ShowLargeIcon();
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        m_rtdbWinPtr->ShowLargeIcon();
    }

}

/*
* 显示小图标
*/
void ChildForm::ShowSmallIcon()
{
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->ShowSmallIcon();
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->ShowSmallIcon();
    } else if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        m_variableManagerWinPtr->ShowSmallIcon();
    } else if(m_currPageFlow == PAGE_DRAW_PAGE) { // 画面管理
        m_drawPageWinPtr->ShowSmallIcon();
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        m_rtdbWinPtr->ShowSmallIcon();
    }

}

void ChildForm::treeItemClicked(const QString &itemText)
{
    //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << itemText;
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->setItemName(itemText);
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->setItemName(itemText);
        m_communicationDeviceWinPtr->ListViewUpdate(itemText);
    } else if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        m_variableManagerWinPtr->setItemName(itemText);
        m_variableManagerWinPtr->init(itemText);
    } else if(m_currPageFlow == PAGE_DRAW_PAGE) { // 画面管理
        m_drawPageWinPtr->setItemName(itemText);
        m_drawPageWinPtr->init();
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        m_rtdbWinPtr->setItemName(itemText);
        m_rtdbWinPtr->init();
    }
}

