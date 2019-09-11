#include "functioneditordialog.h"
#include "ui_functioneditordialog.h"
#include "Helper.h"
#include "XMLObject.h"
#include "Element.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include <QListWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>


FunctionEditorDialog::FunctionEditorDialog(QWidget *parent, QStringList events)
    : QDialog(parent),
      ui(new Ui::FunctionEditorDialog),
      iSelectedCurRow_(-1)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("功能操作编辑"));

    mapNameToShowName_.clear();
    mapShowNameToName_.clear();
    supportEvents_.clear();

    for(int i=0; i<events.size(); i++) {
        QString szNameToShowName = events.at(i);
        QStringList listNameToShowName = szNameToShowName.split('-');
        if ( listNameToShowName.size() == 2 ) {
            supportEvents_ << listNameToShowName.at(1);
            mapNameToShowName_[listNameToShowName.at(0)] = listNameToShowName.at(1);
            mapShowNameToName_[listNameToShowName.at(1)] = listNameToShowName.at(0);
        }
    }

    selectFuncObjItemList_.clear();
    funcObjItemList_.clear();

    variantPropertyManager_ = new QtVariantPropertyManager(this);

    connect(variantPropertyManager_, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(propertyChanged(QtProperty *, const QVariant &)));

    variantEditorFactory_ = new QtVariantEditorFactory(this);

    propertyEditor_ = new QtTreePropertyBrowser(this);
    propertyEditor_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    propertyEditor_->setHeaderLabels(QStringList() << tr("属性") << tr("值"));
    propertyEditor_->setColumnWidth(0, 160);
    propertyEditor_->setColumnWidth(1, 200);
    propertyEditor_->setFactoryForManager(variantPropertyManager_, variantEditorFactory_);
    propertyEditor_->resize(370, propertyEditor_->size().height());
    ui->propTableFuncLayout->addWidget(propertyEditor_);

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
void FunctionEditorDialog::initUI()
{
    initListWidget();
    initTableWidget();
}


/**
 * @brief FunctionEditorDialog::initListWidget
 * @details 初始化列表控件
 */
void FunctionEditorDialog::initListWidget()
{
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
    if(!xmlFuncSupportList.load(buffer, Q_NULLPTR)) {
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
void FunctionEditorDialog::initTableWidget()
{
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


QStringList FunctionEditorDialog::getFunctions()
{
    funcs_.clear();
    int iRowCount = ui->tableEventFunc->rowCount();
    for(int i=0; i<iRowCount; i++) {
        QTableWidgetItem *pItemFuncName = ui->tableEventFunc->item(i, 0);
        QTableWidgetItem *pItemFuncEvent = ui->tableEventFunc->item(i, 1);
        QString szFuncName = pItemFuncName->text();
        QString szFuncEvent = mapShowNameToName_[pItemFuncEvent->text()];
        QString szRowFuncEvent = szFuncName + ":" + szFuncEvent;
        funcs_.append(szRowFuncEvent);
    }
    return funcs_;
}

void FunctionEditorDialog::setFunctions(const QStringList &funcs)
{
    if (funcs_ != funcs) {
        selectFuncObjItemList_.clear();
        funcs_ = funcs;
        foreach (QString szFuncEvent, funcs_) {
            if(szFuncEvent == "")
                continue;
            TFuncObjectItem *pNewFuncObj = new TFuncObjectItem();
            pNewFuncObj->setFuncString(szFuncEvent);
            pNewFuncObj->szEvent_ = mapNameToShowName_[pNewFuncObj->szEvent_];
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
void FunctionEditorDialog::on_btnDel_clicked()
{
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
void FunctionEditorDialog::listItemClicked(QListWidgetItem *item)
{
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
void FunctionEditorDialog::listItemDoubleClicked(QListWidgetItem *item)
{
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
TFuncObjectItem *FunctionEditorDialog::getFuncObjectItem(const QString &name)
{
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
void FunctionEditorDialog::setSupportEvents(QStringList events)
{
    supportEvents_ = events;
}

/**
 * @brief FunctionEditorDialog::clearProperty
 * @details 清空属性列表
 */
void FunctionEditorDialog::clearProperty()
{
    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId_.constBegin();
    while (itProp != propertyToId_.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId_.clear();
    idToProperty_.clear();

    propertyEditor_->clear();
    propList_.clear();
}

void FunctionEditorDialog::on_tableEventFunc_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    iSelectedCurRow_ = ui->tableEventFunc->currentRow();
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == nullptr)
        return;
    QString szFuncNameOrg = ui->tableEventFunc->item(iSelectedCurRow_, 0)->text();
    TFuncObjectItem *pFuncObjItem = selectFuncObjItemList_.at(iSelectedCurRow_);
    if(szFuncNameOrg.indexOf(pFuncObjItem->szName_) > -1) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjItem->szDesc_);
        szSelectedFuncName_ = pFuncObjItem->szName_;
        createPropertyList();
        propertyEditor_->clear();
        QListIterator <QtProperty*> iter(propList_);
        while (iter.hasNext()) {
            propertyEditor_->addProperty(iter.next());
        }
    }
}

void FunctionEditorDialog::createPropertyList()
{
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == nullptr)
        return;
    QTableWidgetItem *pItemNeed = ui->tableEventFunc->item(pItem->row(), 0);
    QString szFuncNameOrg = pItemNeed->text();
    TFuncObjectItem *pFuncObjItem = selectFuncObjItemList_.at(pItem->row());
    if(szFuncNameOrg.indexOf(pFuncObjItem->szName_) > -1) {
        clearProperty();
        updateExpandState();

        QtVariantProperty *property = Q_NULLPTR;

        property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("事件类型"));
        property->setAttribute(QLatin1String("enumNames"), supportEvents_);
        property->setValue(pFuncObjItem->szEvent_);
        addProperty(property, QLatin1String("eventType"));

        property = variantPropertyManager_->addProperty(QVariant::String, pFuncObjItem->szName_);
        property->setAttribute(QLatin1String("readOnly"), true);
        addProperty(property, QLatin1String("funcName"));

        foreach (TArgItem *pArgItem, pFuncObjItem->argList_) {
            if(pArgItem->type == "VALUE") {
                property = variantPropertyManager_->addProperty(QVariant::String, pArgItem->name);
                if(pArgItem->value == "")
                    pArgItem->value = "0";
                property->setValue(pArgItem->value);
                addProperty(property, QLatin1String("value"));
                if(pArgItem->value == "0") {
                    propertyChanged(property, pArgItem->value);
                }
            } else if(pArgItem->type == "TAGLIST") {
                property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), pArgItem->name);
                tagNames_.clear();
                TagManager::getAllTagName(TagManager::getProjectPath(), tagNames_);
                property->setAttribute(QLatin1String("enumNames"), tagNames_);
                if(pArgItem->value == "")
                    pArgItem->value = tagNames_.at(0);
                property->setValue(tagNames_.indexOf(pArgItem->value));
                addProperty(property, QLatin1String("tag"));
                if(pArgItem->value == tagNames_.at(0)) {
                    propertyChanged(property, pArgItem->value);
                }
            } else if(pArgItem->type == "GRAPHPAGELIST") {
                property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), pArgItem->name);
                DrawListUtils::loadDrawList(DrawListUtils::getProjectPath());
                property->setAttribute(QLatin1String("enumNames"), DrawListUtils::drawList_);
                if(pArgItem->value == "")
                    pArgItem->value = DrawListUtils::drawList_.at(0);
                property->setValue(DrawListUtils::drawList_.indexOf(pArgItem->value));
                addProperty(property, QLatin1String("graph"));
                if(pArgItem->value == DrawListUtils::drawList_.at(0)) {
                    propertyChanged(property, pArgItem->value);
                }
            } else if(pArgItem->type == "ELEMENTIDLIST") {
                property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), pArgItem->name);
                elementIds_.clear();
                ElementIDHelper::getAllElementIDName(ElementIDHelper::getProjectPath(), elementIds_);
                property->setAttribute(QLatin1String("enumNames"), elementIds_);
                if(pArgItem->value == "")
                    pArgItem->value = elementIds_.at(0);
                property->setValue(elementIds_.indexOf(pArgItem->value));
                addProperty(property, QLatin1String("element"));
                if(pArgItem->value == elementIds_.at(0)) {
                    propertyChanged(property, pArgItem->value);
                }
            }
        }
    }
}


