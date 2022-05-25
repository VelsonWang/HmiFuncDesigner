#include "statechangeform.h"
#include "ui_statechangeform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

StateChangeForm::StateChangeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StateChangeForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
    QStringList names;
    QSoftCore::getCore()->getProjectCore()->m_tagMgr.getAllTagName(names);
    foreach(QString name, names) {
        QStringList idName = name.split(":");//变量id:变量名称
        if(idName.size() == 2) {
            ui->cboSelectTag->addItem(idName.at(1), idName.at(0));
        }
    }
}

StateChangeForm::~StateChangeForm()
{
    delete ui;
}

void StateChangeForm::updateFromUi()
{
    m_arg = "";
    int index = ui->cboSelectTag->currentIndex();
    m_arg = ui->cboSelectTag->itemData(index).toString();
    m_showArg = "";
    m_showArg = ui->cboSelectTag->currentText();
}

void StateChangeForm::on_cboSelectPage_currentTextChanged(const QString &page)
{
    Q_UNUSED(page)
    updateFromUi();
}

QString StateChangeForm::group()
{
    return tr("变量操作");
}

QString StateChangeForm::name()
{
    return "StateChange";
}

QString StateChangeForm::showName()
{
    return tr("变量0-1切换");
}

QStringList StateChangeForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "char *tagID";
    } else {
        arg << m_arg;
    }
    return arg;
}

QStringList StateChangeForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("变量ID");
    } else {
        arg << m_showArg;
    }
    return arg;
}

QString StateChangeForm::description()
{
    QString desc = tr("变量0-1切换");
    desc += "\r\n";
    desc += "示例: StateChange(\"var0\")  // 切换变量var0的状态";

    return desc;
}

QString StateChangeForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

QString StateChangeForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("") + "\");";
    return func;
}

bool StateChangeForm::fromString(const QString func)
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

        ui->cboSelectTag->setCurrentText(m_showArg);
        int index = ui->cboSelectTag->currentIndex();
        ui->cboSelectTag->setItemData(index, m_arg);
        return true;
    }
    return false;
}
