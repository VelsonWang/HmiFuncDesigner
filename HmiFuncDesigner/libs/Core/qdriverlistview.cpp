#include "qdriverlistview.h"

#include "../shared/qdriverfactory.h"
#include "stylehelper.h"

#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>

class QDriverListDelegate : public QItemDelegate
{
public:
    QDriverListDelegate(QDriverListView* view):m_view(view){}
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    QDriverListView   *m_view;
};

QSize QDriverListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size=QItemDelegate::sizeHint(option,index);
    size.setHeight(21);
    size.rwidth()+=4;
    return size;
}

void QDriverListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTreeWidgetItem *item=m_view->itemFromIndex(index);
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    if(m_view->m_item_to_type.keys().contains(item))
    {
        c=QColor(255,255,191);
        if(opt.features & QStyleOptionViewItemV2::Alternate)
        {
            c=c.lighter(112);
        }
    }
    else
    {
        c=opt.palette.color(QPalette::Dark);
    }
    painter->fillRect(option.rect,c);
    opt.state &=~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
}

QDriverListView::QDriverListView(QWidget *parent) :
    QTreeWidget(parent),
    m_select_item(NULL)
{
    QMap<QString,tagDriverInfo*> drivers=QDriverFactory::get_driver_info();

    QMapIterator<QString,tagDriverInfo*> it(drivers);

    QString group;
    QString name;
    QIcon icon=StyleHelper::drawIndicatorIcon(this->palette(),this->style());
    while(it.hasNext())
    {
        it.next();
        group=it.value()->getShowGroup();
        name=it.value()->getShowName();
        QTreeWidgetItem *par=m_group_to_item.value(group);
        if(par==NULL)
        {
            par=new QTreeWidgetItem(this);
            par->setText(0,group);
            par->setIcon(0,icon);
            par->setExpanded(true);
            m_group_to_item.insert(group,par);
        }

        QTreeWidgetItem *item=new QTreeWidgetItem(par);
        item->setText(0,name);
        m_item_to_type.insert(item,it.key());
        if(m_select_item==NULL)
        {
            m_select_item=item;
            m_select_item->setIcon(0,QIcon(":/images/check.png"));
        }
    }

    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->header()->hide();
    this->setItemDelegate(new QDriverListDelegate(this));
    this->setMinimumSize(100,200);
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(item_click(QTreeWidgetItem*)));
}

void QDriverListView::item_click(QTreeWidgetItem *item)
{
    if(m_item_to_type.keys().contains(item))
    {
        if(m_select_item!=NULL)
        {
            m_select_item->setIcon(0,QIcon());
        }
        m_select_item=item;
        m_select_item->setIcon(0,QIcon(":/images/check.png"));
    }
}

QString QDriverListView::get_type()
{
    return m_item_to_type.value(m_select_item);
}

void QDriverListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QTreeWidgetItem *item=this->itemFromIndex(index);

    QColor c;
    QStyleOptionViewItem opt=options;
    if(m_group_to_item.values().contains(item))
    {
        c=opt.palette.color(QPalette::Dark);
    }
    else
    {
        c=QColor(255,255,191);
    }
    painter->fillRect(options.rect,c);
    opt.palette.setColor(QPalette::AlternateBase,c.lighter(112));
    QTreeWidget::drawRow(painter,opt,index);
}
