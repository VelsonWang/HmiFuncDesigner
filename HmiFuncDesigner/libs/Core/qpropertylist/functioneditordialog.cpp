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
      m_selectedCurRow(-1)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    this->setWindowTitle(tr("功能操作编辑"));

    m_nameToShowName.clear();
    m_showNameToName.clear();
    m_supportEvents.clear();

    for(int i = 0; i < events.size(); i++) {
        QString szNameToShowName = events.at(i);
        QStringList listNameToShowName = szNameToShowName.split('-');
        if ( listNameToShowName.size() == 2 ) {
            m_supportEvents << listNameToShowName.at(1);
            m_nameToShowName[listNameToShowName.at(0)] = listNameToShowName.at(1);
            m_showNameToName[listNameToShowName.at(1)] = listNameToShowName.at(0);
        }
    }

    m_selectFuncObjItemList.clear();
    m_funcObjItemList.clear();

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

    for(int i = 0; i < m_funcObjItemList.count(); i++) {
        qDeleteAll(m_funcObjItemList.at(i)->m_argList);
        m_funcObjItemList.at(i)->m_argList.clear();
    }
    qDeleteAll(m_funcObjItemList);
    m_funcObjItemList.clear();

    for(int i = 0; i < m_selectFuncObjItemList.count(); i++) {
        qDeleteAll(m_selectFuncObjItemList.at(i)->m_argList);
        m_selectFuncObjItemList.at(i)->m_argList.clear();
    }
    qDeleteAll(m_selectFuncObjItemList);
    m_selectFuncObjItemList.clear();

    if(m_propertyView) {
        delete m_propertyView;
        m_propertyView = NULL;
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
    if(!xmlFuncSupportList.load(buffer, NULL)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* funcGroup, childrenFuncSupport) {
        QString szFuncGroupName = funcGroup->getProperty("funcName");
        QTabWidget *pTab = ui->tabFuncSelect;
        QListWidget *pListWidget = new QListWidget(pTab);
        m_listWidgetList.append(pListWidget);
        connect(pListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(listItemClicked(QListWidgetItem*)));
        connect(pListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(listItemDoubleClicked(QListWidgetItem*)));
        pTab->insertTab(pTab->count(), pListWidget, szFuncGroupName);
        pTab->setCurrentIndex(0);

        // add child
        QList<XMLObject*> childrenGroup = funcGroup->getChildren();
        if(childrenGroup.size() < 1) {
            continue;
        }
        foreach(XMLObject* func, childrenGroup) {
            TFuncObjectItem *pFuncObjItem = new TFuncObjectItem();
            QString funcName = func->getProperty("name");
            pFuncObjItem->m_name = funcName;
            QListWidgetItem *pItem = new QListWidgetItem(funcName, pListWidget);
            pListWidget->addItem(pItem);

            QList<XMLObject*> funcDesc = func->getChildren();
            if(funcDesc.size() < 1) {
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
            if(argsXMLObject != NULL) {
                QString strFuncArgs = argsXMLObject->getProperty("name");
                strDesc += strFuncArgs;
                strDesc += QString("\n");

                foreach(XMLObject* arg, argsXMLObject->getChildren()) {
                    QString strArg = arg->getText();
                    TArgItem *pArgItem = new TArgItem();
                    pArgItem->name = arg->getProperty("name");
                    pArgItem->type = arg->getProperty("type").toUpper();
                    pFuncObjItem->m_argList.append(pArgItem);
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

            if(m_supportEvents.count() > 0) {
                pFuncObjItem->m_event = m_supportEvents.at(0);
            }
            pFuncObjItem->m_desc = strDesc;
            QString strFuncOrg = strFuncOrgName.right(strFuncOrgName.length() - strFuncOrgName.indexOf(":") - 1);
            pFuncObjItem->m_funcNameOrg = strFuncOrg;
            m_funcObjItemList.append(pFuncObjItem);
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
    m_funcs.clear();
    int iRowCount = ui->tableEventFunc->rowCount();
    for(int i = 0; i < iRowCount; i++) {
        QTableWidgetItem *pItemFuncName = ui->tableEventFunc->item(i, 0);
        QTableWidgetItem *pItemFuncEvent = ui->tableEventFunc->item(i, 1);
        QString szFuncName = pItemFuncName->text();
        QString szFuncEvent = m_showNameToName[pItemFuncEvent->text()];
        QString szRowFuncEvent = szFuncName + ":" + szFuncEvent;
        m_funcs.append(szRowFuncEvent);
    }
    return m_funcs;
}

void FunctionEditorDialog::setFunctions(const QStringList &funcs)
{
    if (m_funcs != funcs) {
        m_selectFuncObjItemList.clear();
        m_funcs = funcs;
        foreach (QString szFuncEvent, m_funcs) {
            if(szFuncEvent == "") {
                continue;
            }
            TFuncObjectItem *pNewFuncObj = new TFuncObjectItem();
            pNewFuncObj->setFuncString(szFuncEvent);
            pNewFuncObj->m_event = m_nameToShowName[pNewFuncObj->m_event];
            TFuncObjectItem *pFuncObjectItem = getFuncObjectItem(pNewFuncObj->m_name);
            if(pFuncObjectItem == NULL) {
                continue;
            }
            int count = pNewFuncObj->m_argList.count();
            for(int i = 0; i < count; i++) {
                ArgItem *pArgItem = pNewFuncObj->m_argList.at(i);
                pArgItem->type = pFuncObjectItem->m_argList.at(i)->type;
                pArgItem->name = pFuncObjectItem->m_argList.at(i)->name;
            }
            pNewFuncObj->m_desc = pFuncObjectItem->m_desc;
            pNewFuncObj->m_funcNameOrg = pFuncObjectItem->m_funcNameOrg;
            m_selectFuncObjItemList.append(pNewFuncObj);

            int iRowCount = ui->tableEventFunc->rowCount();
            ui->tableEventFunc->insertRow(iRowCount);
            QTableWidgetItem *itemFuncName = new QTableWidgetItem(pNewFuncObj->getFuncString());
            ui->tableEventFunc->setItem(iRowCount, 0, itemFuncName);
            QTableWidgetItem *itemFuncEvent = new QTableWidgetItem(pNewFuncObj->m_event);
            ui->tableEventFunc->setItem(iRowCount, 1, itemFuncEvent);
        }
    }
}

void FunctionEditorDialog::on_btnAdd_clicked()
{
    foreach (QListWidget *pListWidget, m_listWidgetList) {
        QListWidgetItem *pCurItem = pListWidget->currentItem();
        if(pCurItem != NULL) {
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
    if(pItem == NULL) {
        return;
    }
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

        QTableWidgetItem *pItemUpFuncName = ui->tableEventFunc->item(iCurRow - 1, 0);
        QTableWidgetItem *pItemUpFuncEvent = ui->tableEventFunc->item(iCurRow - 1, 1);
        QString szUpFuncNameOrg = pItemUpFuncName->text();
        QString szUpFuncEvent = pItemUpFuncEvent->text();

        pItemCurFuncName->setText(szUpFuncNameOrg);
        pItemCurFuncEvent->setText(szUpFuncEvent);

        pItemUpFuncName->setText(szCurFuncNameOrg);
        pItemUpFuncEvent->setText(szCurFuncEvent);

        ui->tableEventFunc->selectRow(iCurRow - 1);
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

    if(iRowCount > 1 && iCurRow < (iRowCount - 1)) {
        QTableWidgetItem *pItemCurFuncName = ui->tableEventFunc->item(iCurRow, 0);
        QTableWidgetItem *pItemCurFuncEvent = ui->tableEventFunc->item(iCurRow, 1);
        QString szCurFuncNameOrg = pItemCurFuncName->text();
        QString szCurFuncEvent = pItemCurFuncEvent->text();

        QTableWidgetItem *pItemDownFuncName = ui->tableEventFunc->item(iCurRow + 1, 0);
        QTableWidgetItem *pItemDownFuncEvent = ui->tableEventFunc->item(iCurRow + 1, 1);
        QString szDownFuncNameOrg = pItemDownFuncName->text();
        QString szDownFuncEvent = pItemDownFuncEvent->text();

        pItemCurFuncName->setText(szDownFuncNameOrg);
        pItemCurFuncEvent->setText(szDownFuncEvent);

        pItemDownFuncName->setText(szCurFuncNameOrg);
        pItemDownFuncEvent->setText(szCurFuncEvent);

        ui->tableEventFunc->selectRow(iCurRow + 1);
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
    if(pFuncObjectItem != NULL) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjectItem->m_desc);
        m_selectedFuncName = pFuncObjectItem->m_name;
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
    QString szFuncNameOrg = pFuncObjectItem->m_funcNameOrg;
    QTableWidgetItem *itemFuncName = new QTableWidgetItem(szFuncNameOrg);
    ui->tableEventFunc->setItem(iRowCount, 0, itemFuncName);

    QString szFuncEvent = pFuncObjectItem->m_event;
    QTableWidgetItem *itemFuncEvent = new QTableWidgetItem(szFuncEvent);
    ui->tableEventFunc->setItem(iRowCount, 1, itemFuncEvent);

    m_selectedFuncName = pFuncObjectItem->m_name;

    TFuncObjectItem *pNewFuncObj = new TFuncObjectItem();
    pNewFuncObj->m_name = pFuncObjectItem->m_name;
    pNewFuncObj->m_event = pFuncObjectItem->m_event;
    int count = pFuncObjectItem->m_argList.count();
    for(int i = 0; i < count; i++) {
        ArgItem *pArgItem = new ArgItem();
        pArgItem->type = pFuncObjectItem->m_argList.at(i)->type;
        pArgItem->name = pFuncObjectItem->m_argList.at(i)->name;
        pArgItem->value = pFuncObjectItem->m_argList.at(i)->value;
        pNewFuncObj->m_argList.append(pArgItem);
    }
    pNewFuncObj->m_desc = pFuncObjectItem->m_desc;
    pNewFuncObj->m_funcNameOrg = pFuncObjectItem->m_funcNameOrg;
    m_selectFuncObjItemList.append(pNewFuncObj);
}


/**
 * @brief FunctionEditorDialog::getFuncObjectItem
 * @details 查找函数数据对象
 * @param name 函数名称
 * @return 函数数据对象
 */
TFuncObjectItem *FunctionEditorDialog::getFuncObjectItem(const QString &name)
{
    for(int i = 0; i < m_funcObjItemList.count(); i++) {
        TFuncObjectItem *pFuncObjItem = m_funcObjItemList.at(i);
        if(pFuncObjItem->m_name == name) {
            return pFuncObjItem;
        }
    }
    return NULL;
}

/**
 * @brief FunctionEditorDialog::setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void FunctionEditorDialog::setSupportEvents(QStringList events)
{
    m_supportEvents = events;
}


void FunctionEditorDialog::on_tableEventFunc_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    m_selectedCurRow = ui->tableEventFunc->currentRow();
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == NULL) {
        return;
    }
    QString szFuncNameOrg = ui->tableEventFunc->item(m_selectedCurRow, 0)->text();
    TFuncObjectItem *pFuncObjItem = m_selectFuncObjItemList.at(m_selectedCurRow);
    if(szFuncNameOrg.indexOf(pFuncObjItem->m_name) > -1) {
        ui->plainTextFuncDesc->setPlainText(pFuncObjItem->m_desc);
        m_selectedFuncName = pFuncObjItem->m_name;
        updatePropertyEditor();
    }
}

void FunctionEditorDialog::updatePropertyEditor()
{
    QTableWidgetItem *pItem = ui->tableEventFunc->currentItem();
    if(pItem == NULL) {
        return;
    }

    QList<QAbstractProperty *> listProperties;
    QAbstractProperty* pProObj = NULL;

    QTableWidgetItem *pItemNeed = ui->tableEventFunc->item(pItem->row(), 0);
    QString szFuncNameOrg = pItemNeed->text();
    TFuncObjectItem *pFuncObjItem = m_selectFuncObjItemList.at(pItem->row());
    if(szFuncNameOrg.indexOf(pFuncObjItem->m_name) > -1) {
        pProObj = QPropertyFactory::create_property("Enum");
        if(pProObj != NULL) {
            pProObj->setObjectProperty("name", "eventType");
            pProObj->setAttribute("show_name", tr("事件类型"));
            pProObj->setAttribute("group", "Attributes");
            pProObj->setAttribute(ATTR_CAN_SAME, true);

            ComboItems items;
            foreach(QString szEv, m_supportEvents) {
                tagComboItem item;
                item.m_text = szEv;
                item.m_value = szEv;
                items.append(item);
            }

            QVariant v;
            v.setValue<ComboItems>(items);
            pProObj->setAttribute("items", v);
            pProObj->set_value(pFuncObjItem->m_event);
            listProperties.append(pProObj);
        }

        pProObj = QPropertyFactory::create_property("String");
        if(pProObj != NULL) {
            pProObj->setObjectProperty("name", "text");
            pProObj->setAttribute("show_name", tr("函数名称"));
            pProObj->setAttribute(ATTR_EDITABLE, false); // 只读
            pProObj->setAttribute("group", "Attributes");
            pProObj->setAttribute("type", "funcName");
            pProObj->setAttribute(ATTR_CAN_SAME, true);
            pProObj->set_value(pFuncObjItem->m_name);
            listProperties.append(pProObj);
        }

        foreach (TArgItem *pArgItem, pFuncObjItem->m_argList) {
            if(pArgItem->type == "VALUE") {
                pProObj = QPropertyFactory::create_property("String");
                if(pProObj != NULL) {
                    pProObj->setObjectProperty("name", "text");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute(ATTR_EDITABLE, true);
                    pProObj->setAttribute("group", "Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME, true);
                    if(pArgItem->value == "") {
                        pArgItem->value = "0";
                    }
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            } else if(pArgItem->type == "TAGLIST") {
                pProObj = QPropertyFactory::create_property("Tag");
                if(pProObj != NULL) {
                    m_tagNames.clear();
                    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_tagNames);
                    pProObj->setObjectProperty("name", "tag");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute("group", "Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME, true);

                    ComboItems items;
                    foreach(QString szTagName, m_tagNames) {
                        tagComboItem item;
                        item.m_text = szTagName;
                        item.m_value = szTagName;
                        items.append(item);
                    }
                    QVariant v;
                    v.setValue<ComboItems>(items);
                    pProObj->setAttribute("items", v);

                    if(pArgItem->value == "" && m_tagNames.size() > 0) {
                        pArgItem->value = m_tagNames.at(0);
                    }
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            } else if(pArgItem->type == "GRAPHPAGELIST") {
                m_graphPageNames.clear();
                QSoftCore::getCore()->getProjectCore()->getAllGraphPageName(m_graphPageNames);

                pProObj = QPropertyFactory::create_property("Enum");
                if(pProObj != NULL) {
                    pProObj->setObjectProperty("name", "graph");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute("group", "Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME, true);

                    ComboItems items;
                    foreach(QString szName, m_graphPageNames) {
                        tagComboItem item;
                        item.m_text = szName;
                        item.m_value = szName;
                        items.append(item);
                    }

                    QVariant v;
                    v.setValue<ComboItems>(items);
                    pProObj->setAttribute("items", v);

                    if(pArgItem->value == "" && m_graphPageNames.size() > 0) {
                        pArgItem->value = m_graphPageNames.at(0);
                    }
                    pProObj->set_value(pArgItem->value);
                    listProperties.append(pProObj);
                }
            } else if(pArgItem->type == "ELEMENTIDLIST") {
                m_elementIds.clear();
                QSoftCore::getCore()->getProjectCore()->getAllElementIDName(m_elementIds);
                pProObj = QPropertyFactory::create_property("Enum");
                if(pProObj != NULL) {
                    pProObj->setObjectProperty("name", "element");
                    pProObj->setAttribute("show_name", pArgItem->name);
                    pProObj->setAttribute("group", "Attributes");
                    pProObj->setAttribute(ATTR_CAN_SAME, true);

                    ComboItems items;
                    foreach(QString szEle, m_elementIds) {
                        tagComboItem item;
                        item.m_text = szEle;
                        item.m_value = szEle;
                        items.append(item);
                    }

                    QVariant v;
                    v.setValue<ComboItems>(items);
                    pProObj->setAttribute("items", v);

                    if(pArgItem->value == "" && m_elementIds.size() > 0) {
                        pArgItem->value = m_elementIds.at(0);
                    }
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
    pFuncObjItem = m_selectFuncObjItemList.at(m_selectedCurRow);

    QString szName = pro->getAttribute("show_name").toString();
    foreach (TArgItem *pArgItem, pFuncObjItem->m_argList) {
        if(pArgItem->name == szName) {
            pArgItem->value = value.toString();
            break;
        }
    }

    if(szName == tr("事件类型")) {
        pFuncObjItem->m_event = value.toString();
    }

    QString szFunc = pFuncObjItem->getFuncString();
    ui->tableEventFunc->item(m_selectedCurRow, 0)->setText(szFunc);
    ui->tableEventFunc->item(m_selectedCurRow, 1)->setText(pFuncObjItem->m_event);
    pro->set_value(value);
}
