#include "qobjectlistview.h"

#include "qlistviewwidget.h"

#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/shared/host/qabstracthost.h"
#include "../../../libs/shared/property/qabstractproperty.h"
#include "../../../libs/shared/qhostfactory.h"
#include "../../../libs/core/undocommand/qaddhostundocommand.h"
#include "../../../libs/core/undocommand/qpageaddundocommand.h"
#include "../../../libs/core/undocommand/qpropertychangedundocommand.h"

#include <QVariant>
#include <QItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QPalette>
#include <QHeaderView>
#include <QMouseEvent>

class QObjectDelegate: public QItemDelegate
{
public :
    QObjectDelegate(QObjectListView* view):m_view(view){}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    QObjectListView     *m_view;
};

void QObjectDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    QTreeWidgetItem *item=m_view->itemFromIndex(index);
    if(item!=m_view->m_form_item && item!=m_view->m_keyboard_item && item!=m_view->m_dialog_item)
    {
        c=QColor(255,191,239);
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

QSize QObjectDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz=QItemDelegate::sizeHint(option,index);
    sz.setHeight(24);
    return sz;
}

QObjectListView::QObjectListView(QWidget *parent) :
    QTreeWidget(parent),
    m_form_item(NULL),
    m_keyboard_item(NULL),
    m_dialog_item(NULL)
{

    setFrameStyle(QFrame::NoFrame);
    setColumnCount(1);
    header()->hide();
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setItemDelegate(new QObjectDelegate(this));
    setIconSize(QSize(20,20));

    connect(QSoftCore::getCore()->getProjectCore(),SIGNAL(opened_signals()),this,SLOT(project_opened()));
    connect(QSoftCore::getCore()->getProjectCore(),SIGNAL(closed_signals()),this,SLOT(project_closed()));
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(item_clicked(QTreeWidgetItem*)));
    connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));

    connect(QSoftCore::getCore()->getProjectCore()->get_page_manager(),
            SIGNAL(insert_page_signal(QAbstractHost*)),
            this,SLOT(insert_page_slot(QAbstractHost*)));
    connect(QSoftCore::getCore()->getProjectCore()->get_page_manager(),
            SIGNAL(remove_page_signal(QAbstractHost*)),
            this,SLOT(remove_page_slot(QAbstractHost*)));

    QPalette p=this->palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    this->setPalette(p);
}

void QObjectListView::init()
{
    clear();

    QPageManager *manager=QSoftCore::getCore()->getProjectCore()->get_page_manager();

    QList<QAbstractHost*> list;


    list=manager->getPages();
    QTreeWidgetItem *par;
    QString title;
    while(list.size()>0)
    {
        QAbstractHost* h=list.takeFirst();
        if(h->getParent()==NULL)
        {
            title=h->getHostType();
            if(title=="form")
            {
                if(m_form_item==NULL)
                {
                    m_form_item=new QTreeWidgetItem;
                    this->insertTopLevelItem(0,m_form_item);
                    m_form_item->setText(0,tr("Form"));
                    m_form_item->setFlags(Qt::ItemIsEnabled);
                }
                par=m_form_item;
            }
            else if(title=="dialog")
            {
                if(m_dialog_item==NULL)
                {
                    m_dialog_item=new QTreeWidgetItem(this);
                    this->insertTopLevelItem(1,m_dialog_item);
                    m_dialog_item->setText(0,tr("Dialog"));
                    m_dialog_item->setFlags(Qt::ItemIsEnabled);
                }
                par=m_dialog_item;
            }
            else if(title=="keyboard")
            {
                if(m_keyboard_item==NULL)
                {
                    m_keyboard_item=new QTreeWidgetItem(this);
                    this->insertTopLevelItem(2,m_keyboard_item);
                    m_keyboard_item->setText(0,tr("Keyboard"));
                    m_keyboard_item->setFlags(Qt::ItemIsEnabled);
                }
                par=m_keyboard_item;
            }
            else
            {
                continue;
            }
        }
        else
        {
            par=m_host_to_item.value(h->getParent());
        }
        if(par==NULL)
        {
            continue;
        }

        QTreeWidgetItem *item=new QTreeWidgetItem(par);
        QListViewWidget *wid=new QListViewWidget;
        wid->set_icon(get_host_icon(h));
        wid->set_text(h->getPropertyValue("objectName").toString());
        setItemWidget(item,0,wid);
        connect(wid,SIGNAL(remove()),this,SLOT(button_remove()));
        m_host_to_item.insert(h,item);
        m_item_to_host.insert(item,h);
        list+=h->getChildren();
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        connect(h,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
        connect(h,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
                this,SLOT(remove_host_slot(QList<QAbstractHost*>)));
        connect(h,SIGNAL(notifyParentChanged()),this,SLOT(host_parent_changed()));
    }

    if(m_form_item!=NULL)
    {
        m_form_item->child(0)->setSelected(true);
    }
    else if(m_dialog_item!=NULL)
    {
        m_dialog_item->child(0)->setSelected(true);
    }
    else if(m_keyboard_item!=NULL)
    {
        m_keyboard_item->child(0)->setSelected(true);
    }

    this->expandAll();

    connect(manager,SIGNAL(host_name_changed(QAbstractHost*)),this,SLOT(host_name_changed(QAbstractHost*)));
}

void QObjectListView::clear()
{
    m_item_to_host.clear();
    m_host_to_item.clear();

    m_form_item=NULL;
    m_keyboard_item=NULL;
    m_dialog_item=NULL;

    QTreeWidget::clear();
}

void QObjectListView::project_opened()
{
    init();
}

void QObjectListView::project_closed()
{
    clear();
}

void QObjectListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QTreeWidgetItem *item=this->itemFromIndex(index);

    QColor c;
    QStyleOptionViewItem opt=options;
    if(m_form_item==item || m_dialog_item==item || m_keyboard_item==item)
    {
        c=opt.palette.color(QPalette::Dark);
    }
    else
    {
        c=QColor(255,191,239);
    }


    painter->fillRect(options.rect,c);
    opt.palette.setColor(QPalette::AlternateBase,c.lighter(112));
    QTreeWidget::drawRow(painter,opt,index);

    QColor color=static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor));
    painter->setPen(color);
    painter->drawLine(opt.rect.x(),opt.rect.bottom(),opt.rect.right(),opt.rect.bottom());
}

