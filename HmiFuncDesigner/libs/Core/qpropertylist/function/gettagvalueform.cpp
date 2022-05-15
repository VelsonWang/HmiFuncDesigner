#include "gettagvalueform.h"
#include "ui_gettagvalueform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

GetTagValueForm::GetTagValueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetTagValueForm)
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
            ui->cboRetSelectTag->addItem(idName.at(1), idName.at(0));
        }
    }
    ui->cboSelectTag->setCurrentIndex(0);
    ui->cboRetSelectTag->setCurrentIndex(0);
}

GetTagValueForm::~GetTagValueForm()
{
    delete ui;
}


void GetTagValueForm::updateFromUi()
{
    m_args.clear();
    m_showArgs.clear();
    int index = ui->cboSelectTag->currentIndex();
    m_args << ui->cboSelectTag->itemData(index).toString();
    m_showArgs << ui->cboSelectTag->currentText();
    index = ui->cboRetSelectTag->currentIndex();
    m_args << ui->cboRetSelectTag->itemData(index).toString();
    m_showArgs << ui->cboRetSelectTag->currentText();
}

QString GetTagValueForm::group()
{
    return tr("变量操作");
}

QString GetTagValueForm::name()
{
    return "GetTagValue";
}

QString GetTagValueForm::showName()
{
    return tr("获取变量的值");
}

QStringList GetTagValueForm::args()
{
    QStringList arg;
    if(m_args.count() == 0) {
        arg << "char *tagID";
        arg << "char *retTagID";
    } else {
        arg << m_args;
    }
    return arg;
}

QStringList GetTagValueForm::showArgs()
{
    QStringList arg;
    if(m_showArgs.count() == 0) {
        arg << tr("变量ID");
        arg << tr("返回存储变量ID");
    } else {
        arg << m_showArgs;
    }
    return arg;
}

QString GetTagValueForm::description()
{
    QString desc = tr("获取变量的值");
    desc += "\r\n";
    desc += "示例: GetTagValue(\"var0\", \"var1\")  // 获取变量var0的值保存至变量var1";
    return desc;
}

QString GetTagValueForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join("\",\"") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

QString GetTagValueForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join("\",\"") + "\");";
    return func;
}

bool GetTagValueForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(\"", ""); //移除"GetTagValueForm(""
        arg = arg.replace("\");", ""); //移除"");"

        QStringList args = arg.split("\",\"");
        if(args.count() == 2) {
            m_args = args;
        }

        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(\"", ""); //移除"获取变量的值(""
        showArg = showArg.replace("\");", ""); //移除"")"
        args = showArg.split("\",\"");
        if(args.count() == 2) {
            m_showArgs = args;
        }

        if(m_args.count() == m_showArgs.count()) {
            QString text0 = m_showArgs.at(0);
            QString id0 = m_args.at(0);
            QString text1 = m_showArgs.at(1);
            QString id1 = m_args.at(1);
            ui->cboSelectTag->setCurrentText(text0);
            int index = ui->cboSelectTag->currentIndex();
            ui->cboSelectTag->setItemData(index, id0);
            ui->cboRetSelectTag->setCurrentText(text1);
            index = ui->cboRetSelectTag->currentIndex();
            ui->cboRetSelectTag->setItemData(index, id1);
        }
        return true;
    }
    return false;
}

