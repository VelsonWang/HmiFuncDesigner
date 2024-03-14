#include "qformlistwidget.h"
#include "qdesignerformhost.h"
#include "../../../libs/shared/host/qabstracthost.h"
#include "../../../libs/shared/property/qabstractproperty.h"
#include "../../../libs/shared/qcommonstruct.h"
#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include <QVBoxLayout>
#include <QVariant>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QDebug>

QFormListWidget::QFormListWidget(QWidget *parent):
    QWidget(parent),
    m_current_form(NULL)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);

    QVBoxLayout *l = new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    this->setLayout(l);
    this->setFocusPolicy(Qt::StrongFocus);

    QSoftCore *core = QSoftCore::getCore();
    QAction *pActObj;
    pActObj = core->getAction("FormManager.Copy");
    if(pActObj != NULL) {
        pActObj->setShortcut(QKeySequence::Copy);
        m_form_action_list.append(pActObj);
    }

    pActObj = core->getAction("FormManager.Cut");
    if(pActObj != NULL) {
        pActObj->setShortcut(QKeySequence::Cut);
        m_form_action_list.append(pActObj);
    }

    pActObj = core->getAction("FormManager.Paste");
    if(pActObj != NULL) {
        pActObj->setShortcut(QKeySequence::Paste);
        m_form_action_list.append(pActObj);
    }

    pActObj = core->getAction("FormManager.Delete");
    if(pActObj != NULL) {
        m_form_action_list.append(pActObj);
        pActObj->setShortcut(QKeySequence::Delete);
    }

    pActObj = core->getAction("FormManager.Select.All");
    if(pActObj != NULL) {
        m_form_action_list.append(pActObj);
        pActObj->setShortcut(QKeySequence::SelectAll);
    }

    QPageManager *manager = QSoftCore::getCore()->getProjectCore()->getPageManager();
    connect(manager, SIGNAL(insert_page_signal(QAbstractHost*)),
            this, SLOT(slotInsertPage(QAbstractHost*)));
    connect(manager, SIGNAL(remove_page_signal(QAbstractHost*)),
            this, SLOT(slotRemovePage(QAbstractHost*)));
}

QFormListWidget::~QFormListWidget()
{
    clear();
}

void QFormListWidget::insertForm(QAbstractHost *host, int index)
{
    if(!host || m_host_to_form.keys().contains(host) || !host->getObject()->isWidgetType()) {
        return;
    }

    if(index < 0 || index > m_forms.size()) {
        index = m_forms.size();
    }
    QDesignerFormHost *form = new QDesignerFormHost(host, this);
    this->installEventFilter(form);
    connect(form, SIGNAL(select(QAbstractHost*)), this, SIGNAL(select(QAbstractHost*)));
    connect(form, SIGNAL(select(QAbstractHost*)), this, SLOT(onSelect(QAbstractHost*)));
    connect(form, SIGNAL(remove(QList<QAbstractHost*>)), this, SIGNAL(remove(QList<QAbstractHost*>)));
    form->selectHostWidget(host);
    form->setUndoStack(m_undo_stack);
    m_forms.insert(index, form);
    m_host_to_form.insert(host, form);

    selectHost(host);
    select(host);
}

void QFormListWidget::removeForm(QAbstractHost *host)
{
    if(host == NULL || !m_host_to_form.keys().contains(host)) {
        return;
    }
    if(m_current_form != NULL && m_current_form->get_root_host() == host) {
        if(m_forms.size() < 2) {
            m_current_form = NULL;
            select(NULL);
        } else {
            if(m_forms.first() == m_current_form) {
                selectHost(m_forms.at(1)->get_root_host());
                select(m_forms.at(1)->get_root_host());
            } else {
                selectHost(m_forms.at(0)->get_root_host());
                select(m_forms.at(0)->get_root_host());
            }
        }
    }
    QDesignerFormHost *form = m_host_to_form.value(host);
    m_host_to_form.remove(host);
    if(form != NULL) {
        m_forms.removeAll(form);
        delete form;
    }
}

void QFormListWidget::showForm(QAbstractHost *host)
{

    QDesignerFormHost *form = m_host_to_form.value(host);
    if(m_current_form == form) {
        return;
    }
    if(m_current_form != NULL) {
        m_current_form->hide();
    }
    m_current_form = form;
    if(m_current_form != NULL) {
        m_current_form->show();
    }
}