QItemSelectionModel::SelectionFlags QObjectListView::selectionCommand(const QModelIndex &index, const QEvent *event) const
{
    if(event!=NULL && (event->type()==QEvent::MouseButtonDblClick || event->type()==QEvent::MouseButtonPress))
    {
        QTreeWidgetItem *item=this->itemFromIndex(index);
        if(m_form_item==item || m_dialog_item==item || m_keyboard_item==item)
        {
            return QItemSelectionModel::NoUpdate;
        }
    }

    return QTreeWidget::selectionCommand(index,event);
}

QString QObjectListView::get_host_icon(QAbstractHost *host)
{
    tagHostInfo *info=QHostFactory::get_host_info(host->getAttribute(HOST_TYPE));
    if(info!=NULL)
    {
        return info->getShowIcon();
    }
    else
    {
        return ":/images/widget.png";
    }
}

void QObjectListView::item_clicked(QTreeWidgetItem *item)
{
    QAbstractHost* host;
    host=m_item_to_host.value(item);
    if(host!=NULL)
    {
        emit select(host);
        return;
    }
}

QModelIndex QObjectListView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex index=QTreeWidget::moveCursor(cursorAction,modifiers);
    QTreeWidgetItem *item=itemFromIndex(index);
    if(m_form_item==item || m_dialog_item==item || m_keyboard_item==item)
    {
        return currentIndex();
    }
    else
    {
        return index;
    }
}

void QObjectListView::select_changed(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if(current!=NULL)
    {
        item_clicked(current);
    }
}

