#include "FunctionEditorDialog.h"
#include "ui_FunctionEditorDialog.h"
#include "Helper.h"
#include "xmlobject.h"
#include "Element.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include <QListWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>


FunctionEditorDialog::FunctionEditorDialog(QWidget *parent, QStringList events) :
    QDialog(parent),
    ui(new Ui::FunctionEditorDialog),
    supportEvents_(events),
    iSelectedCurRow_(-1)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("功能操作编辑"));
    selectFuncObjItemList_.clear();
    funcObjItemList_.clear();
    propertyModel_ = new PropertyModel();
    connect(propertyModel_, SIGNAL(onDataChangedByEditor(Property* )), SLOT(propertyChanged(Property* )));
    propertyView_ = new PropertyTableView(propertyModel_);
    propertyView_->setPropertyKeyColumnWidth(160);
    propertyView_->setPropertyValueColumnWidth(200);
    propertyView_->resize(370, propertyView_->size().height());
    propertyView_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->propTableFuncLayout->addWidget(propertyView_);
    initUI();
}

FunctionEditorDialog::~FunctionEditorDialog()
{
    delete ui;

    for(int i=0; i<funcObjItemList_.count(); i++) {
        qDeleteAll(funcObjItemList_.at(i)->argList_);
        funcObjItemList_.at(i)->argList_.clear();
    }
    qDeleteAll(funcObjItemList_);
    funcObjItemList_.clear();

    for(int i=0; i<selectFuncObjItemList_.count(); i++) {
        qDeleteAll(selectFuncObjItemList_.at(i)->argList_);
        selectFuncObjItemList_.at(i)->argList_.clear();
    }
    qDeleteAll(selectFuncObjItemList_);
    selectFuncObjItemList_.clear();
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

    QString xmlFileName = Helper::AppDir() + "/Config/DrawAppJScriptFun.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists()) {
        QMessageBox::critical(this, tr("提示"), tr("函数配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, 0)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* funcGroup, childrenFuncSupport) {
        QString szFuncGroupName = funcGroup->getProperty("funcName");
        QTabWidget *pTab = ui->tabFuncSelect;
        QListWidget *pListWidget = new QListWidget(pTab);
        listWidgetList_.append(pListWidget);
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
        foreach(XMLObject* func, childrenGroup) {
            TFuncObjectItem *pFuncObjItem = new TFuncObjectItem();
            QString funcName = func->getProperty("name");
            pFuncObjItem->szName_ = funcName;
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
                    TArgItem *pArgItem = new TArgItem();
                    pArgItem->name = arg->getProperty("name");
                    pArgItem->type = arg->getProperty("type").toUpper();
                    pFuncObjItem->argList_.append(pArgItem);
                    strDesc += QString("\t");
                    strDesc += strArg;
                    strDesc += QString("\n");
                }
            }
            QString strFuncDemo = func->getCurrentChild("demo")->getText();
            strDesc += strFuncDemo;
            strDesc += QString("\n");

            strDesc.replace(QString("_N"), QString("\n"));
            strDesc.replace(QString("_T"), QString("    "));

            if(supportEvents_.count() > 0)
                pFuncObjItem->szEvent_ = supportEvents_.at(0);
            pFuncObjItem->szDesc_ = strDesc;
            QString strFuncOrg = strFuncOrgName.right(strFuncOrgName.length() - strFuncOrgName.indexOf(":") - 1);
            pFuncObjItem->szFuncNameOrg_ = strFuncOrg;
            funcObjItemList_.append(pFuncObjItem);
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
    ui->tableEventFunc->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableEventFunc->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEventFunc->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableEventFunc->setColumnWidth(0, 240);
    ui->tableEventFunc->setColumnWidth(1, 160);
}


QStringList FunctionEditorDialog::getFunctions() {
    funcs_.clear();
    int iRowCount = ui->tableEventFunc->rowCount();
    for(int i=0; i<iRowCount; i++) {
        QTableWidgetItem *pItemFuncName = ui->tableEventFunc->item(i, 0);
        QTableWidgetItem *pItemFuncEvent = ui->tableEventFunc->item(i, 1);
        QString szFuncName = pItemFuncName->text();
        QString szFuncEvent = pItemFuncEvent->text();
        QString szRowFuncEvent = szFuncName + ":" + szFuncEvent;
        funcs_.append(szRowFuncEvent);
    }
    return funcs_;
}

void FunctionEditorDialog::setFunctions(const QStringList &funcs) {
    if (funcs_ != funcs) {
        selectFuncObjItemList_.clear();
        funcs_ = funcs;
        foreach (QString szFuncEvent, funcs_) {
            if(szFuncEvent == "")continue;
            TFuncObjectItem *pNewFuncObj = new TFuncObjectItem();
            pNewFuncObj->setFuncString(szFuncEvent);
            TFuncObjectItem *pFuncObjectItem = getFuncObjectItem(pNewFuncObj->szName_);
            if(pFuncObjectItem == nullptr)
                continue;
            int count = pNewFuncObj->argList_.count();
            for(int i=0; i<count; i++) {
                ArgItem *pArgItem = pNewFuncObj->argList_.at(i);
                pArgItem->type = pFuncObjectItem->argList_.at(i)->type;
                pArgItem->name = pFuncObjectItem->argList_.at(i)->name;
            }
            pNewFuncObj->szDesc_ = pFuncObjectItem->szDesc_;
            pNewFuncObj->szFuncNameOrg_ = pFuncObjectItem->szFuncNameOrg_;
            selectFuncObjItemList_.append(pNewFuncObj);

            int iRowCount = ui->tableEventFunc->rowCount();
            ui->tableEventFunc->insertRow(iRowCount);
            QTableWidgetItem *itemFuncName = new QTableWidgetItem(pNewFuncObj->getFuncString());
            ui->tableEventFunc->setItem(iRowCount, 0, itemFuncName);
            QTableWidgetItem *itemFuncEvent = new QTableWidgetItem(pNewFuncObj->szEvent_);
            ui->tableEventFunc->setItem(iRowCount, 1, itemFuncEvent);
        }
    }
}

void FunctionEditorDialog::on_btnAdd_clicked()
{
    foreach (QListWidget *pListWidget, listWidgetList_) {
        QListWidgetItem *pCurItem = pListWidget->currentItem();
        if(pCurItem != nullptr) {
            listItemDoubleClicked(pCurItem);
        }
    }
}

/**
 * @brief FunctionEditorDialog::on_btnDel_clicked
 * @details 删除选中的功能函数
 */
void FunctionEditorDialog::on_btnDel_clicked() {
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == nullptr)
        return;
    ui->tableEventFunc->removeRow(pItem->row());
}

