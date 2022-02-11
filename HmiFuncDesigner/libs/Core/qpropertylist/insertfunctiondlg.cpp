﻿#include "insertfunctiondlg.h"
#include "ui_insertfunctiondlg.h"
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QStringList>
#include <QTreeWidgetItem>
#include "../../shared/confighelper.h"
#include "../../shared/xmlobject.h"
#include <QApplication>

#include <QDebug>

InsertFunctionDlg::InsertFunctionDlg(QWidget* parent)
    : QDialog(parent),
      m_selectedFuncName(""),
      ui(new Ui::InsertFunctionDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    treeWidgetInit();
}

InsertFunctionDlg::~InsertFunctionDlg()
{
    delete ui;
}

void InsertFunctionDlg::treeWidgetInit()
{
    ui->treeWidgetFunc->setHeaderHidden(true);

    // file encoding="UTF-8"
    QString xmlFileName = QApplication::applicationDirPath() + "/Config/JScriptFun.xml";

    QFile fileCfg(xmlFileName);
    if (!fileCfg.exists()) {
        QMessageBox::critical(this, tr("提示"), tr("函数配置列表文件不存在！"));
        return;
    }
    if (!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if (!xmlFuncSupportList.load(buffer, NULL)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach (XMLObject* funcGroup, childrenFuncSupport) {
        // qDebug() << funcGroup->getTagName() << "  "  <<
        // funcGroup->getProperty("name");
        QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->treeWidgetFunc);
        QString funcGroupName = funcGroup->getProperty("funcName");
        pItem->setText(0, funcGroupName);

        // add child
        QList<XMLObject*> childrenGroup = funcGroup->getChildren();
        if (childrenGroup.size() < 1) {
            continue;
        }
        foreach (XMLObject* func, childrenGroup) {
            QTreeWidgetItem* pChildItem = new QTreeWidgetItem();
            QString funcName = func->getProperty("name");
            pChildItem->setText(0, funcName);
            pItem->addChild(pChildItem);

            QList<XMLObject*> funcDesc = func->getChildren();
            if (funcDesc.size() < 1) {
                continue;
            }
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

            foreach (XMLObject* arg, argsXMLObject->getChildren()) {
                QString strArg = arg->getText();
                strDesc += QString("\t");
                strDesc += strArg;
                strDesc += QString("\n");
            }

            QString strFuncDemo = func->getCurrentChild("demo")->getText();
            strDesc += strFuncDemo;
            strDesc += QString("\n");

            strDesc.replace(QString("_N"), QString("\n"));
            // strDesc.replace(QString("_T"), QString("\t"));
            strDesc.replace(QString("_T"), QString("    "));

            m_funcNameToDesc.insert(funcName, strDesc);
            QString strFuncOrg = strFuncOrgName.right(
                                     strFuncOrgName.length() - strFuncOrgName.indexOf(":") - 1);
            m_funcNameToName.insert(funcName, strFuncOrg);
            // qDebug() << funcName << "    " << strFuncOrg;
        }
    }
    ui->treeWidgetFunc->expandAll();
}

void InsertFunctionDlg::on_treeWidgetFunc_itemClicked(QTreeWidgetItem* item,
        int column)
{
    Q_UNUSED(column)
    if (m_funcNameToDesc.contains(item->text(0))) {
        ui->textEditFuncDesc->setText(m_funcNameToDesc.value(item->text(0)));
        m_selectedFuncName = m_funcNameToName.value(item->text(0));
    }
}

void InsertFunctionDlg::on_btnOk_clicked()
{
    this->accept();
}

void InsertFunctionDlg::on_btnCancel_clicked()
{
    m_selectedFuncName = "";
    this->reject();
}

QString InsertFunctionDlg::getSelectedFuncName(void) const
{
    return m_selectedFuncName;
}
