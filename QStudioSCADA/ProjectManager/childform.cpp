#include "childform.h"
#include "ui_childform.h"
#include <QDebug>

ChildForm::ChildForm(QWidget *parent, const QString & projName) :
    QWidget(parent),
    ui(new Ui::ChildForm),
    m_strProjectName(projName),
    m_bModifiedFlag(false)
{
    ui->setupUi(this);
    setMinimumSize(600,400);
    qRegisterMetaType<PAGE_FLOWTYPE>("PAGE_FLOWTYPE");

    // 系统参数设置页面
    m_sysParamWinPtr = new SystemParametersWin(this);
    ui->stackedWidget->addWidget(m_sysParamWinPtr);

    // 通讯设备页面
    m_communicationDeviceWinPtr = new CommunicationDeviceWin(this);
    ui->stackedWidget->addWidget(m_communicationDeviceWinPtr);
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

/**
 * @brief MainWindow::switchPage 切换页面
 * @param page
 */
void ChildForm::switchPage(PAGE_FLOWTYPE page)
{
    m_currPageFlow = page;

    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->setProjectName(m_strProjectName);
        ui->stackedWidget->setCurrentWidget(m_sysParamWinPtr);
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->setProjectName(m_strProjectName);
        ui->stackedWidget->setCurrentWidget(m_communicationDeviceWinPtr);
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
    }


}

/*
 * 保存文件
 */
void ChildForm::save()
{
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->save();
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->save();
    }


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
    }

}

void ChildForm::treeItemClicked(const QString &itemText)
{
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) { // 系统参数设置页面
        m_sysParamWinPtr->setItemName(itemText);
    } else if(m_currPageFlow == PAGE_COMMUNICATE_DEVICE) { // 通讯设备页面
        m_communicationDeviceWinPtr->setItemName(itemText);
        m_communicationDeviceWinPtr->ListViewUpdate(itemText);
    }
}

