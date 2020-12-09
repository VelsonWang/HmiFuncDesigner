#ifndef ELEMENTSIMPLELISTWIDGET_H
#define ELEMENTSIMPLELISTWIDGET_H

#include <QTreeWidget>
#include <QMouseEvent>

class QTreewidgetItem;

typedef struct tagItemInfo
{
    QString name;
    QString showIcon;
    QString showName;
    QString showGroup;
}TItemInfo;

class ElementTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ElementTreeWidget(QString name, QTreeWidget *parent = Q_NULLPTR);

protected:
    void initWidgetbox();
    void addWidget(TItemInfo* info);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void startDrag();
    void addElements(QString name);

protected:
    QMap<TItemInfo*, QTreeWidgetItem*> m_infoToItem;
    QMap<QTreeWidgetItem*, TItemInfo*> m_itemToInfo;
    QMap<QString,QTreeWidgetItem*> m_nameToGroup;
    QIcon m_expandIcon;
    friend class QWidgetDelegate;

private:
    QPoint startPos;
};


#endif // ELEMENTSIMPLELISTWIDGET_H