void QObjectListView::set_select(QAbstractHost *host)
{
    QTreeWidgetItem *item;
    disconnect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));
    this->clearSelection();
    item=m_host_to_item.value(host);
    if(item!=NULL)
    {
        item->setSelected(true);
    }
    connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void QObjectListView::insert_host_slot(const QList<QAbstractHost *> &list, const QList<int> &indexs)
{
    if(list.size()==0)
    {
        return;
    }
    QAbstractHost* par=(QAbstractHost*)sender();

    int count=list.size();
    for(int i=0;i<count;i++)
    {
        QAbstractHost* h=list.at(i);
        int index=indexs.at(i);

        QTreeWidgetItem *parent=m_host_to_item.value(par);
        if(parent==NULL)
        {
            continue;
        }
        parent->setExpanded(true);
        QTreeWidgetItem *item=new QTreeWidgetItem;
        parent->insertChild(index,item);
        QListViewWidget *wid=new QListViewWidget;
        wid->set_icon(get_host_icon(h));
        wid->set_text(h->getPropertyValue("objectName").toString());
        setItemWidget(item,0,wid);
        connect(wid,SIGNAL(remove()),this,SLOT(button_remove()));
        m_item_to_host.insert(item,h);
        m_host_to_item.insert(h,item);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        connect(h,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
        connect(h,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
                this,SLOT(remove_host_slot(QList<QAbstractHost*>)));
        connect(h,SIGNAL(notifyParentChanged()),this,SLOT(host_parent_changed()));

        QList<QAbstractHost*> ch=h->getChildren();
        while(ch.size()>0)
        {
            QAbstractHost* hh=ch.takeFirst();
            QTreeWidgetItem *p=m_host_to_item.value(hh->getParent());
            QTreeWidgetItem *temp=new QTreeWidgetItem(p);
            QListViewWidget *wid=new QListViewWidget;
            wid->set_icon(get_host_icon(hh));
            wid->set_text(hh->getPropertyValue("objectName").toString());
            setItemWidget(item,0,wid);
            connect(wid,SIGNAL(remove()),this,SLOT(button_remove()));
            m_item_to_host.insert(temp,hh);
            m_host_to_item.insert(hh,temp);
            connect(hh,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
                    this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
            connect(hh,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
                    this,SLOT(remove_host_slot(QList<QAbstractHost*>)));
            connect(hh,SIGNAL(notifyParentChanged()),this,SLOT(host_parent_changed()));
            ch+=hh->getChildren();
        }

    }

    set_select(list.first());
}

void QObjectListView::remove_host_slot(const QList<QAbstractHost *> &list)
{
    QList<QAbstractHost*> l=list;
    while(l.size()>0)
    {
        QAbstractHost* h=l.takeFirst();
        disconnect(h,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
        disconnect(h,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
                this,SLOT(remove_host_slot(QList<QAbstractHost*>)));
        disconnect(h,SIGNAL(notifyParentChanged()),this,SLOT(host_parent_changed()));
        QTreeWidgetItem *item;
        item=m_host_to_item.value(h);
        if(item!=NULL)
        {
            m_host_to_item.remove(h);
            m_item_to_host.remove(item);
        }
        l+=h->getChildren();
        if(list.contains(h))
        {
            delete item;
        }
    }
}

void QObjectListView::host_parent_changed()
{
    QAbstractHost* host=(QAbstractHost*)sender();

    QTreeWidgetItem* item;
    item=m_host_to_item.value(host);
    QTreeWidgetItem *par=m_host_to_item.value(host->getParent());
    item->parent()->removeChild(item);
    int index=host->getParent()->getChildren().indexOf(host);
    par->insertChild(index,item);
    par->setExpanded(true);
}

void QObjectListView::button_remove()
{
    QWidget* wid=(QWidget*)sender();

    QMapIterator<QAbstractHost*,QTreeWidgetItem*> it(m_host_to_item);
    while(it.hasNext())
    {
        it.next();
        if(itemWidget(it.value(),0)==wid)
        {
            QAbstractHost* par=it.key()->getParent();
            if(par!=NULL)
            {
                QList<QAbstractHost*> list;
                list.append(it.key());
                QList<int> indexs;
                indexs.append(par->getChildren().indexOf(it.key()));
                QAddHostUndoCommand *cmd=new QAddHostUndoCommand(par,list,indexs,AHT_REMOVE);
                m_undo_stack->push(cmd);
            }
            else
            {
                QPageManager* manager=QSoftCore::getCore()->getProjectCore()->get_page_manager();

                QUndoCommand *cmd=new QUndoCommand;
                new QPageAddUndoCommand(it.key(),manager->getPages().indexOf(it.key()),PAT_REMOVE,cmd);

                QAbstractProperty* pro=QSoftCore::getCore()->getProjectCore()->get_project_host()->getProperty("start_page");
                if(pro!=NULL)
                {
                    if(pro->get_value().toString()==it.key()->getUuid())
                    {
                        QList<QAbstractHost*> list=QSoftCore::getCore()
                                ->getProjectCore()->get_page_manager()->getPages_by_title("form");
                        list.removeAll(it.key());
                        QString str;
                        if(list.size()>0)
                        {
                            str=list.first()->getUuid();
                        }
                        else
                        {
                            str="";
                        }

                        new QPropertyChangedUndoCommand(QSoftCore::getCore()->getProjectCore()->get_project_host()->getUuid(),
                                                        "start_page",pro->get_value(),str,cmd);
                    }
                }

                m_undo_stack->push(cmd);
            }
            return;
        }
    }
}

void QObjectListView::set_undo_stack(QUndoStack *stack)
{
    m_undo_stack=stack;
}

void QObjectListView::host_name_changed(QAbstractHost *host)
{
    QTreeWidgetItem *item=m_host_to_item.value(host);
    if(item!=NULL)
    {
        QListViewWidget *wid=(QListViewWidget*)itemWidget(item,0);
        wid->set_text(host->getPropertyValue("objectName").toString());
    }
}

void QObjectListView::insert_page_slot(QAbstractHost *page)
{
    disconnect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
           this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));
    QString type=page->getHostType();
    QPageManager *manager=QSoftCore::getCore()->getProjectCore()->get_page_manager();

    int index=manager->getPages_by_title(type).indexOf(page);

    QTreeWidgetItem *par;
    if(type=="form")
    {
        if(m_form_item==NULL)
        {
            m_form_item=new QTreeWidgetItem;
            this->insertTopLevelItem(0,m_form_item);
            m_form_item->setText(0,tr("Form"));
            m_form_item->setFlags(Qt::ItemIsEnabled);
        }
        par=m_form_item;
    }
    else if(type=="dialog")
    {
        if(m_dialog_item==NULL)
        {
            m_dialog_item=new QTreeWidgetItem(this);
            this->insertTopLevelItem(1,m_dialog_item);
            m_dialog_item->setText(0,tr("Dialog"));
            m_dialog_item->setFlags(Qt::ItemIsEnabled);
        }
        par=m_dialog_item;
    }
    else if(type=="keyboard")
    {
        if(m_keyboard_item==NULL)
        {
            m_keyboard_item=new QTreeWidgetItem(this);
            this->insertTopLevelItem(2,m_keyboard_item);
            m_keyboard_item->setText(0,tr("Keyboard"));
            m_keyboard_item->setFlags(Qt::ItemIsEnabled);
        }
        par=m_keyboard_item;
    }
    else
    {
        return;
    }

    par->setExpanded(true);

    QTreeWidgetItem *item=new QTreeWidgetItem;
    par->insertChild(index,item);
    QListViewWidget *wid=new QListViewWidget;
    wid->set_icon(get_host_icon(page));
    wid->set_text(page->getPropertyValue("objectName").toString());
    setItemWidget(item,0,wid);
    connect(wid,SIGNAL(remove()),this,SLOT(button_remove()));
    m_item_to_host.insert(item,page);
    m_host_to_item.insert(page,item);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setExpanded(true);
    item->setSelected(true);

    connect(page,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
            this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
    connect(page,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
            this,SLOT(remove_host_slot(QList<QAbstractHost*>)));

    QList<QAbstractHost*> ch=page->getChildren();
    while(ch.size()>0)
    {
        QAbstractHost* hh=ch.takeFirst();
        QTreeWidgetItem *p=m_host_to_item.value(hh->getParent());
        QTreeWidgetItem *temp=new QTreeWidgetItem(p);
        QListViewWidget *wid=new QListViewWidget;
        wid->set_icon(get_host_icon(hh));
        wid->set_text(hh->getPropertyValue("objectName").toString());
        setItemWidget(temp,0,wid);
        connect(wid,SIGNAL(remove()),this,SLOT(button_remove()));
        m_item_to_host.insert(temp,hh);
        m_host_to_item.insert(hh,temp);
        connect(hh,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
        connect(hh,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
                this,SLOT(remove_host_slot(QList<QAbstractHost*>)));
        connect(hh,SIGNAL(notifyParentChanged()),this,SLOT(host_parent_changed()));
        ch+=hh->getChildren();
    }
    connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
           this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void QObjectListView::remove_page_slot(QAbstractHost *page)
{
    disconnect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
           this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));
    QList<QAbstractHost*> l;
    l.append(page);
    while(l.size()>0)
    {
        QAbstractHost* h=l.takeFirst();
        disconnect(h,SIGNAL(insert_children_signal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insert_host_slot(QList<QAbstractHost*>,QList<int>)));
        disconnect(h,SIGNAL(remove_children_signal(QList<QAbstractHost*>)),
                this,SLOT(remove_host_slot(QList<QAbstractHost*>)));
        disconnect(h,SIGNAL(notifyParentChanged()),this,SLOT(host_parent_changed()));
        QTreeWidgetItem *item;
        item=m_host_to_item.value(h);
        if(item!=NULL)
        {
            m_host_to_item.remove(h);
            m_item_to_host.remove(item);
        }
        l+=h->getChildren();
        if(h==page)
        {
            delete item;
        }
    }

    QString type=page->getHostType();
    if(type=="form")
    {
        if(m_form_item->childCount()==0)
        {
            delete m_form_item;
            m_form_item=NULL;
        }
    }
    else if(type=="dialog")
    {
        if(m_dialog_item->childCount()==0)
        {
            delete m_dialog_item;
            m_dialog_item=NULL;
        }
    }
    else if(type=="keyboard")
    {
        if(m_keyboard_item->childCount()==0)
        {
            delete m_keyboard_item;
            m_keyboard_item=NULL;
        }
    }

    connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(select_changed(QTreeWidgetItem*,QTreeWidgetItem*)));
}
