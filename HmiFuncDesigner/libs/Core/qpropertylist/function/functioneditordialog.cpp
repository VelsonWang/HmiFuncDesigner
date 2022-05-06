#include "functioneditordialog.h"
#include "ui_functioneditordialog.h"
#include <QListWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include "switchgraphpageform.h"
#include "returngraphpageform.h"

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
    QAbstractFunction *pObj = NULL;
    m_nameToFuncsUi.clear();
    //空白
    ui->stackedWidget->addWidget(new QWidget);
    //切换画面
    pObj = new SwitchGraphPageForm;
    m_nameToFuncsUi.insert(pObj->name(), pObj);
    ui->stackedWidget->addWidget(dynamic_cast<QWidget*>(pObj));
    //返回画面
    pObj = new ReturnGraphPage;
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
                pListWidget = dynamic_cast<QListWidget*>(ui->tabFuncSelect->widget(i));
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
        QString szCurFuncName = pItemCurFuncObj->data(Qt::UserRole).toString();
        QAbstractFunction * pCurFuncObj = m_nameToFuncsUi[szCurFuncName];
        QStringList values;
        values << pCurFuncObj->name()
               << pCurFuncObj->showName()
               << pCurFuncObj->args()
               << pCurFuncObj->showArgs()
               << pCurFuncObj->description()
               << pCurFuncObj->toString()
               << pCurFuncObj->toShowString();

        QTableWidgetItem *pItemUpFuncObj = ui->tableEventFunc->item(iCurRow - 1, 0);
        QString szUpFuncName = pItemUpFuncObj->data(Qt::UserRole).toString();
        QAbstractFunction * pUpFuncObj = m_nameToFuncsUi[szUpFuncName];
        QStringList values2;
        values2 << pUpFuncObj->name()
                << pUpFuncObj->showName()
                << pUpFuncObj->args()
                << pUpFuncObj->showArgs()
                << pUpFuncObj->description()
                << pUpFuncObj->toString()
                << pUpFuncObj->toShowString();

        if(pItemCurFuncObj && pUpFuncObj) {
            for(int i = 0; i < values2.size(); i++) {
                pItemCurFuncObj->setData(Qt::UserRole + i, values2[i]);
            }
            pItemCurFuncObj->setText(values2.last());
        }
        if(pItemUpFuncObj && pCurFuncObj) {
            for(int i = 0; i < values.size(); i++) {
                pItemUpFuncObj->setData(Qt::UserRole + i, values[i]);
            }
            pItemUpFuncObj->setText(values.last());
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
        QString szCurFuncName = pItemCurFuncObj->data(Qt::UserRole).toString();
        QAbstractFunction * pCurFuncObj = m_nameToFuncsUi[szCurFuncName];
        QStringList values;
        values << pCurFuncObj->name()
               << pCurFuncObj->showName()
               << pCurFuncObj->args()
               << pCurFuncObj->showArgs()
               << pCurFuncObj->description()
               << pCurFuncObj->toString()
               << pCurFuncObj->toShowString();

        QTableWidgetItem *pItemDownFuncObj = ui->tableEventFunc->item(iCurRow + 1, 0);
        QString szDownFuncName = pItemDownFuncObj->data(Qt::UserRole).toString();
        QAbstractFunction * pDownFuncObj = m_nameToFuncsUi[szDownFuncName];
        QStringList values2;
        values2 << pDownFuncObj->name()
                << pDownFuncObj->showName()
                << pDownFuncObj->args()
                << pDownFuncObj->showArgs()
                << pDownFuncObj->description()
                << pDownFuncObj->toString()
                << pDownFuncObj->toShowString();

        if(pItemCurFuncObj && pDownFuncObj) {
            for(int i = 0; i < values2.size(); i++) {
                pItemCurFuncObj->setData(Qt::UserRole + i, values2[i]);
            }
            pItemCurFuncObj->setText(values2.last());
        }
        if(pItemDownFuncObj && pCurFuncObj) {
            for(int i = 0; i < values.size(); i++) {
                pItemDownFuncObj->setData(Qt::UserRole + i, values[i]);
            }
            pItemDownFuncObj->setText(values.last());
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
        QString desc = pItemObj->data(Qt::UserRole + 4).toString();
        ui->plainTextFuncDesc->setPlainText(desc);
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

