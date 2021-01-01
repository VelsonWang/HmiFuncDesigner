#ifndef QPROPERTYLISTVIEW_H
#define QPROPERTYLISTVIEW_H

#include "../corelibglobal.h"

#include <QTreeWidget>

#include <QMap>
#include <QItemDelegate>
#include <QVector>
#include <QPair>
#include <QColor>
#include <QMap>

class QAbstractProperty;
class QUndoStack;
class QPropertyListView;
class QPropertyBaseEditor;

class QPropertyEditorDelegate: public QItemDelegate
{
    Q_OBJECT
public :
    QPropertyEditorDelegate(QPropertyListView* parent):m_listView(parent){}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void  setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
protected:
    QPropertyListView   *m_listView;
};

class CORELIB_EXPORT QPropertyListView: public QTreeWidget
{
    Q_OBJECT
public:
    QPropertyListView(QWidget *parent=Q_NULLPTR);
    void    setPropertys(const QList<QAbstractProperty*> &propertys);

    void    clearAll();

    void    setUndoStack(QUndoStack* stack);
public:
    QAbstractProperty               *itemToProperty(QTreeWidgetItem *item) const;
    QTreeWidgetItem                 *indexToItem(const QModelIndex &index)const;
    bool                            isGroupItem(QTreeWidgetItem *item)const;
    int                             groupIndex(QTreeWidgetItem *item)const;
protected:
    void    updateView();
    void    insertItem(QAbstractProperty* property,QAbstractProperty* parent=NULL);
    void    mousePressEvent(QMouseEvent *event);
    void    drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
protected slots:
    void    property_changed();
    void    property_edit(const QVariant& value);

signals:
    void    notifyPropertyEdit(QAbstractProperty* pro,const QVariant& value);
    void    active();

private:
    friend class QPropertyEditorDelegate;
protected:
    QList<QAbstractProperty*>                               m_propertys;
    QMap<QString,QTreeWidgetItem*>                          m_groupItems;
    QList<QTreeWidgetItem*>                                 m_groups;
    QMap<QAbstractProperty*,QTreeWidgetItem*>               m_propertyToItem;
    QMap<QTreeWidgetItem*,QAbstractProperty*>               m_itemToProperty;
    QVector<QPair<QColor,QColor> >                          m_colors;
    QPropertyEditorDelegate                                 *m_delegate;
    QIcon                                                   m_expandIcon;
    QVariant                                                m_page_position;
    QUndoStack                                              *m_undo_stack;
};

#endif // QPROPERTYLISTVIEW_H
