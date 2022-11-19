#include "returngraphpageform.h"
#include "ui_returngraphpageform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

ReturnGraphPage::ReturnGraphPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReturnGraphPage)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
}

ReturnGraphPage::~ReturnGraphPage()
{
    delete ui;
}

QString ReturnGraphPage::group()
{
    return tr("系统操作");
}

QString ReturnGraphPage::name()
{
    return "ReturnGraphPage";
}

QString ReturnGraphPage::showName()
{
    return tr("返回画面");
}

QStringList ReturnGraphPage::args()
{
    return QStringList();
}

QStringList ReturnGraphPage::showArgs()
{
    return QStringList();
}

QString ReturnGraphPage::description()
{
    return tr("返回至上一画面");
}

QString ReturnGraphPage::toString()
{
    QString func;
    func = name() + "(" + args().join("") + ");";
    func += ":";
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

QString ReturnGraphPage::toShowString()
{
    QString func;
    func += showName() + "(" + showArgs().join("") + ");";
    return func;
}

bool ReturnGraphPage::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(", ""); //移除"ReturnGraphPage(""
        arg = arg.replace(");", ""); //移除");"
        m_arg = arg;

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(", ""); //移除"返回画面(""
        showArg = showArg.replace(");", ""); //移除"")"
        m_showArg = arg;

        return true;
    }
    return false;
}
