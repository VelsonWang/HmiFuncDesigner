#ifndef QPROPERTYVIEW_H
#define QPROPERTYVIEW_H

#include <QWidget>

class QPropertyListView;
class StyledBar;
class QAbstractHost;
class QAbstractProperty;
class QUndoStack;

class QPropertyView : public QWidget
{
    Q_OBJECT
public:
    explicit QPropertyView(QWidget *parent = 0);
    void    set_undo_stack(QUndoStack* stack);
public slots:
    void    selectWidget(QAbstractHost* wid);
signals:
    void    property_edit_signal(QAbstractProperty* pro,const QVariant& value);
protected:
    QPropertyListView           *m_propertyView;
    StyledBar                   *m_styledBar;
    QAbstractHost               *m_select;
};

#endif // QPROPERTYVIEW_H
