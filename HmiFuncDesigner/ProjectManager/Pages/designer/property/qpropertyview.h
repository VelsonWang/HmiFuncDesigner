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
    void setUndoStack(QUndoStack* stack);

public slots:
    void selectWidget(QAbstractHost* wid);

signals:
    void notifyPropertyEdit(QAbstractProperty* pro, const QVariant& value);

protected:
    QPropertyListView *m_propertyView;
    StyledBar *m_styledBar;
    QAbstractHost *m_select;
};

#endif // QPROPERTYVIEW_H
