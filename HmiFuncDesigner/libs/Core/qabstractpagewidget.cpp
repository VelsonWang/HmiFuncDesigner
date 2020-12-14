#include "qabstractpagewidget.h"

QAbstractPageWidget::QAbstractPageWidget(QWidget *parent) :
    QWidget(parent)
{
}

void QAbstractPageWidget::setVisible(bool visible)
{
    emit selected(visible);
    QWidget::setVisible(visible);
}

void QAbstractPageWidget::set_undo_stack(QUndoStack *stack)
{
    m_undo_stack=stack;
}