void QFormListWidget::clear()
{
    qDeleteAll(m_forms);
    m_forms.clear();
    m_host_to_form.clear();
    m_current_form = NULL;
}


void QFormListWidget::slotPropertyEdit(QAbstractProperty *pro, const QVariant &value)
{
    if(m_current_form != NULL) {
        m_current_form->propertyEdited(pro, value);
    }
}

void QFormListWidget::setUndoStack(QUndoStack *stack)
{
    m_undo_stack = stack;
}

void QFormListWidget::sameLeft()
{
    if(m_current_form != NULL) {
        m_current_form->sameLeft();
    }
}

void QFormListWidget::sameTop()
{
    if(m_current_form != NULL) {
        m_current_form->sameTop();
    }
}


void QFormListWidget::sameRight()
{
    if(m_current_form != NULL) {
        m_current_form->sameRight();
    }
}


void QFormListWidget::sameBottom()
{
    if(m_current_form != NULL) {
        m_current_form->sameBottom();
    }
}


void QFormListWidget::sameWidth()
{
    if(m_current_form != NULL) {
        m_current_form->sameWidth();
    }
}


void QFormListWidget::sameHeight()
{
    if(m_current_form != NULL) {
        m_current_form->sameHeight();
    }
}


void QFormListWidget::sameGeometry()
{
    if(m_current_form != NULL) {
        m_current_form->sameGeometry();
    }
}


void QFormListWidget::sameVCenter()
{
    if(m_current_form != NULL) {
        m_current_form->sameVCenter();
    }
}


void QFormListWidget::sameHCenter()
{
    if(m_current_form != NULL) {
        m_current_form->sameHCenter();
    }
}

void QFormListWidget::horizontalUniformDistribution()
{
    if(m_current_form != NULL) {
        m_current_form->horizontalUniformDistribution();
    }
}

void QFormListWidget::verticalUniformDistribution()
{
    if(m_current_form != NULL) {
        m_current_form->verticalUniformDistribution();
    }
}

void QFormListWidget::redo()
{
    if(m_undo_stack) {
        m_undo_stack->redo();
    }
}

void QFormListWidget::undo()
{
    if(m_undo_stack) {
        m_undo_stack->undo();
    }
}

void QFormListWidget::selectHost(QAbstractHost *host)
{
    QAbstractHost *par = host;
    while(par->getParent() != NULL) {
        par = par->getParent();
    }

    if(m_current_form != NULL) {
        if(m_current_form->get_root_host() == par) {
            m_current_form->selectHostWidget(host);
        } else {
            showForm(par);
            if(m_current_form != NULL) {
                m_current_form->selectHostWidget(host);
            }
        }
    } else {
        showForm(par);
        if(m_current_form != NULL) {
            m_current_form->selectHostWidget(host);
        }
    }
}

void QFormListWidget::slotInsertPage(QAbstractHost *host)
{
    QPageManager *manager = QSoftCore::getCore()->getProjectCore()->getPageManager();
    int index = manager->getPages().indexOf(host);
    insertForm(host, index);
}

void QFormListWidget::slotRemovePage(QAbstractHost *host)
{
    removeForm(host);
}

void QFormListWidget::onSelect(QAbstractHost* host)
{
    if(host != NULL) {
        // 更新变量编辑器
        QList<QAbstractProperty*> listProps = host->getPropertys();
        foreach(QAbstractProperty* pPropObj, listProps) {
            QString type = pPropObj->getObjectProperty("type").toString().toLower();
            if(type == "tag") {
                QProjectCore* pPorjCore = QSoftCore::getCore()->getProjectCore();
                QStringList szListTagName;
                pPorjCore->getAllTagName(szListTagName, "ALL");

                ComboItems items;
                foreach(QString szTagName, szListTagName) {
                    tagComboItem item;
                    item.m_text = szTagName;
                    item.m_value = szTagName;
                    items.append(item);
                }

                QVariant v;
                v.setValue<ComboItems>(items);
                pPropObj->setAttribute("items", v);
            }
        }
    }
}

void QFormListWidget::onZoom()
{
    if(m_current_form != NULL) {
        m_current_form->onZoom();
    }
}
