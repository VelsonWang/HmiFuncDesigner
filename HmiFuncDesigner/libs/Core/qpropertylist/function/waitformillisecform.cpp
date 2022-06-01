#include "waitformillisecform.h"
#include "ui_waitformillisecform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

WaitForMillisecForm::WaitForMillisecForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitForMillisecForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
    ui->spinBoxMilliSecond->setRange(1, 65536);
    ui->spinBoxMilliSecond->setValue(1);
}

WaitForMillisecForm::~WaitForMillisecForm()
{
    delete ui;
}

void WaitForMillisecForm::updateFromUi()
{
    m_arg = QString::number(ui->spinBoxMilliSecond->value());
    m_showArg = QString::number(ui->spinBoxMilliSecond->value());
}

void WaitForMillisecForm::on_cboSelectPage_currentTextChanged(const QString &page)
{
    Q_UNUSED(page)
    updateFromUi();
}

QString WaitForMillisecForm::group()
{
    return tr("系统操作");
}

QString WaitForMillisecForm::name()
{
    return "WaitForMillisec";
}

QString WaitForMillisecForm::showName()
{
    return tr("延时");
}

QStringList WaitForMillisecForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "int nSec";
    } else {
        arg << m_arg;
    }
    return arg;
}

QStringList WaitForMillisecForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("秒");
    } else {
        arg << m_showArg;
    }
    return arg;
}

QString WaitForMillisecForm::description()
{
    QString desc = tr("延时[1,65536]毫秒");
    desc += "\r\n";
    desc += "示例:延时(1)  // 延时1毫秒";
    return desc;
}

QString WaitForMillisecForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(" + args().join("") + ");";
    func += ":";
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

QString WaitForMillisecForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

bool WaitForMillisecForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(", "");
        arg = arg.replace(");", "");
        m_arg = arg;

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(", "");
        showArg = showArg.replace(");", "");
        m_showArg = arg;

        ui->spinBoxMilliSecond->setValue(m_arg.toUInt());

        return true;
    }
    return false;
}
