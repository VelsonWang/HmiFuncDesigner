#include "ChildForm.h"
#include "ui_ChildForm.h"


ChildForm::ChildForm(QWidget *parent, const QString & projName) :
    QWidget(parent),
    ui(new Ui::ChildForm),
    m_bModifiedFlag(false),
    m_strProjectName(projName)
{
    ui->setupUi(this);
    setMinimumSize(600, 400);
    qRegisterMetaType<PAGE_FLOWTYPE>("PAGE_FLOWTYPE");

    // 实时数据库
    m_rtdbWinPtr = new RealTimeDatabaseWin(this);
    m_rtdbWinPtr->setProjectName(m_strProjectName);
    ui->stackedWidget->addWidget(m_rtdbWinPtr);

    // 脚本编辑器
    m_scriptManageWinPtr = new ScriptManageWin(this);
    m_scriptManageWinPtr->setProjectName(m_strProjectName);
    m_scriptManageWinPtr->open();
    ui->stackedWidget->addWidget(m_scriptManageWinPtr);
}

ChildForm::~ChildForm()
{
    if(m_rtdbWinPtr != Q_NULLPTR) {
        delete m_rtdbWinPtr;
        m_rtdbWinPtr = Q_NULLPTR;
    }

    if(m_scriptManageWinPtr != Q_NULLPTR) {
        delete m_scriptManageWinPtr;
        m_scriptManageWinPtr = Q_NULLPTR;
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
    this->setWindowTitle(title);
}

QString ChildForm::getItemName() const
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        //return m_tagManagerWinPtr->getItemName();
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        return m_rtdbWinPtr->getItemName();
    } else if(m_currPageFlow == PAGE_RTDB) { // 脚本编辑器
        return m_scriptManageWinPtr->getItemName();
    }
    return QString();
}


/**
 * @brief MainWindow::switchPage 切换页面
 * @param page
 */
void ChildForm::switchPage(PAGE_FLOWTYPE page)
{
    m_currPageFlow = page;

    if(m_currPageFlow == PAGE_NONE) {
        ui->stackedWidget->setCurrentWidget(Q_NULLPTR);
    } else if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        //ui->stackedWidget->setCurrentWidget(m_tagManagerWinPtr);
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        ui->stackedWidget->setCurrentWidget(m_rtdbWinPtr);
    } else if(m_currPageFlow == PAGE_SCRIPT_MANAGER) { // 脚本编辑器
        ui->stackedWidget->setCurrentWidget(m_scriptManageWinPtr);
    }
}


/*
 * 打开文件
 */
void ChildForm::open()
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        //m_tagManagerWinPtr->open();
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        m_rtdbWinPtr->open();
    } else if(m_currPageFlow == PAGE_RTDB) { // 脚本编辑器
        m_scriptManageWinPtr->open();
    }
}

/*
 * 保存文件
 */
void ChildForm::save()
{
    //m_tagManagerWinPtr->save();
    m_rtdbWinPtr->save();
    m_scriptManageWinPtr->save();
}


void ChildForm::treeItemClicked(const QString &itemText)
{
    if(m_currPageFlow == PAGE_VARIABLE_MANAGER) { // 变量管理
        //m_tagManagerWinPtr->setItemName(itemText);
        //m_tagManagerWinPtr->init(itemText);
    } else if(m_currPageFlow == PAGE_RTDB) { // 实时数据库
        m_rtdbWinPtr->setItemName(itemText);
        m_rtdbWinPtr->init();
    } else if(m_currPageFlow == PAGE_RTDB) { // 脚本编辑器
        m_scriptManageWinPtr->setItemName(itemText);
    }
}

