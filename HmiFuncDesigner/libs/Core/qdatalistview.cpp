#include "qdatalistview.h"

#include "qsoftcore.h"
#include "stylehelper.h"

#include "../shared/qprojectcore.h"
#include "../shared/qdatamanager.h"
#include "../shared/qdata.h"

#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>

class QDataListDelegate : public QItemDelegate
{
public:
    QDataListDelegate(QDataListView* view):m_view(view){}
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    QDataListView   *m_view;
};

QSize QDataListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size=QItemDelegate::sizeHint(option,index);
    size.setHeight(21);
    size.rwidth()+=4;
    return size;
}

void QDataListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTreeWidgetItem *item=m_view->itemFromIndex(index);
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    if(m_view->m_item_to_data.keys().contains(item))
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

QDataListView::QDataListView(QWidget *parent) :
    QTreeWidget(parent),
    m_select_item(NULL)
{
    QDataManager* manager=QSoftCore::getCore()->getProjectCore()->get_data_manager();

    QList<QData*>   datas=manager->get_all_datas();

    QTreeWidgetItem* par;
    QTreeWidgetItem* item;

    QIcon icon=StyleHelper::drawIndicatorIcon(this->palette(),this->style());

    foreach(QData* data,datas)
    {
        par=new QTreeWidgetItem(this);
        par->setText(0,data->get_name());
        par->setToolTip(0,data->get_name());
        par->setIcon(0,icon);
        par->setExpanded(true);
        m_item_to_group.insert(par,data);

        QList<tagDataInfo*> list=data->get_datas();
        foreach(tagDataInfo* d,list)
        {
            item=new QTreeWidgetItem(par);
            item->setText(0,d->m_name);
            item->setToolTip(0,d->m_name);
            m_item_to_data.insert(item,d);
            if(m_select_item==NULL)
            {
                m_select_item=item;
                m_select_item->setIcon(0,QIcon(":/images/check.png"));
            }
        }
    }

    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->header()->hide();
    this->setItemDelegate(new QDataListDelegate(this));
    this->setMinimumSize(100,200);
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(item_clicked(QTreeWidgetItem*)));

}

void QDataListView::item_clicked(QTreeWidgetItem *item)
{
    if(m_item_to_data.keys().contains(item) && m_select_item!=item)
    {
        if(m_select_item!=NULL)
        {
            m_select_item->setIcon(0,QIcon());
        }
        m_select_item=item;
        if(m_select_item!=NULL)
        {
            m_select_item->setIcon(0,QIcon(":/images/check.png"));
        }
    }
}

void QDataListView::set_select(const QString &data_uuid)
{
    tagDataInfo *data=QSoftCore::getCore()->getProjectCore()->get_data_manager()
            ->get_data(data_uuid);
    if(data!=NULL)
    {
        QMapIterator<QTreeWidgetItem*,tagDataInfo*> it(m_item_to_data);

        while(it.hasNext())
        {
            it.next();
            if(it.value()==data)
            {
                if(m_select_item!=it.key())
                {
                    if(m_select_item!=NULL)
                    {
                        m_select_item->setIcon(0,QIcon());
                    }
                    m_select_item=it.key();
                    m_select_item->setIcon(0,QIcon(":/images/check.png"));

                }
                return;
            }
        }
    }
}

QString QDataListView::get_select_data()
{
    return m_item_to_data.value(m_select_item)->m_uuid;
}

void QDataListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QTreeWidgetItem *item=this->itemFromIndex(index);

    QColor c;
    QStyleOptionViewItem opt=options;
    if(m_item_to_group.keys().contains(item))
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