/**
 * @brief FunctionEditorDialog::on_btnMoveUp_clicked
 * @details 行上移
 */
void FunctionEditorDialog::on_btnMoveUp_clicked()
{
    int iRowCount = ui->tableEventFunc->rowCount();
    int iCurRow = ui->tableEventFunc->currentRow();

    if(iRowCount > 1 && iCurRow > 0) {
        QTableWidgetItem *pItemCurFuncName = ui->tableEventFunc->item(iCurRow, 0);
        QTableWidgetItem *pItemCurFuncEvent = ui->tableEventFunc->item(iCurRow, 1);
        QString szCurFuncNameOrg = pItemCurFuncName->text();
        QString szCurFuncEvent = pItemCurFuncEvent->text();

        QTableWidgetItem *pItemUpFuncName = ui->tableEventFunc->item(iCurRow-1, 0);
        QTableWidgetItem *pItemUpFuncEvent = ui->tableEventFunc->item(iCurRow-1, 1);
        QString szUpFuncNameOrg = pItemUpFuncName->text();
        QString szUpFuncEvent = pItemUpFuncEvent->text();

        pItemCurFuncName->setText(szUpFuncNameOrg);
        pItemCurFuncEvent->setText(szUpFuncEvent);

        pItemUpFuncName->setText(szCurFuncNameOrg);
        pItemUpFuncEvent->setText(szCurFuncEvent);

        ui->tableEventFunc->selectRow(iCurRow-1);
    }
}

/**
 * @brief FunctionEditorDialog::on_btnMoveDown_clicked
 * @details 行下移
 */
void FunctionEditorDialog::on_btnMoveDown_clicked()
{
    int iRowCount = ui->tableEventFunc->rowCount();
    int iCurRow = ui->tableEventFunc->currentRow();

    if(iRowCount > 1 && iCurRow < (iRowCount-1)) {
        QTableWidgetItem *pItemCurFuncName = ui->tableEventFunc->item(iCurRow, 0);
        QTableWidgetItem *pItemCurFuncEvent = ui->tableEventFunc->item(iCurRow, 1);
        QString szCurFuncNameOrg = pItemCurFuncName->text();
        QString szCurFuncEvent = pItemCurFuncEvent->text();

        QTableWidgetItem *pItemDownFuncName = ui->tableEventFunc->item(iCurRow+1, 0);
        QTableWidgetItem *pItemDownFuncEvent = ui->tableEventFunc->item(iCurRow+1, 1);
        QString szDownFuncNameOrg = pItemDownFuncName->text();
        QString szDownFuncEvent = pItemDownFuncEvent->text();

        pItemCurFuncName->setText(szDownFuncNameOrg);
        pItemCurFuncEvent->setText(szDownFuncEvent);

        pItemDownFuncName->setText(szCurFuncNameOrg);
        pItemDownFuncEvent->setText(szCurFuncEvent);

        ui->tableEventFunc->selectRow(iCurRow+1);
    }
}