QList<QtProperty *> FunctionEditorDialog::getPropertyList() const
{
    return propList_;
}

void FunctionEditorDialog::propertyChanged(QtProperty *property, const QVariant &value)
{
    if (!propertyToId_.contains(property))
        return;

    QString id = propertyToId_[property];

    TFuncObjectItem *pFuncObjItem;
    pFuncObjItem = selectFuncObjItemList_.at(iSelectedCurRow_);

    QString szName = property->propertyName();
    foreach (TArgItem *pArgItem, pFuncObjItem->argList_) {
        if(pArgItem->name == szName) {
            QString szVal = "null";
            if (id == QLatin1String("value")) {
                szVal = value.toString();
            } else if (id == QLatin1String("tag")) {
                szVal = tagNames_.at(value.toInt());
            } else if (id == QLatin1String("graph")) {
                szVal = DrawListUtils::drawList_.at(value.toInt());
            }  else if (id == QLatin1String("element")) {
                szVal = elementIds_.at(value.toInt());
            }  else  {

            }
            pArgItem->value = szVal;
            break;
        }
    }

    if(szName == tr("事件类型")) {
        pFuncObjItem->szEvent_ = supportEvents_.at(value.toInt());
    }

    QString szFunc = pFuncObjItem->getFuncString();
    ui->tableEventFunc->item(iSelectedCurRow_, 0)->setText(szFunc);
    ui->tableEventFunc->item(iSelectedCurRow_, 1)->setText(pFuncObjItem->szEvent_);
}


void FunctionEditorDialog::addProperty(QtVariantProperty *property, const QString &id)
{
    propList_.append(property);
    propertyToId_[property] = id;
    idToProperty_[id] = property;
    QtBrowserItem *item = propertyEditor_->addProperty(property);
    if (idToExpanded_.contains(id)) {
        propertyEditor_->setExpanded(item, idToExpanded_[id]);
    }
}


void FunctionEditorDialog::updateExpandState()
{
    QList<QtBrowserItem *> list = propertyEditor_->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        QtProperty *prop = item->property();
        idToExpanded_[propertyToId_[prop]] = propertyEditor_->isExpanded(item);
    }
}
