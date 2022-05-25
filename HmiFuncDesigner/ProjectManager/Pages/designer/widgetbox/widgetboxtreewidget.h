#ifndef WIDGETBOXTREEWIDGET_H
#define WIDGETBOXTREEWIDGET_H

#include <QTreeWidget>

class QTreewidgetItem;
class tagHostInfo;

class WidgetBoxTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit WidgetBoxTreeWidget(QWidget *parent = 0);
    ~WidgetBoxTreeWidget();

protected:
    void initWidgetBox();
    void addWidget(tagHostInfo* info);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void mousePressEvent(QMouseEvent *event);
    void handleMousePress(tagHostInfo* info);

protected:
    friend class QWidgetDelegate;

protected:
    QMap<tagHostInfo*, QTreeWidgetItem*> m_info_to_item;
    QMap<QTreeWidgetItem*, tagHostInfo*> m_item_to_info;
    QMap<QString, QTreeWidgetItem*> m_name_to_group;
    QIcon m_expandIcon;
};

#endif // WIDGETBOXTREEWIDGET_H
