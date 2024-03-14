#include "widgetboxtreewidget.h"
#include "../qdesignermimedata.h"
#include "../../../libs/shared/qhostfactory.h"
#include "../../../libs/core/stylehelper.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QPalette>
#include <QMouseEvent>

class QWidgetDelegate: public QItemDelegate
{
public :
    QWidgetDelegate(WidgetBoxTreeWidget* view): m_view(view) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    WidgetBoxTreeWidget *m_view;
};

void QWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    QColor c;
    if(m_view->m_item_to_info.keys().contains(m_view->itemFromIndex(index))) {
        c = QColor(242, 242, 242);
        if(opt.features & QStyleOptionViewItem::Alternate) {
            c = c.lighter(112);
        }
    } else {
        c = opt.palette.color(QPalette::Dark);
    }
    painter->fillRect(option.rect, c);
    opt.state &= ~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter, opt, index);
}

QSize QWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz = QItemDelegate::sizeHint(option, index);
    sz.setHeight(25);
    return sz;
}

WidgetBoxTreeWidget::WidgetBoxTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setIndentation(0);
    setRootIsDecorated(false);
    setColumnCount(1);
    header()->hide();
    header()->setSectionResizeMode(QHeaderView::Stretch);
    setTextElideMode(Qt::ElideMiddle);
    setVerticalScrollMode(ScrollPerPixel);
    setFrameStyle(QFrame::NoFrame);
    setAcceptDrops(true);
    setAlternatingRowColors(true);
    setIconSize(QSize(24, 24));
    setItemDelegate(new QWidgetDelegate(this));
    m_expandIcon = StyleHelper::drawIndicatorIcon(this->palette(), this->style());
    initWidgetBox();
}

WidgetBoxTreeWidget::~WidgetBoxTreeWidget()
{

}

void WidgetBoxTreeWidget::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QColor c;
    QStyleOptionViewItem opt = options;
    if(m_name_to_group.values().contains(itemFromIndex(index))) {
        c = opt.palette.color(QPalette::Dark);
    } else {
        c = QColor(255, 240, 191);
    }
    painter->fillRect(options.rect, c);
    opt.palette.setColor(QPalette::AlternateBase, c.lighter(112));
    QTreeWidget::drawRow(painter, opt, index);
}

void WidgetBoxTreeWidget::initWidgetBox()
{
    QMapIterator<QString, tagHostInfo*> it(QHostFactory::getHostInfo());
    while(it.hasNext()) {
        it.next();
        addWidget(it.value());
    }
    this->expandAll();
}

void WidgetBoxTreeWidget::addWidget(tagHostInfo *info)
{
    QString group = info->getShowGroup();
    QString name = info->getShowName();
    if(group == "" || name == "") {
        return;
    }
    QTreeWidgetItem *p = m_name_to_group.value(group);
    QTreeWidgetItem *item;
    if(!p) {
        p = new QTreeWidgetItem(this);
        p->setText(0, group);
        p->setFlags(Qt::ItemIsEnabled);
        p->setIcon(0, m_expandIcon);
        m_name_to_group.insert(group, p);
    }

    item = new QTreeWidgetItem(p);
    item->setText(0, name);
    item->setIcon(0, QIcon(info->getShowIcon()));
    item->setFlags(Qt::ItemIsEnabled);
    m_info_to_item.insert(info, item);
    m_item_to_info.insert(item, info);
}

void WidgetBoxTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());

    if(item) {
        if(m_name_to_group.values().contains(item)) {
            if(event->pos().x() + header()->offset() < 20) {
                item->setExpanded(!item->isExpanded());
            }
        } else if(m_item_to_info.keys().contains(item)) {
            if(event->button() != Qt::LeftButton) {
                return;
            }
            tagHostInfo *info = m_item_to_info.value(item);
            if(info) {
                handleMousePress(info);
            }
        }
    }

    QTreeWidget::mousePressEvent(event);
}

void WidgetBoxTreeWidget::handleMousePress(tagHostInfo *info)
{
    QList<QDesignerDnDItemInterface*> item_list;
    item_list.append(new WidgetBoxDnDItem(info->m_name, QCursor::pos()));

    foreach(QDesignerDnDItemInterface *item, item_list) {
        if(!item->host()) {
            return;
        }
    }

    QDesignerMimeData::execDrag(item_list, this);
}
