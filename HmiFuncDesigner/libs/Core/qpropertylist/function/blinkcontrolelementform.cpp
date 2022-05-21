#include "blinkcontrolelementform.h"
#include "ui_blinkcontrolelementform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

BlinkControlElementForm::BlinkControlElementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlinkControlElementForm)
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

BlinkControlElementForm::~BlinkControlElementForm()
{
    delete ui;
}

void BlinkControlElementForm::updateFromUi()
{
    int index = ui->cboSelectElement->currentIndex();
    m_arg = ui->cboSelectElement->itemData(index).toString();
    m_showArg = ui->cboSelectElement->currentText();
}

QString BlinkControlElementForm::group()
{
    return tr("控件操作");
}

QString BlinkControlElementForm::name()
{
    return "BlinkControlElement";
}

QString BlinkControlElementForm::showName()
{
    return tr("闪烁控件");
}

QStringList BlinkControlElementForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "char *id";
    } else {
        arg << m_arg;
    }
    return arg;
}

QStringList BlinkControlElementForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("控件ID");
    } else {
        arg << m_showArg;
    }
    return arg;
}

QString BlinkControlElementForm::description()
{
    QString desc = tr("将指定控件元素变为闪烁显示");
    desc += "\r\n";
    desc += "示例: BlinkControlElement(\"main.label\")  // 将文本控件\"main.label\"闪烁显示";
    return desc;
}

QString BlinkControlElementForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

QString BlinkControlElementForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

bool BlinkControlElementForm::fromString(const QString func)
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
