#include "movecontrolelementform.h"
#include "ui_movecontrolelementform.h"
#include "../../shared/qprojectcore.h"
#include "../../qsoftcore.h"

MoveControlElementForm::MoveControlElementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveControlElementForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    m_arg = "";
    m_showArg = "";
    m_x = 0;
    m_y = 0;
    QStringList graphs;
    QSoftCore::getCore()->getProjectCore()->getAllElementIDName(graphs);
    foreach(QString graph, graphs) {
        QStringList idName = graph.split(":");//画面id.控件id:画面名称.控件名称
        if(idName.size() == 2) {
            ui->cboSelectElement->addItem(idName.at(1), idName.at(0));
        }
    }
    int width = QSoftCore::getCore()->getProjectCore()->m_projInfoMgr.getGraphPageWidth();
    int height = QSoftCore::getCore()->getProjectCore()->m_projInfoMgr.getGraphPageHeight();
    ui->spinBoxX->setRange(-width, width);
    ui->spinBoxX->setValue(0);
    ui->spinBoxY->setRange(-height, height);
    ui->spinBoxY->setValue(0);
}

MoveControlElementForm::~MoveControlElementForm()
{
    delete ui;
}

void MoveControlElementForm::updateFromUi()
{
    int index = ui->cboSelectElement->currentIndex();
    m_arg = ui->cboSelectElement->itemData(index).toString();
    m_showArg = ui->cboSelectElement->currentText();
    m_x = ui->spinBoxX->value();
    m_y = ui->spinBoxY->value();
}

QString MoveControlElementForm::group()
{
    return tr("控件操作");
}

QString MoveControlElementForm::name()
{
    return "MoveControlElement";
}

QString MoveControlElementForm::showName()
{
    return tr("移动控件");
}

QStringList MoveControlElementForm::args()
{
    QStringList arg;
    if(m_arg == "") {
        arg << "char *id";
        arg << "int x";
        arg << "int y";
    } else {
        arg << m_arg;
        arg << QString::number(m_x);
        arg << QString::number(m_y);
    }
    return arg;
}

QStringList MoveControlElementForm::showArgs()
{
    QStringList arg;
    if(m_showArg == "") {
        arg << tr("控件ID");
        arg << tr("x移动值");
        arg << tr("y移动值");
    } else {
        arg << m_showArg;
        arg << QString::number(m_x);
        arg << QString::number(m_y);
    }
    return arg;
}

QString MoveControlElementForm::description()
{
    QString desc = tr("将指定控件元素坐标移动(x, y)");
    desc += "\r\n";
    desc += "示例: MoveControlElement(\"main.label, -5, -10\")  // 将文本控件\"main.label\"坐标左移5，上移10";
    return desc;
}

QString MoveControlElementForm::toString()
{
    updateFromUi();
    QString func;
    func = name() + "(\"" + args().join(",") + "\");";
    func += ":";
    func += showName() + "(\"" + showArgs().join(",") + "\");";
    return func;
}

QString MoveControlElementForm::toShowString()
{
    updateFromUi();
    QString func;
    func += showName() + "(\"" + showArgs().join(",") + "\");";
    return func;
}

bool MoveControlElementForm::fromString(const QString func)
{
    QStringList funcs = func.split(":");
    if(funcs.size() == 2) {
        //解析参数
        QString arg = funcs.at(0);
        arg = arg.replace(name() + "(\"", "");
        arg = arg.replace("\"", "");
        arg = arg.replace(");", "");
        QStringList args = arg.split(",");
        if(args.count() == 3) {
            m_arg = args[0];
            m_x = args[1].toInt();
            m_y = args[2].toInt();
        }


        //解析显示参数
        QString showArg = funcs.at(1);
        showArg = showArg.replace(showName() + "(\"", "");
        showArg = showArg.replace("\"", "");
        showArg = showArg.replace(");", "");
        QStringList showArgs = showArg.split(",");
        if(showArgs.count() == 3) {
            m_showArg = showArgs[0];
        }

        ui->cboSelectElement->setCurrentText(m_showArg);
        int index = ui->cboSelectElement->currentIndex();
        ui->cboSelectElement->setItemData(index, m_arg);
        ui->spinBoxX->setValue(m_x);
        ui->spinBoxY->setValue(m_y);

        return true;
    }
    return false;
}
