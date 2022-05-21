#include "switchgraphpageform.h"
#include "ui_switchgraphpageform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

SwitchGraphPageForm::SwitchGraphPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchGraphPageForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
    QStringList graphs;
    QSoftCore::getCore()->getProjectCore()->getAllGraphPageName(graphs);
    foreach(QString graph, graphs) {
        QStringList idName = graph.split(":");//id:画面名称
        if(idName.size() == 2) {
            ui->cboSelectPage->addItem(idName.at(1), idName.at(0));
        }
    }
}

SwitchGraphPageForm::~SwitchGraphPageForm()
{
    delete ui;
}

void SwitchGraphPageForm::updateFromUi()
{
    int index = ui->cboSelectPage->currentIndex();
    m_arg = ui->cboSelectPage->itemData(index).toString();
    m_showArg = ui->cboSelectPage->currentText();
}

void SwitchGraphPageForm::on_cboSelectPage_currentTextChanged(const QString &page)
{
    Q_UNUSED(page)
    updateFromUi();
}

QString SwitchGraphPageForm::group()
{
    return tr("系统操作");
}

QString SwitchGraphPageForm::name()
{
    return "SwitchGraphPage";
}

QString SwitchGraphPageForm::showName()
{
    return tr("切换画面");
}

QStringList SwitchGraphPageForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "char *pageID";
    } else {
        arg << m_arg;
    }
    return arg;
}

QStringList SwitchGraphPageForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("画面ID");
    } else {
        arg << m_showArg;
    }
    return arg;
}

QString SwitchGraphPageForm::description()
{
    QString desc = tr("切换至指定ID的画面");
    desc += "\r\n";
    desc += "示例:切换画面(\"参数设置\")  // 切换至参数设置画面";
    return desc;
}

QString SwitchGraphPageForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

QString SwitchGraphPageForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

bool SwitchGraphPageForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(\"", ""); //移除"SwitchGraphPage(""
        arg = arg.replace("\");", ""); //移除"");"
        m_arg = arg;

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(\"", ""); //移除"切换画面(""
        showArg = showArg.replace("\");", ""); //移除"")"
        m_showArg = arg;

        ui->cboSelectPage->setCurrentText(m_showArg);
        int index = ui->cboSelectPage->currentIndex();
        ui->cboSelectPage->setItemData(index, m_arg);

        return true;
    }
    return false;
}
