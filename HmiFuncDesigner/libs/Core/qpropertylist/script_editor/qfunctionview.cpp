#include "qfunctionview.h"

#include "../../../shared/host/qabstracthost.h"

#include <QItemDelegate>
#include <QPainter>
#include <QHeaderView>
#include <QIcon>
#include <QApplication>
#include <QVBoxLayout>
#include <QMetaMethod>
#include <QStringList>

class QFunctionDelegate: public QItemDelegate
{
public :
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

void QFunctionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    c=QColor(255,191,239);
    if(opt.features & QStyleOptionViewItemV2::Alternate)
    {
        c=c.lighter(112);
    }

    painter->fillRect(option.rect,c);
    opt.state &=~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
    opt.palette.setCurrentColorGroup(QPalette::Active);
    QColor color=static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor,&opt));
    painter->setPen(color);

    if(index.column()==0)
    {
        int right=option.direction==Qt::LeftToRight?option.rect.right():option.rect.left();
        painter->drawLine(right,option.rect.y(),right,option.rect.bottom());
    }
}

QSize QFunctionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz=QItemDelegate::sizeHint(option,index);
    sz.setHeight(20);
    return sz;
}

QFunctionView::QFunctionView(QWidget *parent) :
    QTreeWidget(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setItemDelegate(new QFunctionDelegate);
    setRootIsDecorated(false);
    header()->hide();

    QPalette p=this->palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    this->setPalette(p);

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(doubleClicked(QTreeWidgetItem*,int)));
}

void QFunctionView::setHost(QAbstractHost *host)
{
    clearAll();

    int count=host->metaObject()->methodCount();
    QMetaMethod method;
    QMetaMethod::MethodType methodType;

    QString str;

    QStringList l=host->property("function_list").toStringList();
    for(int i=0;i<count;i++)
    {
        method=host->metaObject()->method(i);
        methodType=method.methodType();
        if(methodType==QMetaMethod::Slot && method.access()==QMetaMethod::Public)
        {
            str=method.methodSignature();
            if(!l.contains(str))
            {
                addItem(method,host->functionInformation(str));
            }
        }
    }
    if(m_items.size()>0)
    {
        setCurrentItem(m_items.first());
    }
}

void QFunctionView::clearAll()
{
    m_items.clear();
    m_itemToMethod.clear();
    clear();
}

void QFunctionView::addItem(const QMetaMethod &method, const QString &toolTip)
{
    QTreeWidgetItem *item;
    QString str=method.methodSignature();
    int s=str.indexOf("(");
    QString str1,str2;
    str1=str.left(s);
    str2=str.mid(s);
    str2=str2.replace("QString","String");
    str=str1+str2;
    item=new QTreeWidgetItem(this);
    item->setText(0,str);
    item->setToolTip(0,toolTip==""?str:toolTip);
    m_items.append(item);
    m_itemToMethod.insert(item,method);
}

void QFunctionView::findNext(const QString &text)
{
    if(m_items.size()<=0)
    {
        return;
    }
    QList<QTreeWidgetItem*> list=this->selectedItems();
    QTreeWidgetItem *item=NULL;
    QTreeWidgetItem *root=NULL;
    if(list.size()>0)
    {
        root=list.first();
    }
    QString str;
    if(root==NULL)
    {
        root=m_items.first();
        str=root->text(0);
        if(str.indexOf(text)>=0)
        {
            root->setSelected(true);
            setCurrentItem(root);
            return;
        }
    }
    int index=m_items.indexOf(root);
    int temp;
    int count=m_items.size();
    for(int i=0;i<count;i++)
    {
        temp=(index+i+1)%count;
        item=m_items.at(temp);
        str=item->text(0);
        if(str.indexOf(text)>=0)
        {
            root->setSelected(false);
            item->setSelected(true);
            setCurrentItem(item);
            return;
        }
    }
}

void QFunctionView::findPrev(const QString &text)
{
    if(m_items.size()<=0)
    {
        return;
    }
    QList<QTreeWidgetItem*> list=this->selectedItems();
    QTreeWidgetItem *item=NULL;
    QTreeWidgetItem *root=NULL;
    if(list.size()>0)
    {
        root=list.first();
    }
    QString str;
    if(root==NULL)
    {
        root=m_items.first();
        str=root->text(0);
        if(str.indexOf(text)>=0)
        {
            root->setSelected(true);
            setCurrentItem(root);
            return;
        }
    }
    int index=m_items.indexOf(root);
    int temp;
    int count=m_items.size();
    for(int i=0;i<count;i++)
    {
        temp=(index-i-1+count)%count;
        item=m_items.at(temp);
        str=item->text(0);
        if(str.indexOf(text)>=0)
        {
            root->setSelected(false);
            item->setSelected(true);
            setCurrentItem(item);
            return;
        }
    }
}

void QFunctionView::doubleClicked(QTreeWidgetItem *item, int)
{
    emit select(m_itemToMethod.value(item));
}
