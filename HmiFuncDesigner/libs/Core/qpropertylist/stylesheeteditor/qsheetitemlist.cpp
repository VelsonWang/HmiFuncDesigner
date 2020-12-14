#include "qsheetitemlist.h"

#include "qitemlistwidget.h"

#include "../../../shared/property/stylesheetitem/qabstractstylesheetitem.h"

#include <QItemDelegate>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPainter>

class QSheetItemDelegate: public QItemDelegate
{
public :
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
};

void QSheetItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    c=QColor(255,255,191);
    if(opt.features & QStyleOptionViewItemV2::Alternate)
    {
        c=c.lighter(112);
    }
    painter->fillRect(option.rect,c);
    opt.state &=~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
}

QSize QSheetItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz=QItemDelegate::sizeHint(option,index);
    sz.setHeight(22);
    return sz;
}

QSheetItemList::QSheetItemList(QWidget *parent) :
    QTreeWidget(parent),
    m_select_item(NULL)
{
    this->setFrameStyle(QFrame::NoFrame);
    this->header()->hide();
    this->setAlternatingRowColors(true);
    this->setItemDelegate(new QSheetItemDelegate);
    this->setRootIsDecorated(false);

    this->setMinimumWidth(100);

    QPalette p=palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    setPalette(p);

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(double_clicked(QTreeWidgetItem*)));
}

void QSheetItemList::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu;

    menu.addAction(tr("Add"),this,SIGNAL(add()));

    menu.exec(e->globalPos());
}

void QSheetItemList::add(QAbstractStylesheetItem *item)
{
    QTreeWidgetItem *temp=new QTreeWidgetItem(this);

    QString title=item->attribute("title");
    if(title.startsWith("::"))
    {
        title=title.remove(0,2);
    }
    else if(title.startsWith(":"))
    {
        title=title.remove(0,1);
    }
    title=title.replace("::",":");
    title=title.replace(":",".");

    QItemListWidget *wid=new QItemListWidget;
    wid->set_text(title);
    connect(wid,SIGNAL(remove()),this,SLOT(remove()));
    connect(wid,SIGNAL(changed()),this,SLOT(changed()));
    setItemWidget(temp,0,wid);

    m_item_to_sheet.insert(temp,item);
    m_sheet_to_item.insert(item,temp);
}

void QSheetItemList::changed_item(QAbstractStylesheetItem *item)
{
    QTreeWidgetItem *temp=m_sheet_to_item.value(item);
    if(temp!=NULL)
    {
        QString title=item->attribute("title");
        if(title.startsWith("::"))
        {
            title=title.remove(0,2);
        }
        else if(title.startsWith(":"))
        {
            title=title.remove(0,1);
        }
        title=title.replace("::",":");
        title=title.replace(":",".");

        QItemListWidget* wid=(QItemListWidget*)itemWidget(temp,0);
        wid->set_text(title);
    }
}

void QSheetItemList::remove()
{
    QItemListWidget* wid=(QItemListWidget*)sender();

    QMapIterator<QAbstractStylesheetItem*,QTreeWidgetItem*> it(m_sheet_to_item);

    while(it.hasNext())
    {
        it.next();
        if(itemWidget(it.value(),0)==wid)
        {
            if(m_select_item==it.value())
            {
                set_select(m_item_to_sheet.value(topLevelItem(0)));
            }
            m_sheet_to_item.remove(it.key());
            m_item_to_sheet.remove(it.value());
            emit remove(it.key());
            delete it.value();
            return;
        }
    }
}

void QSheetItemList::changed()
{
    QItemListWidget* wid=(QItemListWidget*)sender();

    QMapIterator<QAbstractStylesheetItem*,QTreeWidgetItem*> it(m_sheet_to_item);

    while(it.hasNext())
    {
        it.next();
        if(itemWidget(it.value(),0)==wid)
        {
            emit changed(it.key());
            return;
        }
    }
}

void QSheetItemList::double_clicked(QTreeWidgetItem *item)
{
    if(m_select_item!=item)
    {
        if(m_select_item!=NULL)
        {
            QItemListWidget *wid=(QItemListWidget*)itemWidget(m_select_item,0);
            wid->set_icon("");
        }
        m_select_item=item;
        if(m_select_item!=NULL)
        {
            QItemListWidget *wid=(QItemListWidget*)itemWidget(m_select_item,0);
            wid->set_icon(":/images/check.png");
        }
        emit select(m_item_to_sheet.value(item));
    }
}

void QSheetItemList::set_select(QAbstractStylesheetItem *item)
{
    if(m_select_item!=NULL)
    {
        QItemListWidget *wid=(QItemListWidget*)itemWidget(m_select_item,0);
        wid->set_icon("");
    }
    m_select_item=m_sheet_to_item.value(item);
    if(m_select_item!=NULL)
    {
        QItemListWidget *wid=(QItemListWidget*)itemWidget(m_select_item,0);
        wid->set_icon(":/images/check.png");
    }
    emit select(item);
}
