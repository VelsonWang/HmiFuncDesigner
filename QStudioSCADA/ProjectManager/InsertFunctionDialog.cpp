#include "InsertFunctionDialog.h"
#include "ui_InsertFunctionDialog.h"
#include "configutils.h"
#include "xmlobject.h"
#include "Helper.h"
#include <QFile>
#include <QList>
#include <QStringList>
#include <QMessageBox>
#include <QTreeWidgetItem>


#include <QDebug>

InsertFunctionDialog::InsertFunctionDialog(QWidget *parent) :
    QDialog(parent),
    m_strSelectedFuncName(""),
    ui(new Ui::InsertFunctionDialog)
{
    ui->setupUi(this);

    treeWidgetInit();
}

InsertFunctionDialog::~InsertFunctionDialog()
{
    delete ui;
}

void InsertFunctionDialog::treeWidgetInit()
{
    ui->treeWidgetFunc->setHeaderHidden(true);

    // file encoding="UTF-8"
    QString xmlFileName = Helper::AppDir() + "/Config/JScriptFun.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists())
    {
        QMessageBox::critical(this, tr("提示"), tr("函数配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly))
    {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, 0))
    {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* funcGroup, childrenFuncSupport)
    {
        //qDebug() << funcGroup->get_title() << "  "  << funcGroup->get_property("name");
        QTreeWidgetItem *pItem = new QTreeWidgetItem(ui->treeWidgetFunc);
        QString funcGroupName = funcGroup->getProperty("funcName");
        pItem->setText(0, funcGroupName);

        // add child
        QList<XMLObject*> childrenGroup = funcGroup->getChildren();
        if(childrenGroup.size() < 1)
            continue;
        foreach(XMLObject* func, childrenGroup)
        {
            QTreeWidgetItem *pChildItem = new QTreeWidgetItem();
            QString funcName = func->getProperty("name");
            pChildItem->setText(0, funcName);
            pItem->addChild(pChildItem);

            QList<XMLObject*> funcDesc = func->getChildren();
            if(funcDesc.size() < 1)
                continue;
            QString strDesc = "";
            QString strFuncOrgName = func->getCurrentChild("name")->getText();
            strDesc += strFuncOrgName;
            strDesc += QString("\n");
            QString strFuncDesc = func->getCurrentChild("desc")->getText();
            strDesc += strFuncDesc;
            strDesc += QString("\n");
            XMLObject* argsXMLObject = func->getCurrentChild("args");
            QString strFuncArgs = argsXMLObject->getProperty("name");
            strDesc += strFuncArgs;
            strDesc += QString("\n");

            foreach(XMLObject* arg, argsXMLObject->getChildren())
            {
                QString strArg = arg->getText();
                strDesc += QString("\t");
                strDesc += strArg;
                strDesc += QString("\n");
            }

            QString strFuncDemo = func->getCurrentChild("demo")->getText();
            strDesc += strFuncDemo;
            strDesc += QString("\n");

            strDesc.replace(QString("_N"), QString("\n"));
            //strDesc.replace(QString("_T"), QString("\t"));
            strDesc.replace(QString("_T"), QString("    "));

            m_funcNameDescMap.insert(funcName, strDesc);
            QString strFuncOrg = strFuncOrgName.right(strFuncOrgName.length() - strFuncOrgName.indexOf(":") - 1);
            m_funcNameNameMap.insert(funcName, strFuncOrg);
            //qDebug() << funcName << "    " << strFuncOrg;
        }
    }
    ui->treeWidgetFunc->expandAll();
}


void InsertFunctionDialog::on_treeWidgetFunc_itemClicked(QTreeWidgetItem *item, int column)
{
    //qDebug() << item->text(0);
    if(m_funcNameDescMap.contains(item->text(0)))
    {
        ui->textEditFuncDesc->setText(m_funcNameDescMap.value(item->text(0)));
        m_strSelectedFuncName = m_funcNameNameMap.value(item->text(0));
    }
}

void InsertFunctionDialog::on_btnOk_clicked()
{
    this->accept();
}

void InsertFunctionDialog::on_btnCancel_clicked()
{
    m_strSelectedFuncName = "";
    this->reject();
}

QString InsertFunctionDialog::getSelectedFuncName(void) const
{
    return m_strSelectedFuncName;
}
