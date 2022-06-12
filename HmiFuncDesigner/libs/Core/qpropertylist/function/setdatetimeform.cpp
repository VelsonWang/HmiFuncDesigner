#include "setdatetimeform.h"
#include "ui_setdatetimeform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

SetDateTimeForm::SetDateTimeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDateTimeForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_args.clear();
    m_showArgs.clear();
    ui->spinBoxYear->setRange(1970, 9999);
    ui->spinBoxYear->setValue(1970);
    ui->spinBoxMonth->setRange(1, 12);
    ui->spinBoxMonth->setValue(1);
    ui->spinBoxDay->setRange(1, 31);
    ui->spinBoxDay->setValue(1);
    ui->spinBoxHour->setRange(0, 23);
    ui->spinBoxHour->setValue(0);
    ui->spinBoxMinute->setRange(0, 59);
    ui->spinBoxMinute->setValue(0);
    ui->spinBoxSecond->setRange(0, 59);
    ui->spinBoxSecond->setValue(0);
}

SetDateTimeForm::~SetDateTimeForm()
{
    delete ui;
}

void SetDateTimeForm::updateFromUi()
{
    m_args.clear();
    m_args << QString::number(ui->spinBoxYear->value())
           << QString::number(ui->spinBoxMonth->value())
           << QString::number(ui->spinBoxDay->value())
           << QString::number(ui->spinBoxHour->value())
           << QString::number(ui->spinBoxMinute->value())
           << QString::number(ui->spinBoxSecond->value());
    m_showArgs = m_args;
}

QString SetDateTimeForm::group()
{
    return tr("系统操作");
}

QString SetDateTimeForm::name()
{
    return "SetDateTime";
}

QString SetDateTimeForm::showName()
{
    return tr("设置系统时间");
}

QStringList SetDateTimeForm::args()
{
    QStringList arg;
    if(m_args.count() == 0) {
        arg << "int nYear";
        arg << "int nMonth";
        arg << "int nDay";
        arg << "int nHour";
        arg << "int nMinute";
        arg << "int nSecond";
    } else {
        updateFromUi();
        arg << m_args;
    }
    return arg;
}

QStringList SetDateTimeForm::showArgs()
{
    QStringList arg;
    if(m_showArgs.count() == 0) {
        arg << tr("年");
        arg << tr("月");
        arg << tr("日");
        arg << tr("时");
        arg << tr("分");
        arg << tr("秒");
    } else {
        updateFromUi();
        arg << m_showArgs;
    }
    return arg;
}

QString SetDateTimeForm::description()
{
    QString desc = tr("函数描述:设置系统时间");
    desc += "\r\n";
    desc += "示例:SetDateTime(2022,10,1,1,2,3)  // 设置系统时间为2022-10-01 01:02:03";
    return desc;
}

QString SetDateTimeForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(" + args().join(",") + ");";
    func += ":";
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

QString SetDateTimeForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(" + showArgs().join(",") + ");";
    return func;
}

bool SetDateTimeForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(", "");
        arg = arg.replace(");", "");
        QStringList args = arg.split(",");
        if(args.count() == 6) {
            m_args = args;
            m_showArgs = args;
            ui->spinBoxYear->setValue(m_args[0].toUInt());
            ui->spinBoxMonth->setValue(m_args[1].toUInt());
            ui->spinBoxDay->setValue(m_args[2].toUInt());
            ui->spinBoxHour->setValue(m_args[3].toUInt());
            ui->spinBoxMinute->setValue(m_args[4].toUInt());
            ui->spinBoxSecond->setValue(m_args[5].toUInt());
        }
        return true;
    }
    return false;
}
