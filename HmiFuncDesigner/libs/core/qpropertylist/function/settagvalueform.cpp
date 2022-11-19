#include "settagvalueform.h"
#include "ui_settagvalueform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

SetTagValueForm::SetTagValueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetTagValueForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_args.clear();
    m_showArgs.clear();
    QStringList names;
    QSoftCore::getCore()->getProjectCore()->m_tagMgr.getAllTagName(names);
    foreach(QString name, names) {
        QStringList idName = name.split(":");//变量id:变量名称
        if(idName.size() == 2) {
            ui->cboSelectTag->addItem(idName.at(1), idName.at(0));
        }
    }
    ui->editTagValue->setText("");
}

SetTagValueForm::~SetTagValueForm()
{
    delete ui;
}

void SetTagValueForm::updateFromUi()
{
    m_args.clear();
    int index = ui->cboSelectTag->currentIndex();
    m_args << ui->cboSelectTag->itemData(index).toString()
           << ui->editTagValue->text();

    m_showArgs.clear();
    m_showArgs << ui->cboSelectTag->currentText()
               << ui->editTagValue->text();
}

void SetTagValueForm::on_cboSelectTag_currentTextChanged(const QString &id)
{
    Q_UNUSED(id)
    updateFromUi();
}

QString SetTagValueForm::group()
{
    return tr("变量操作");
}

QString SetTagValueForm::name()
{
    return "SetTagValue";
}

QString SetTagValueForm::showName()
{
    return tr("设置变量值");
}

QStringList SetTagValueForm::args()
{
    QStringList arg;
    if(m_args.count() == 0) {
        arg << "char *tagID";
        arg << "char *tagValue";
    } else {
        arg << m_args;
    }
    return arg;
}

QStringList SetTagValueForm::showArgs()
{
    QStringList arg;
    if(m_showArgs.count() == 0) {
        arg << tr("变量ID");
        arg << tr("变量值");
    } else {
        arg << m_showArgs;
    }
    return arg;
}

QString SetTagValueForm::description()
{
    QString desc = tr("设置变量值");
    desc += "\r\n";
    desc += "示例: SetTagValue(\"var0\", \"123.12\")  // 设置变量var0的值为123.12";
    return desc;
}

QString SetTagValueForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("\",\"") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

QString SetTagValueForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

bool SetTagValueForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(\"", ""); //移除"SetTagValueForm(""
        arg = arg.replace("\");", ""); //移除"");"

        QStringList args = arg.split("\",\"");
        if(args.count() == 2) {
            m_args = args;
        }

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(\"", ""); //移除"设置变量值(""
        showArg = showArg.replace("\");", ""); //移除"")"
        args = showArg.split("\",\"");
        if(args.count() == 2) {
            m_showArgs = args;
        }

        if(m_args.count() == m_showArgs.count()) {
            ui->editTagValue->setText(m_args.at(1));
            ui->cboSelectTag->setCurrentText(m_showArgs.at(0));
            int index = ui->cboSelectTag->currentIndex();
            ui->cboSelectTag->setItemData(index, m_args.at(0));
        }
        return true;
    }
    return false;
}
