#include "functioneditordialog.h"
#include "ui_functioneditordialog.h"
#include "../../shared/confighelper.h"
#include "../../shared/xmlobject.h"
#include "qsoftcore.h"
#include "../../shared/qprojectcore.h"
#include "../shared/qpropertyfactory.h"
#include "../shared/qcommonstruct.h"
#include <QListWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QCoreApplication>


FunctionEditorDialog::FunctionEditorDialog(QWidget *parent, QStringList events)
    : QDialog(parent),
      ui(new Ui::FunctionEditorDialog),
      iSelectedCurRow_(-1)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
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

    m_propertyView = new QPropertyListView(this);
    connect(m_propertyView, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)),
            this, SLOT(onPropertyEdit(QAbstractProperty*, QVariant)));
    m_propertyView->resize(370, m_propertyView->size().height());
    ui->propTableFuncLayout->addWidget(m_propertyView);

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

    if(m_propertyView) {
        delete m_propertyView;
        m_propertyView = Q_NULLPTR;
    }
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
    QString xmlFileName = QCoreApplication::applicationDirPath() + "/Config/DrawAppJScriptFun.xml";

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
            if(argsXMLObject != Q_NULLPTR) {
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
            if(pFuncObjectItem == Q_NULLPTR)
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
        if(pCurItem != Q_NULLPTR) {
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
    if(pItem == Q_NULLPTR)
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
    if(pFuncObjectItem != Q_NULLPTR) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjectItem->szDesc_);
        szSelectedFuncName_ = pFuncObjectItem->szName_;
    }
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
    return Q_NULLPTR;
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


void FunctionEditorDialog::on_tableEventFunc_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    iSelectedCurRow_ = ui->tableEventFunc->currentRow();
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == Q_NULLPTR)
        return;
    QString szFuncNameOrg = ui->tableEventFunc->item(iSelectedCurRow_, 0)->text();
    TFuncObjectItem *pFuncObjItem = selectFuncObjItemList_.at(iSelectedCurRow_);
    if(szFuncNameOrg.indexOf(pFuncObjItem->szName_) > -1) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjItem->szDesc_);
        szSelectedFuncName_ = pFuncObjItem->szName_;
        updatePropertyEditor();
    }
}

void FunctionEditorDialog::updatePropertyEditor()
{
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == Q_NULLPTR)
        return;

    QList<QAbstractProperty *> listProperties;
    QAbstractProperty* pProObj = Q_NULLPTR;

    QTableWidgetItem *pItemNeed = ui->tableEventFunc->item(pItem->row(), 0);
    QString szFuncNameOrg = pItemNeed->text();
    TFuncObjectItem *pFuncObjItem = selectFuncObjItemList_.at(pItem->row());
    if(szFuncNameOrg.indexOf(pFuncObjItem->szName_) > -1) {
        pProObj = QPropertyFactory::create_property("Enum");
        if(pProObj != Q_NULLPTR) {
            pProObj->setObjectProperty("name","eventType");
            pProObj->setAttribute("show_name", tr("事件类型"));
            pProObj->setAttribute("group","Attributes");
            pProObj->setAttribute(ATTR_CAN_SAME,true);

            ComboItems items;
            foreach(QString szEv, supportEvents_) {
                tagComboItem item;
                item.m_text = szEv;
                item.m_value = szEv;
                items.append(item);
            }

            QVariant v;
            v.setValue<ComboItems>(items);
            pProObj->setAttribute("items", v);
            pProObj->set_value(pFuncObjItem->szEvent_);
            listProperties.append(pProObj);
        }

        pProObj = QPropertyFactory::create_property("String");
        if(pProObj != Q_NULLPTR) {
            pProObj->setObjectProperty("name","text");
            pProObj->setAttribute("show_name", tr("函数名称"));
            pProObj->setAttribute(ATTR_EDITABLE, false); // 只读
            pProObj->setAttribute("group","Attributes");
            pProObj->setAttribute("type", "funcName");
            pProObj->setAttribute(ATTR_CAN_SAME,true);
            pProObj->set_value(pFuncObjItem->szName_);
            listProperties.append(pProObj);
        }

        foreach (TArgItem *pArgItem, pFuncObjItem->argList_) {
            if(pArgItem->type == "VALUE") {
                pProObj = QPropertyFactory::create_property("String");
                if(pProObj != Q_NULLPTR) {
                    pProObj->setObjectProperty("name","text");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute(ATTR_EDITABLE, true); // 只读
                    pProObj->setAttribute("group","Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME,true);
                    if(pArgItem->value == "")
                        pArgItem->value = "0";
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            } else if(pArgItem->type == "TAGLIST") {
                pProObj = QPropertyFactory::create_property("Tag");
                if(pProObj != Q_NULLPTR) {
                    tagNames_.clear();
                    QSoftCore::getCore()->getProjectCore()->getAllTagName(tagNames_);
                    pProObj->setObjectProperty("name", "tag");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute("group", "HMI");
                    pProObj->setAttribute(ATTR_CAN_SAME, true);
                    if(pArgItem->value == "")
                        pArgItem->value = tagNames_.at(0);
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            } else if(pArgItem->type == "GRAPHPAGELIST") {
                graphPageNames_.clear();
                QSoftCore::getCore()->getProjectCore()->getAllGraphPageName(graphPageNames_);

                pProObj = QPropertyFactory::create_property("Enum");
                if(pProObj != Q_NULLPTR) {
                    pProObj->setObjectProperty("name","graph");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute("group","Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME,true);

                    ComboItems items;
                    foreach(QString szName, graphPageNames_) {
                        tagComboItem item;
                        item.m_text = szName;
                        item.m_value = szName;
                        items.append(item);
                    }

                    QVariant v;
                    v.setValue<ComboItems>(items);
                    pProObj->setAttribute("items", v);

                    if(pArgItem->value == "")
                        pArgItem->value = graphPageNames_.at(0);
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            } else if(pArgItem->type == "ELEMENTIDLIST") {
                elementIds_.clear();
                QSoftCore::getCore()->getProjectCore()->getAllElementIDName(elementIds_);
                pProObj = QPropertyFactory::create_property("Enum");
                if(pProObj != Q_NULLPTR) {
                    pProObj->setObjectProperty("name","element");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute("group","Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME,true);

                    ComboItems items;
                    foreach(QString szEle, elementIds_) {
                        tagComboItem item;
                        item.m_text = szEle;
                        item.m_value = szEle;
                        items.append(item);
                    }

                    QVariant v;
                    v.setValue<ComboItems>(items);
                    pProObj->setAttribute("items", v);

                    if(pArgItem->value == "")
                        pArgItem->value = elementIds_.at(0);
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            }
        }
    }
    m_propertyView->setPropertys(listProperties);
}


void FunctionEditorDialog::onPropertyEdit(QAbstractProperty *pro, const QVariant &value)
{
    TFuncObjectItem *pFuncObjItem;
    pFuncObjItem = selectFuncObjItemList_.at(iSelectedCurRow_);

    QString szName = pro->getAttribute("show_name").toString();
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
    pro->set_value(value);
}
