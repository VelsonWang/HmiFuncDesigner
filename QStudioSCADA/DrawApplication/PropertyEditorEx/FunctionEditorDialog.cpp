#include "FunctionEditorDialog.h"
#include "ui_FunctionEditorDialog.h"
#include "Helper.h"
#include "xmlobject.h"
#include <QListWidget>
#include <QMessageBox>

#include <QDebug>

FunctionEditorDialog::FunctionEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionEditorDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("功能操作编辑"));
    propertyModel_ = new PropertyModel();
    propertyView_ = new PropertyTableView(propertyModel_);
    propertyView_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->propTableFuncLayout->addWidget(propertyView_);
    initUI();
}

FunctionEditorDialog::~FunctionEditorDialog()
{
    delete ui;
}

/**
 * @brief FunctionEditorDialog::initUI
 * @details 初始化ui
 */
void FunctionEditorDialog::initUI() {

    initListWidget();
    initTableWidget();

}


/**
 * @brief FunctionEditorDialog::initListWidget
 * @details 初始化列表控件
 */
void FunctionEditorDialog::initListWidget() {

    // file encoding="UTF-8"
    QString xmlFileName = Helper::AppDir() + "/Config/DrawAppJScriptFun.xml";

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
        QString szFuncGroupName = funcGroup->getProperty("funcName");
        QTabWidget *pTab = ui->tabFuncSelect;
        QListWidget *pListWidget = new QListWidget(pTab);
        connect(pListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(listItemClicked(QListWidgetItem*)));
        connect(pListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(listItemDoubleClicked(QListWidgetItem*)));
        pTab->insertTab(pTab->count(), pListWidget, szFuncGroupName);
        pTab->setCurrentIndex(0);

        // add child
        QList<XMLObject*> childrenGroup = funcGroup->getChildren();
        if(childrenGroup.size() < 1)
            continue;
        foreach(XMLObject* func, childrenGroup)
        {
            QString funcName = func->getProperty("name");
            QListWidgetItem *pItem = new QListWidgetItem(funcName, pListWidget);
            pListWidget->addItem(pItem);

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
            if(argsXMLObject != nullptr) {
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
            }
            QString strFuncDemo = func->getCurrentChild("demo")->getText();
            strDesc += strFuncDemo;
            strDesc += QString("\n");

            strDesc.replace(QString("_N"), QString("\n"));
            //strDesc.replace(QString("_T"), QString("\t"));
            strDesc.replace(QString("_T"), QString("    "));

            funcNameDescMap_.insert(funcName, strDesc);
            QString strFuncOrg = strFuncOrgName.right(strFuncOrgName.length() - strFuncOrgName.indexOf(":") - 1);
            funcNameNameMap_.insert(funcName, strFuncOrg);
        }
    }
}


/**
 * @brief FunctionEditorDialog::initTableWidget
 * @details 初始化表控件
 */
void FunctionEditorDialog::initTableWidget() {

    QStringList headerLabels;
    headerLabels << tr("功能函数") << tr("事件类型");
    ui->tableEventFunc->setColumnCount(headerLabels.count());
    ui->tableEventFunc->setHorizontalHeaderLabels(headerLabels);
    ui->tableEventFunc->horizontalHeader()->setHighlightSections(false);
    ui->tableEventFunc->verticalHeader()->hide();
}


QStringList FunctionEditorDialog::getFunctions() const {
    return funcs_;
}

void FunctionEditorDialog::on_btnAdd_clicked()
{

}

void FunctionEditorDialog::on_btnDel_clicked()
{

}

void FunctionEditorDialog::on_btnMoveUp_clicked()
{

}

void FunctionEditorDialog::on_btnMoveDown_clicked()
{

}

void FunctionEditorDialog::on_btnOk_clicked()
{
    this->accept();
}

void FunctionEditorDialog::on_btnCancel_clicked()
{

}

/**
 * @brief FunctionEditorDialog::listItemClicked
 * @details 列表项函数被单击
 * @param item 列表项
 */
void FunctionEditorDialog::listItemClicked(QListWidgetItem *item) {
    if(funcNameDescMap_.contains(item->text())) {
        ui->plainTextFuncDesc->setPlainText(funcNameDescMap_.value(item->text()));
        szSelectedFuncName_ = funcNameNameMap_.value(item->text());
    }
}

/**
 * @brief FunctionEditorDialog::listItemDoubleClicked
 * @details 列表项函数被双击
 * @param item 列表项
 */
void FunctionEditorDialog::listItemDoubleClicked(QListWidgetItem *item) {
    qDebug() << szSelectedFuncName_ << item->text();
}
