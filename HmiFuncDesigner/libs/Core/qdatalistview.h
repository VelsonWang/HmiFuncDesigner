#ifndef QDATALISTVIEW_H
#define QDATALISTVIEW_H

#include "corelibglobal.h"

#include <QTreeWidget>

#include <QMap>

class QData;
struct tagDataInfo;


class CORELIB_EXPORT QDataListView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QDataListView(QWidget *parent = 0);
    QString    get_select_data();
    void    set_select(const QString &data_uuid);
protected:
    void    drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
protected:
    friend class QDataListDelegate;
signals:
public slots:
    void    item_clicked(QTreeWidgetItem* item);
protected:
    QMap<QTreeWidgetItem*,QData*>       m_item_to_group;
    QMap<QTreeWidgetItem*,tagDataInfo*> m_item_to_data;
    QTreeWidgetItem*                    m_select_item;
};

#endif // QDATALISTVIEW_H
