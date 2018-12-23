#include "childform.h"
#include "ui_childform.h"

ChildForm::ChildForm(QWidget *parent, const QString & projName) :
    QWidget(parent),
    ui(new Ui::ChildForm),
    m_strProjectName(projName),
    m_bModifiedFlag(false)
{
    ui->setupUi(this);
    setMinimumSize(600,400);
    qRegisterMetaType<PAGE_FLOWTYPE>("PAGE_FLOWTYPE");

    //系统参数设置页面
    m_sysParamWinPtr = new SystemParametersWin(this);
    ui->stackedWidget->addWidget(m_sysParamWinPtr);
}

ChildForm::~ChildForm()
{
    if(m_sysParamWinPtr != nullptr) {
        delete m_sysParamWinPtr;
        m_sysParamWinPtr = nullptr;
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

    // 系统参数设置页面
    if (m_currPageFlow == PAGE_SYSTEM_PARAMETER) {
        m_sysParamWinPtr->setProjectName(m_strProjectName);
        ui->stackedWidget->setCurrentWidget(m_sysParamWinPtr);
    }
}