void FunctionEditorDialog::on_btnOk_clicked()
{
    this->accept();
}

void FunctionEditorDialog::on_btnCancel_clicked()
{
    this->reject();
}

/**
 * @brief FunctionEditorDialog::listItemClicked
 * @details 列表项函数被单击
 * @param item 列表项
 */
void FunctionEditorDialog::listItemClicked(QListWidgetItem *item) {
    TFuncObjectItem *pFuncObjectItem = getFuncObjectItem(item->text());
    if(pFuncObjectItem != nullptr) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjectItem->szDesc_);
        szSelectedFuncName_ = pFuncObjectItem->szName_;
    }
    clearProperty();
}

/**
 * @brief FunctionEditorDialog::listItemDoubleClicked
 * @details 列表项函数被双击
 * @param item 列表项
 */
void FunctionEditorDialog::listItemDoubleClicked(QListWidgetItem *item) {
    int iRowCount = ui->tableEventFunc->rowCount();
    ui->tableEventFunc->insertRow(iRowCount);
    TFuncObjectItem *pFuncObjectItem = getFuncObjectItem(item->text());
    QString szFuncNameOrg = pFuncObjectItem->szFuncNameOrg_;
    QTableWidgetItem *itemFuncName = new QTableWidgetItem(szFuncNameOrg);
    ui->tableEventFunc->setItem(iRowCount, 0, itemFuncName);

    QString szFuncEvent = pFuncObjectItem->szEvent_;
    QTableWidgetItem *itemFuncEvent = new QTableWidgetItem(szFuncEvent);
    ui->tableEventFunc->setItem(iRowCount, 1, itemFuncEvent);

    szSelectedFuncName_ = pFuncObjectItem->szName_;

    TFuncObjectItem *pNewFuncObj = new TFuncObjectItem();
    pNewFuncObj->szName_ = pFuncObjectItem->szName_;
    pNewFuncObj->szEvent_ = pFuncObjectItem->szEvent_;
    int count = pFuncObjectItem->argList_.count();
    for(int i=0; i<count; i++) {
        ArgItem *pArgItem = new ArgItem();
        pArgItem->type = pFuncObjectItem->argList_.at(i)->type;
        pArgItem->name = pFuncObjectItem->argList_.at(i)->name;
        pArgItem->value = pFuncObjectItem->argList_.at(i)->value;
        pNewFuncObj->argList_.append(pArgItem);
    }
    pNewFuncObj->szDesc_ = pFuncObjectItem->szDesc_;
    pNewFuncObj->szFuncNameOrg_ = pFuncObjectItem->szFuncNameOrg_;
    selectFuncObjItemList_.append(pNewFuncObj);

    clearProperty();
}


/**
 * @brief FunctionEditorDialog::getFuncObjectItem
 * @details 查找函数数据对象
 * @param name 函数名称
 * @return 函数数据对象
 */
TFuncObjectItem *FunctionEditorDialog::getFuncObjectItem(const QString &name) {
    for(int i=0; i<funcObjItemList_.count(); i++) {
        TFuncObjectItem *pFuncObjItem = funcObjItemList_.at(i);
        if(pFuncObjItem->szName_ == name){
            return pFuncObjItem;
        }
    }
    return nullptr;
}

/**
 * @brief FunctionEditorDialog::setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void FunctionEditorDialog::setSupportEvents(QStringList events) {
    supportEvents_ = events;
}

/**
 * @brief FunctionEditorDialog::clearProperty
 * @details 清空属性列表
 */
void FunctionEditorDialog::clearProperty() {
    propertyModel_->resetModel();
    qDeleteAll(propList_);
    propList_.clear();
}

