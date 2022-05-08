#include "sleepform.h"
#include "ui_sleepform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

SleepForm::SleepForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SleepForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
    ui->spinBoxSecond->setRange(1, 65536);
    ui->spinBoxSecond->setValue(1);
}

SleepForm::~SleepForm()
{
    delete ui;
}

void SleepForm::updateFromUi()
{
    m_arg = QString::number(ui->spinBoxSecond->value());
    m_showArg = QString::number(ui->spinBoxSecond->value());
}

void SleepForm::on_cboSelectPage_currentTextChanged(const QString &page)
{
    Q_UNUSED(page)
    updateFromUi();
}

QString SleepForm::group()
{
    return tr("系统操作");
}

QString SleepForm::name()
{
    return "Sleep";
}

QString SleepForm::showName()
{
    return tr("控制延时");
}

QStringList SleepForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "int nSec";
    } else {
        arg << m_arg;
    }
    return arg;
}

QStringList SleepForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("秒");
    } else {
        arg << m_showArg;
    }
    return arg;
}

QString SleepForm::description()
{
    QString desc = tr("控制延时，系统休眠[1,65536]秒");
    desc += "\r\n";
    desc += "示例:控制延时(1)  // 延时1秒";

    return desc;
}

QString SleepForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(" + args().join("") + ");";
    func += ":";
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

QString SleepForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

bool SleepForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(", ""); //移除"Sleep("
        arg = arg.replace(");", ""); //移除");"
        m_arg = arg;

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(", ""); //移除"Sleep("
        showArg = showArg.replace(");", ""); //移除")"
        m_showArg = arg;

        ui->spinBoxSecond->setValue(m_arg.toUInt());

        return true;
    }
    return false;
}
