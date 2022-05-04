#include "functioneditordialog.h"
#include "ui_functioneditordialog.h"
#include <QListWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include "switchgraphpageform.h"

FunctionEditorDialog::FunctionEditorDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::FunctionEditorDialog),
      m_selectedCurRow(-1)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    this->setWindowTitle(tr("功能操作编辑"));
    initUI();
}

FunctionEditorDialog::~FunctionEditorDialog()
{
    delete ui;

    QMapIterator<QString, QAbstractFunction *> iter(m_nameToFuncsUi);
    while (iter.hasNext()) {
        iter.next();
        QAbstractFunction *pObj = iter.value();
        QWidget*pWidgetObj = dynamic_cast<QWidget*>(pObj);
        delete pWidgetObj;
    }
    m_nameToFuncsUi.clear();
}

/**
 * @brief FunctionEditorDialog::initUI
 * @details 初始化ui
 */
void FunctionEditorDialog::initUI()
{
    m_nameToFuncsUi.clear();
    //空白
    ui->stackedWidget->addWidget(new QWidget);
    //切换画面
    QAbstractFunction *pObj = new SwitchGraphPageForm;
    m_nameToFuncsUi.insert(pObj->name(), pObj);
    ui->stackedWidget->addWidget(dynamic_cast<QWidget*>(pObj));

    initListWidget();
    initTableWidget();
}


/**
 * @brief FunctionEditorDialog::initListWidget
 * @details 初始化列表控件
 */
void FunctionEditorDialog::initListWidget()
{
    QMapIterator<QString, QAbstractFunction *> iter(m_nameToFuncsUi);
    while (iter.hasNext()) {
        iter.next();

        QAbstractFunction * pFuncObj = iter.value();
        bool found = false;
        QListWidget *pListWidget = NULL;
        for(int i = 0; i < ui->tabFuncSelect->count(); i++) {
            if(pFuncObj->group() == ui->tabFuncSelect->tabText(i)) {
                found = true;
                break;
            }
        }
        if(!found) {
            QTabWidget *pTab = ui->tabFuncSelect;
            pListWidget = new QListWidget(pTab);
            m_listWidgetList.append(pListWidget);
            connect(pListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*)));
            connect(pListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listItemDoubleClicked(QListWidgetItem*)));
            pTab->insertTab(pTab->count(), pListWidget, pFuncObj->group());
            pTab->setCurrentIndex(0);
            found = true;
        }

        if(found && pListWidget) {
            QListWidgetItem *pItem = new QListWidgetItem(pFuncObj->showName(), pListWidget);
            pItem->setData(Qt::UserRole, pFuncObj->name());
            pItem->setData(Qt::UserRole + 1, pFuncObj->showName());
            pItem->setData(Qt::UserRole + 2, pFuncObj->args());
            pItem->setData(Qt::UserRole + 3, pFuncObj->showArgs());
            pItem->setData(Qt::UserRole + 4, pFuncObj->description());
            pItem->setData(Qt::UserRole + 5, pFuncObj->toString());
            pItem->setData(Qt::UserRole + 6, pFuncObj->toShowString());
            pListWidget->addItem(pItem);
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
    headerLabels << tr("功能函数");
    ui->tableEventFunc->setColumnCount(headerLabels.count());
    ui->tableEventFunc->setHorizontalHeaderLabels(headerLabels);
    ui->tableEventFunc->horizontalHeader()->setSectionsClickable(false);
    ui->tableEventFunc->horizontalHeader()->setHighlightSections(false);
    ui->tableEventFunc->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableEventFunc->verticalHeader()->show();
    ui->tableEventFunc->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableEventFunc->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEventFunc->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableEventFunc->setWordWrap(false);
    ui->tableEventFunc->setColumnWidth(0, 430);
    this->m_funcs.clear();
    m_selectedCurRow = -1;
}


QStringList FunctionEditorDialog::getFunctions()
{
    m_funcs.clear();
    int iRowCount = ui->tableEventFunc->rowCount();
    for(int i = 0; i < iRowCount; i++) {
        QTableWidgetItem *pItemObj = ui->tableEventFunc->item(i, 0);
        if(pItemObj) {
            QString name = pItemObj->data(Qt::UserRole).toString();
            QAbstractFunction * pFuncObj = m_nameToFuncsUi[name];
            if(pFuncObj) {
                pFuncObj->fromString(pItemObj->data(Qt::UserRole + 5).toString());
                QWidget *pWidgetObj = dynamic_cast<QWidget*>(pFuncObj);
                ui->stackedWidget->setCurrentWidget(pWidgetObj);
                m_funcs.append(pFuncObj->toString());
            }
        }
    }
    return m_funcs;
}

