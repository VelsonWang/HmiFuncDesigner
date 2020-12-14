#ifndef QDESIGNERWIDGET_H
#define QDESIGNERWIDGET_H

#include <QWidget>

#include "../../libs/core/qabstractpagewidget.h"

class QPropertyView;
class QWidgetBoxView;
class QFormWidgetView;
class QObjectListWidget;

class QDesignerWidget : public QAbstractPageWidget
{
    Q_OBJECT
public:
    explicit QDesignerWidget(QWidget *parent = 0);

    void    set_undo_stack(QUndoStack *stack);
signals:

public slots:
protected:
    QWidgetBoxView  *m_widgetBox;
    QPropertyView   *m_propertyView;
    QFormWidgetView *m_formWidgetView;
    QObjectListWidget   *m_objectListWidget;
};

#endif // QDESIGNERWIDGET_H
