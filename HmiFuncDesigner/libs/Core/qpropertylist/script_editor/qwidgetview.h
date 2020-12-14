#ifndef QWIDGETVIEW_H
#define QWIDGETVIEW_H

#include <QTreeWidget>
#include <QMap>

class QAbstractHost;

enum  FIND_POINT
{
    FP_NEXT,
    FP_PREV
};

class QWidgetView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QWidgetView(QAbstractHost* host,QWidget *parent = 0);
    void    setSelect(QAbstractHost *host);
    QAbstractHost *currentHost();
protected:
    void    initWidgetList();
    void    drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void    setSelected(QTreeWidgetItem *item);
    QIcon   get_host_icon(QAbstractHost* host);
signals:
    void    select(QAbstractHost* host);
public slots:
    void    clickedItem(QTreeWidgetItem *item,int col);
protected:
    QMap<QAbstractHost*,QTreeWidgetItem*>   m_items;
    QAbstractHost                           *m_property_host;
};

#endif // QWIDGETVIEW_H
