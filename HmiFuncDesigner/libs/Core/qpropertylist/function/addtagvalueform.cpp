#include "addtagvalueform.h"
#include "ui_addtagvalueform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

AddTagValueForm::AddTagValueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddTagValueForm)
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

AddTagValueForm::~AddTagValueForm()
{
    delete ui;
}

void AddTagValueForm::updateFromUi()
{
    m_args.clear();
    int index = ui->cboSelectTag->currentIndex();
    m_args << ui->cboSelectTag->itemData(index).toString()
           << ui->editTagValue->text();

    m_showArgs.clear();
    m_showArgs << ui->cboSelectTag->currentText()
               << ui->editTagValue->text();
}

void AddTagValueForm::on_cboSelectTag_currentTextChanged(const QString &id)
{
    Q_UNUSED(id)
    updateFromUi();
}

QString AddTagValueForm::group()
{
    return tr("变量操作");
}

QString AddTagValueForm::name()
{
    return "AddTagValue";
}

QString AddTagValueForm::showName()
{
    return tr("变量值自增");
}

QStringList AddTagValueForm::args()
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

QStringList AddTagValueForm::showArgs()
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

QString AddTagValueForm::description()
{
    QString desc = tr("变量值自增");
    desc += "\r\n";
    desc += "示例: AddTagValue(\"var0\", \"1\")  // 变量var0值自增1";
    return desc;
}

QString AddTagValueForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("\",\"") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

QString AddTagValueForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

bool AddTagValueForm::fromString(const QString func)
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
