#include "enablecontrolelementform.h"
#include "ui_enablecontrolelementform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

EnableControlElementForm::EnableControlElementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnableControlElementForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
    QStringList graphs;
    QSoftCore::getCore()->getProjectCore()->getAllElementIDName(graphs);
    foreach(QString graph, graphs) {
        QStringList idName = graph.split(":");//画面id.控件id:画面名称.控件名称
        if(idName.size() == 2) {
            ui->cboSelectElement->addItem(idName.at(1), idName.at(0));
        }
    }
}

EnableControlElementForm::~EnableControlElementForm()
{
    delete ui;
}

void EnableControlElementForm::updateFromUi()
{
    int index = ui->cboSelectElement->currentIndex();
    m_arg = ui->cboSelectElement->itemData(index).toString();
    m_showArg = ui->cboSelectElement->currentText();
}

QString EnableControlElementForm::group()
{
    return tr("控件操作");
}

QString EnableControlElementForm::name()
{
    return "EnableControlElement";
}

QString EnableControlElementForm::showName()
{
    return tr("生效控件");
}

QStringList EnableControlElementForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "char *id";
    } else {
        arg << m_arg;
    }
    return arg;
}

QStringList EnableControlElementForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("控件ID");
    } else {
        arg << m_showArg;
    }
    return arg;
}

QString EnableControlElementForm::description()
{
    QString desc = tr("将指定控件元素变为可用状态");
    desc += "\r\n";
    desc += "示例: EnableControlElement(\"main.label\")  // 将文本控件\"main.label\"变为可用状态";
    return desc;
}

QString EnableControlElementForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

QString EnableControlElementForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

bool EnableControlElementForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(\"", "");
        arg = arg.replace("\");", "");
        m_arg = arg;

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(\"", "");
        showArg = showArg.replace("\");", "");
        m_showArg = showArg;

        ui->cboSelectElement->setCurrentText(m_showArg);
        int index = ui->cboSelectElement->currentIndex();
        ui->cboSelectElement->setItemData(index, m_arg);

        return true;
    }
    return false;
}
