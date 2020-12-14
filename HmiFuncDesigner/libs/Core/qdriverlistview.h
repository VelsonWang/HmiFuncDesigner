#ifndef QDRIVERLISTVIEW_H
#define QDRIVERLISTVIEW_H

#include "corelibglobal.h"

#include <QTreeWidget>
#include <QMap>

class CORELIB_EXPORT QDriverListView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QDriverListView(QWidget *parent = 0);
    
    QString get_type();
protected:
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
signals:
protected:
    friend class QDriverListDelegate;
public slots:
    void    item_click(QTreeWidgetItem* item);
protected:
    QMap<QTreeWidgetItem*,QString>      m_item_to_type;
    QMap<QString,QTreeWidgetItem*>      m_group_to_item;
    QTreeWidgetItem*                    m_select_item;
};

#endif // QDRIVERLISTVIEW_H
