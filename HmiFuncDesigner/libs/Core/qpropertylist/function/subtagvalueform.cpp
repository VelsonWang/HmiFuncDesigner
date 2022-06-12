#include "subtagvalueform.h"
#include "ui_subtagvalueform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

SubTagValueForm::SubTagValueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubTagValueForm)
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
    ui->editTagValue->setText("1");
}

SubTagValueForm::~SubTagValueForm()
{
    delete ui;
}

void SubTagValueForm::updateFromUi()
{
    m_args.clear();
    int index = ui->cboSelectTag->currentIndex();
    m_args << ui->cboSelectTag->itemData(index).toString()
           << ui->editTagValue->text();

    m_showArgs.clear();
    m_showArgs << ui->cboSelectTag->currentText()
               << ui->editTagValue->text();
}

void SubTagValueForm::on_cboSelectTag_currentTextChanged(const QString &id)
{
    Q_UNUSED(id)
    updateFromUi();
}

QString SubTagValueForm::group()
{
    return tr("变量操作");
}

QString SubTagValueForm::name()
{
    return "SubTagValue";
}

QString SubTagValueForm::showName()
{
    return tr("变量值自减");
}

QStringList SubTagValueForm::args()
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

QStringList SubTagValueForm::showArgs()
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

QString SubTagValueForm::description()
{
    QString desc = tr("变量值自减");
    desc += "\r\n";
    desc += "示例: SubTagValue(\"var0\", \"1\")  // 变量var0值自减1";
    return desc;
}

QString SubTagValueForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("\",\"") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

QString SubTagValueForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

bool SubTagValueForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(\"", "");
        arg = arg.replace("\");", "");

        QStringList args = arg.split("\",\"");
        if(args.count() == 2) {
            m_args = args;
        }

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(\"", "");
        showArg = showArg.replace("\");", "");
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
