#include "propertytableview.h"


PropertyTableView::PropertyTableView(PropertyModel *model, bool bLeaveEvent, QWidget *parent)
    : QTableView(parent),
      bLeaveEvent_(bLeaveEvent)
{
    if (model) {
        setModel(model);
    }

    init();
}

void PropertyTableView::init()
{
    resizeColumnsToContents();
    setColumnWidth(0, 150);
    setAlternatingRowColors(true);
    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);
    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(25);
    horizontalHeader()->setStretchLastSection(true);

    delegate_ = new PropertyDelegate(this);
    setItemDelegate(delegate_);
}

void PropertyTableView::setPropertyKeyColumnWidth(int width)
{
    setColumnWidth(0, width);
}


void PropertyTableView::setPropertyValueColumnWidth(int width)
{
    setColumnWidth(1, width);
}


void PropertyTableView::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    if(bLeaveEvent_) {
        setPropertyValue();
    }
    QTableView::focusOutEvent(event);
}

void PropertyTableView::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(bLeaveEvent_) {
        setPropertyValue();
    }
    QTableView::leaveEvent(event);
}


void PropertyTableView::setPropertyValue()
{
    QModelIndex index = this->currentIndex();
    if(index.isValid()) {
        const PropertyModel *pModel = dynamic_cast<const PropertyModel*>(index.model());
        Property *tmpProperty = pModel->getProperty(index.row());
        if(index.column() == 1) {
            PropertyModel *pPropModel = tmpProperty->getPropertyModel();
            if(pModel != nullptr) {
                QWidget *pEditor = tmpProperty->getPropertyEditor();
                if(pEditor != nullptr) {
                    QVariant tmpData = tmpProperty->getEditorData(pEditor);
                    pPropModel->setData(index, tmpData);
                }
            }
        }
    }
}