void FunctionEditorDialog::on_tableEventFunc_clicked(const QModelIndex &index) {
    Q_UNUSED(index)
    iSelectedCurRow_ = ui->tableEventFunc->currentRow();
    clearProperty();
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == nullptr)
        return;
    QString szFuncNameOrg = ui->tableEventFunc->item(iSelectedCurRow_, 0)->text();
    TFuncObjectItem *pFuncObjItem = selectFuncObjItemList_.at(iSelectedCurRow_);
    if(szFuncNameOrg.indexOf(pFuncObjItem->szName_) > -1) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjItem->szDesc_);
        szSelectedFuncName_ = pFuncObjItem->szName_;
        createPropertyList();
        propertyModel_->resetModel();
        QListIterator<Property *> iter(getPropertyList());
        while (iter.hasNext()) {
            propertyModel_->addProperty(iter.next());
        }
    }
}

void FunctionEditorDialog::createPropertyList() {

    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == nullptr)
        return;
    QTableWidgetItem *pItemNeed = ui->tableEventFunc->item(pItem->row(), 0);
    QString szFuncNameOrg = pItemNeed->text();
    TFuncObjectItem *pFuncObjItem = selectFuncObjItemList_.at(pItem->row());
    if(szFuncNameOrg.indexOf(pFuncObjItem->szName_) > -1) {

        ListProperty *propEvent = new ListProperty(tr("事件类型"));
        propEvent->setId(EL_EVENT);
        propEvent->setList(supportEvents_);
        propEvent->setValue(pFuncObjItem->szEvent_);
        propList_.insert(propList_.end(), propEvent);

        EmptyProperty *funcNameProp = new EmptyProperty(pFuncObjItem->szName_);
        propList_.insert(propList_.end(), funcNameProp);
        foreach (TArgItem *pArgItem, pFuncObjItem->argList_) {
            if(pArgItem->type == "VALUE") {
                TextProperty *varProp = new TextProperty(pArgItem->name);
                varProp->setId(EL_TEXT);
                propList_.insert(propList_.end(), varProp);
                if(pArgItem->value == "")
                    pArgItem->value = "0";
                varProp->setValue(pArgItem->value);
            } else if(pArgItem->type == "TAGLIST") {
                ListProperty *tagProp = new ListProperty(pArgItem->name);
                tagProp->setId(EL_TAG);
                QStringList varList;
                TagManager::getAllTagName(TagManager::getProjectPath(), varList);
                tagProp->setList(varList);
                propList_.insert(propList_.end(), tagProp);
                if(pArgItem->value == "")
                    pArgItem->value = varList.at(0);
                tagProp->setValue(pArgItem->value);
            } else if(pArgItem->type == "GRAPHPAGELIST") {
                ListProperty *graphProp = new ListProperty(pArgItem->name);
                graphProp->setId(EL_GRAPHPAGE);
                DrawListUtils::loadDrawList(DrawListUtils::getProjectPath());
                graphProp->setList(DrawListUtils::drawList_);
                propList_.insert(propList_.end(), graphProp);
                if(pArgItem->value == "")
                    pArgItem->value = DrawListUtils::drawList_.at(0);
                graphProp->setValue(pArgItem->value);
            } else if(pArgItem->type == "ELEMENTIDLIST") {
                ListProperty *idProp = new ListProperty(pArgItem->name);
                idProp->setId(EL_ID);
                QStringList idList;
                ElementIDHelper::getAllElementIDName(ElementIDHelper::getProjectPath(), idList);
                idProp->setList(idList);
                propList_.insert(propList_.end(), idProp);
                if(pArgItem->value == "")
                    pArgItem->value = idList.at(0);
                idProp->setValue(pArgItem->value);
            }
        }
    }
}

void FunctionEditorDialog::updateElementProperty(uint id, const QVariant &value) {
    TFuncObjectItem *pFuncObjItem;
    pFuncObjItem = selectFuncObjItemList_.at(iSelectedCurRow_);

    foreach (Property *prop, propList_) {
        if(prop->getId() == id) {
            QString szName = prop->getName();
            foreach (TArgItem *pArgItem, pFuncObjItem->argList_) {
                if(pArgItem->name == szName) {
                    pArgItem->value = value.toString();
                    break;
                }
            }
            if(szName == tr("事件类型")) {
                pFuncObjItem->szEvent_ = value.toString();
            }

            QString szFunc = pFuncObjItem->getFuncString();
            ui->tableEventFunc->item(iSelectedCurRow_, 0)->setText(szFunc);
            ui->tableEventFunc->item(iSelectedCurRow_, 1)->setText(pFuncObjItem->szEvent_);
            break;
        }
    }
}

QList<Property *> FunctionEditorDialog::getPropertyList() const {
    return propList_;
}

void FunctionEditorDialog::propertyChanged(Property *property) {
    updateElementProperty(property->getId(), property->getValue());
}