void FunctionEditorDialog::setFunctions(const QStringList &funcs)
{
    if (m_funcs != funcs) {
        m_funcs = funcs;
        foreach (QString szFunc, m_funcs) {
            if(szFunc == "") {
                continue;
            }

            QString funcName = szFunc.left(szFunc.indexOf("("));
            QAbstractFunction * pFuncObj = m_nameToFuncsUi[funcName];
            if(pFuncObj) {
                if(pFuncObj->fromString(szFunc)) {
                    int iRowCount = ui->tableEventFunc->rowCount();
                    ui->tableEventFunc->insertRow(iRowCount);
                    QString showString = pFuncObj->toShowString();
                    QTableWidgetItem *pNewItemObj = new QTableWidgetItem(showString);
                    pNewItemObj->setData(Qt::UserRole, pFuncObj->name()); //name
                    pNewItemObj->setData(Qt::UserRole + 1, pFuncObj->showName()); //showName
                    pNewItemObj->setData(Qt::UserRole + 2, pFuncObj->args()); //args
                    pNewItemObj->setData(Qt::UserRole + 3, pFuncObj->showArgs()); //showArgs
                    pNewItemObj->setData(Qt::UserRole + 4, pFuncObj->description()); //description
                    pNewItemObj->setData(Qt::UserRole + 5, pFuncObj->toString()); //toString
                    pNewItemObj->setData(Qt::UserRole + 6, pFuncObj->toShowString()); //toShowString
                    ui->tableEventFunc->setItem(iRowCount, 0, pNewItemObj);
                }
            }
        }
        m_selectedCurRow = -1;
        if(ui->tableEventFunc->rowCount() > 0) {
            m_selectedCurRow = 0;
            ui->tableEventFunc->selectRow(m_selectedCurRow);
        }
    }
    ui->stackedWidget->setCurrentIndex(0);
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
    ui->stackedWidget->setCurrentIndex(0);
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
        QTableWidgetItem *pItemCurFuncObj = ui->tableEventFunc->item(iCurRow, 0);
        QString szCurFuncNameOrg = pItemCurFuncObj->data(Qt::UserRole).toString();
        QAbstractFunction * pCurFuncObj = m_nameToFuncsUi[szCurFuncNameOrg];

        QTableWidgetItem *pItemUpFuncObj = ui->tableEventFunc->item(iCurRow - 1, 0);
        QString szUpFuncNameOrg = pItemUpFuncObj->text();
        QAbstractFunction * pUpFuncObj = m_nameToFuncsUi[szUpFuncNameOrg];

        if(pItemCurFuncObj && pUpFuncObj) {
            pItemCurFuncObj->setData(Qt::UserRole, pUpFuncObj->name()); //name
            pItemCurFuncObj->setData(Qt::UserRole + 1, pUpFuncObj->showName()); //showName
            pItemCurFuncObj->setData(Qt::UserRole + 2, pUpFuncObj->args()); //args
            pItemCurFuncObj->setData(Qt::UserRole + 3, pUpFuncObj->showArgs()); //showArgs
            pItemCurFuncObj->setData(Qt::UserRole + 4, pUpFuncObj->description()); //description
            pItemCurFuncObj->setData(Qt::UserRole + 5, pUpFuncObj->toString()); //toString
            pItemCurFuncObj->setData(Qt::UserRole + 6, pUpFuncObj->toShowString()); //toShowString
            pItemCurFuncObj->setText(pUpFuncObj->toShowString());
        }

        if(pItemUpFuncObj && pCurFuncObj) {
            pItemUpFuncObj->setData(Qt::UserRole, pCurFuncObj->name()); //name
            pItemUpFuncObj->setData(Qt::UserRole + 1, pCurFuncObj->showName()); //showName
            pItemUpFuncObj->setData(Qt::UserRole + 2, pCurFuncObj->args()); //args
            pItemUpFuncObj->setData(Qt::UserRole + 3, pCurFuncObj->showArgs()); //showArgs
            pItemUpFuncObj->setData(Qt::UserRole + 4, pCurFuncObj->description()); //description
            pItemUpFuncObj->setData(Qt::UserRole + 5, pCurFuncObj->toString()); //toString
            pItemUpFuncObj->setData(Qt::UserRole + 6, pCurFuncObj->toShowString()); //toShowString
            pItemUpFuncObj->setText(pCurFuncObj->toShowString());
        }
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
        QTableWidgetItem *pItemCurFuncObj = ui->tableEventFunc->item(iCurRow, 0);
        QString szCurFuncNameOrg = pItemCurFuncObj->text();
        QAbstractFunction * pCurFuncObj = m_nameToFuncsUi[szCurFuncNameOrg];

        QTableWidgetItem *pItemDownFuncObj = ui->tableEventFunc->item(iCurRow + 1, 0);
        QString szDownFuncNameOrg = pItemDownFuncObj->text();
        QAbstractFunction * pDownFuncObj = m_nameToFuncsUi[szDownFuncNameOrg];

        if(pItemCurFuncObj && pDownFuncObj) {
            pItemCurFuncObj->setData(Qt::UserRole, pDownFuncObj->name()); //name
            pItemCurFuncObj->setData(Qt::UserRole + 1, pDownFuncObj->showName()); //showName
            pItemCurFuncObj->setData(Qt::UserRole + 2, pDownFuncObj->args()); //args
            pItemCurFuncObj->setData(Qt::UserRole + 3, pDownFuncObj->showArgs()); //showArgs
            pItemCurFuncObj->setData(Qt::UserRole + 4, pDownFuncObj->description()); //description
            pItemCurFuncObj->setData(Qt::UserRole + 5, pDownFuncObj->toString()); //toString
            pItemCurFuncObj->setData(Qt::UserRole + 6, pDownFuncObj->toShowString()); //toShowString
            pItemCurFuncObj->setText(pDownFuncObj->toShowString());
        }

        if(pItemDownFuncObj && pCurFuncObj) {
            pItemDownFuncObj->setData(Qt::UserRole, pCurFuncObj->name()); //name
            pItemDownFuncObj->setData(Qt::UserRole + 1, pCurFuncObj->showName()); //showName
            pItemDownFuncObj->setData(Qt::UserRole + 2, pCurFuncObj->args()); //args
            pItemDownFuncObj->setData(Qt::UserRole + 3, pCurFuncObj->showArgs()); //showArgs
            pItemDownFuncObj->setData(Qt::UserRole + 4, pCurFuncObj->description()); //description
            pItemDownFuncObj->setData(Qt::UserRole + 5, pCurFuncObj->toString()); //toString
            pItemDownFuncObj->setData(Qt::UserRole + 6, pCurFuncObj->toShowString()); //toShowString
            pItemDownFuncObj->setText(pCurFuncObj->toShowString());
        }
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
void FunctionEditorDialog::listItemClicked(QListWidgetItem *pItemObj)
{
    if(pItemObj) {
        QString desc = pItemObj->data(Qt::UserRole + 4).toString();
        ui->plainTextFuncDesc->setPlainText(desc);
    }
}

/**
 * @brief FunctionEditorDialog::listItemDoubleClicked
 * @details 列表项函数被双击
 * @param item 列表项
 */
void FunctionEditorDialog::listItemDoubleClicked(QListWidgetItem *pItemObj)
{
    if(pItemObj) {
        int iRowCount = ui->tableEventFunc->rowCount();
        ui->tableEventFunc->insertRow(iRowCount);
        QString showString = pItemObj->data(Qt::UserRole + 6).toString();
        QTableWidgetItem *pNewItemObj = new QTableWidgetItem(showString);
        pNewItemObj->setData(Qt::UserRole, pItemObj->data(Qt::UserRole)); //name
        pNewItemObj->setData(Qt::UserRole + 1, pItemObj->data(Qt::UserRole + 1)); //showName
        pNewItemObj->setData(Qt::UserRole + 2, pItemObj->data(Qt::UserRole + 2)); //args
        pNewItemObj->setData(Qt::UserRole + 3, pItemObj->data(Qt::UserRole + 3)); //showArgs
        pNewItemObj->setData(Qt::UserRole + 4, pItemObj->data(Qt::UserRole + 4)); //description
        pNewItemObj->setData(Qt::UserRole + 5, pItemObj->data(Qt::UserRole + 5)); //toString
        pNewItemObj->setData(Qt::UserRole + 6, pItemObj->data(Qt::UserRole + 6)); //toShowString
        ui->tableEventFunc->setItem(iRowCount, 0, pNewItemObj);
    }
}

void FunctionEditorDialog::on_tableEventFunc_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    QTableWidgetItem *pLastItemObj = ui->tableEventFunc->item(m_selectedCurRow, 0);
    if(pLastItemObj) {
        QString name = pLastItemObj->data(Qt::UserRole).toString();
        QAbstractFunction * pFuncObj = m_nameToFuncsUi[name];
        if(pFuncObj) {
            pLastItemObj->setData(Qt::UserRole, pFuncObj->name()); //name
            pLastItemObj->setData(Qt::UserRole + 1, pFuncObj->showName()); //showName
            pLastItemObj->setData(Qt::UserRole + 2, pFuncObj->args()); //args
            pLastItemObj->setData(Qt::UserRole + 3, pFuncObj->showArgs()); //showArgs
            pLastItemObj->setData(Qt::UserRole + 4, pFuncObj->description()); //description
            pLastItemObj->setData(Qt::UserRole + 5, pFuncObj->toString()); //toString
            pLastItemObj->setData(Qt::UserRole + 6, pFuncObj->toShowString()); //toShowString
            pLastItemObj->setText(pFuncObj->toShowString());
        }
    }

    QTableWidgetItem *pItemObj = ui->tableEventFunc->currentItem();
    if(pItemObj) {
        QString name = pItemObj->data(Qt::UserRole).toString();
        QAbstractFunction * pFuncObj = m_nameToFuncsUi[name];
        if(pFuncObj) {
            if(pFuncObj->fromString(pItemObj->data(Qt::UserRole + 5).toString())) {
                QWidget *pWidgetObj = dynamic_cast<QWidget*>(pFuncObj);
                ui->stackedWidget->setCurrentWidget(pWidgetObj);
            }
        }
    }

    m_selectedCurRow = ui->tableEventFunc->currentRow();
}

