#ifndef ELEMENTSIMPLELISTWIDGET_H
#define ELEMENTSIMPLELISTWIDGET_H

#include <QTreeWidget>
#include <QMouseEvent>

class QTreewidgetItem;

typedef struct tagItemInfo
{
    QString name;
    QIcon showIcon;
    QString showName;
    QString showGroup;
}TItemInfo;

class ElementTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ElementTreeWidget(QTreeWidget *parent = Q_NULLPTR);
    ~ElementTreeWidget();
    void addElements(const QString &szGroupName);

protected:
    void addWidget(TItemInfo* info);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void startDrag();


protected:
    QMap<TItemInfo*, QTreeWidgetItem*> m_infoToItem;
    QMap<QTreeWidgetItem*, TItemInfo*> m_itemToInfo;
    QMap<QString, QTreeWidgetItem*> m_nameToGroup;
    QIcon m_expandIcon;
    friend class QWidgetDelegate;

private:
    QPoint startPos;
};


#endif // ELEMENTSIMPLELISTWIDGET_H
