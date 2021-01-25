#ifndef QDESIGNERWIDGET_H
#define QDESIGNERWIDGET_H

#include <QWidget>

#include "../../libs/core/qabstractpagewidget.h"

class QPropertyView;
class QWidgetBoxView;
class QFormWidgetView;
class QObjectListWidget;
class FormNameListWidget;

class QDesignerWidget : public QAbstractPageWidget
{
    Q_OBJECT
public:
    explicit QDesignerWidget(QWidget *parent = Q_NULLPTR);
    void setUndoStack(QUndoStack *stack);

protected:
    bool event(QEvent *ev);

protected:
    QWidgetBoxView *m_widgetBox;
    QPropertyView *m_propertyView;
    QFormWidgetView *m_formWidgetView;
    QObjectListWidget *m_objectListWidget;
    FormNameListWidget *m_formNameListWidget;
};

#endif // QDESIGNERWIDGET_H
