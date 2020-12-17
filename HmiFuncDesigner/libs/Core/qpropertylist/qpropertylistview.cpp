#include "qpropertylistview.h"

#include "qpropertybaseeditor.h"

#include "../stylehelper.h"

#include "../../shared/property/qabstractproperty.h"
#include "../../shared/qpropertyfactory.h"


#include <QItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QHeaderView>
#include <QMetaObject>
#include <QMetaProperty>
#include <QPalette>
#include <QMouseEvent>

QWidget *QPropertyEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    QWidget *wid=NULL;
    if(index.column()==1)
    {
        QTreeWidgetItem *item=m_listView->indexToItem(index);
        QAbstractProperty *property=m_listView->itemToProperty(item);
        if(property!=NULL && property->getAttribute(ATTR_EDITABLE).toBool())
        {
            wid=new QPropertyBaseEditor(property,m_listView->m_undo_stack,parent);
        }
    }
    return wid;
}


void QPropertyEditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect.adjusted(0,0,0,-1));
}

QSize QPropertyEditorDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size=QItemDelegate::sizeHint(option,index);
    size.setHeight(21);
    size.rwidth()+=4;
    return size;
}

void QPropertyEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void QPropertyEditorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTreeWidgetItem* item=m_listView->indexToItem(index);
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    if(item!=NULL)
    {
        QAbstractProperty *p=m_listView->itemToProperty(item);

        if(p!=NULL)
        {
            if(p->modified() && index.column()==0 && p->getParent()==NULL)
            {
                opt.font.setBold(true);
                opt.fontMetrics=QFontMetrics(opt.font);
            }
            QModelIndex temp=index;
            while(temp.isValid() && !m_listView->isGroupItem(m_listView->indexToItem(temp)))
            {
                temp=temp.parent();
            }
            if(temp.isValid())
            {
                QTreeWidgetItem *tempItem=m_listView->indexToItem(temp);
                c=m_listView->m_colors.at(m_listView->groupIndex(tempItem)%m_listView->m_colors.count()).first;
            }
            else
            {
                c=m_listView->m_colors.at(0).first;
            }

            if(opt.features & QStyleOptionViewItemV2::Alternate)
            {
                c=c.lighter(112);
            }

        }
        else
        {
            c=opt.palette.color(QPalette::Dark);
            opt.palette.setColor(QPalette::Text,opt.palette.color(QPalette::BrightText));
        }
    }



    painter->fillRect(option.rect,c);
    opt.state &=~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
    opt.palette.setCurrentColorGroup(QPalette::Active);
    QColor color=static_cast<QRgb>(qApp->style()->styleHint(QStyle::SH_Table_GridLineColor,&opt));
    painter->setPen(color);

    if(!m_listView->isGroupItem(item))
    {
        if(index.column()==0)
        {
            int right=option.direction==Qt::LeftToRight?option.rect.right():option.rect.left();
            painter->drawLine(right,option.rect.y(),right,option.rect.bottom());
        }
    }
}


QPropertyListView::QPropertyListView(QWidget *parent):
    QTreeWidget(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setIconSize(QSize(18, 18));
    setColumnCount(2);
    QStringList labels;
    labels.append(tr("Property"));
    labels.append(tr("Value"));
    setHeaderLabels(labels);
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setRootIsDecorated(false);
    //header()->setMovable(false);

    QVector<QColor> colors;
    colors.reserve(6);
    colors.push_back(QColor(255,230,191));
    colors.push_back(QColor(255,255,191));
    colors.push_back(QColor(191,255,191));
    colors.push_back(QColor(199,255,255));
    colors.push_back(QColor(234,191,255));
    colors.push_back(QColor(255,191,239));
    m_colors.reserve(colors.count());
    const int darknessFactor=250;
    for(int i=0;i<colors.count();i++)
    {
        QColor c=colors.at(i);
        m_colors.push_back(qMakePair(c,c.darker(darknessFactor)));
    }

    m_delegate=new QPropertyEditorDelegate(this);
    setItemDelegate(m_delegate);

    m_expandIcon=StyleHelper::drawIndicatorIcon(this->palette(),this->style());

    QPalette p=this->palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    this->setPalette(p);
}

void QPropertyListView::setPropertys(const QList<QAbstractProperty *> &propertys)
{
    clearAll();
    m_propertys=propertys;
    updateView();
    this->expandAll();
}

void QPropertyListView::clearAll()
{
    foreach(QAbstractProperty* pro,m_propertys)
    {
        disconnect(pro,SIGNAL(value_chaged(QVariant,QVariant)),this,SLOT(property_changed()));
        disconnect(pro,SIGNAL(edit_value(QVariant)),this,SLOT(property_edit(QVariant)));
    }

    this->clear();
    m_groupItems.clear();
    m_propertyToItem.clear();
    m_itemToProperty.clear();
    m_groups.clear();
}

void QPropertyListView::updateView()
{
    foreach(QAbstractProperty* pro,m_propertys)
    {
        insertItem(pro);
    }
}

void QPropertyListView::insertItem(QAbstractProperty *property, QAbstractProperty *parent)
{
    if(property==NULL)
    {
        return;
    }
    if(!property->getAttribute(ATTR_VISIBLE).toBool())
    {
        return;
    }
    QTreeWidgetItem *parentItem;
    if(parent==NULL)
    {
        QString str=property->getAttribute("group").toString();
        if(str=="")
        {
            str=tr("Common");
        }
        parentItem=m_groupItems.value(str);
        if(parentItem==NULL)
        {
            parentItem=new QTreeWidgetItem(this);
            parentItem->setText(0,str);
            parentItem->setIcon(0,m_expandIcon);
            parentItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_groupItems.insert(str,parentItem);
            m_groups.append(parentItem);
        }
    }
    else
    {
        parentItem=m_propertyToItem.value(parent);
    }
    if(parentItem==NULL)
    {
        return;
    }
    QTreeWidgetItem *item=new QTreeWidgetItem(parentItem);
    item->setText(0,property->getAttribute("show_name").toString());
    item->setToolTip(0,property->getAttribute("show_name").toString());
    item->setText(1,property->get_value_text());
    item->setToolTip(1,property->get_value_text());
    item->setIcon(1,property->get_value_icon());
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    m_itemToProperty.insert(item,property);
    m_propertyToItem.insert(property,item);
    connect(property,SIGNAL(value_chaged(QVariant,QVariant)),this,SLOT(property_changed()));
    connect(property,SIGNAL(edit_value(QVariant)),this,SLOT(property_edit(QVariant)));

    QList<QAbstractProperty*>    pros=property->getChildren();
    foreach(QAbstractProperty* pro,pros)
    {
        insertItem(pro,property);
    }
}

QTreeWidgetItem *QPropertyListView::indexToItem(const QModelIndex &index) const
{
    return itemFromIndex(index);
}

QAbstractProperty  * QPropertyListView::itemToProperty(QTreeWidgetItem *item) const
{
    return m_itemToProperty.value(item,NULL);
}

bool QPropertyListView::isGroupItem(QTreeWidgetItem *item)const
{
    return m_groupItems.values().contains(item);
}

int QPropertyListView::groupIndex(QTreeWidgetItem *item) const
{
    return m_groups.indexOf(item);
}

void QPropertyListView::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);

    QTreeWidgetItem *item=itemAt(event->pos());

    if(item)
    {
        if(m_groupItems.values().contains(item))
        {
            if(event->pos().x()+header()->offset()<20)
            {
                item->setExpanded(!item->isExpanded());
            }
        }
        else
        {
            if(header()->logicalIndexAt(event->pos())==1 && event->button()==Qt::LeftButton)
            {
                editItem(item,1);
            }
        }
    }
}

void QPropertyListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QModelIndex temp=index;
    while(temp.isValid() && !isGroupItem(indexToItem(temp)))
    {
        temp=temp.parent();
    }
    QTreeWidgetItem* item=indexToItem(index);
    QTreeWidgetItem* tempItem=indexToItem(temp);
    QStyleOptionViewItemV3 opt=options;
    QColor c;
    if(item!=NULL)
    {
        QAbstractProperty  *p=this->itemToProperty(item);
        if(p!=NULL)
        {

            c=m_colors.at(groupIndex(tempItem)%m_colors.count()).first;
            painter->fillRect(options.rect,c);
            opt.palette.setColor(QPalette::AlternateBase,c.lighter(112));
        }
        else
        {
            c=opt.palette.color(QPalette::Dark);
            painter->fillRect(options.rect,c);
            opt.palette.setColor(QPalette::AlternateBase,c);
        }
    }


    QTreeWidget::drawRow(painter,opt,index);

    QColor color=static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor));
    painter->setPen(color);
    painter->drawLine(opt.rect.x(),opt.rect.bottom(),opt.rect.right(),opt.rect.bottom());

}

void QPropertyListView::property_changed()
{
    QAbstractProperty* pro=(QAbstractProperty*)sender();
    QTreeWidgetItem *item=m_propertyToItem.value(pro);
    if(item==NULL)
    {
        return;
    }
    item->setText(1,pro->get_value_text());
    item->setToolTip(1,pro->get_value_text());
    item->setIcon(1,pro->get_value_icon());
    this->update(indexFromItem(item));
    emit active();
}

void QPropertyListView::property_edit(const QVariant &value)
{
    emit notifyPropertyEdit((QAbstractProperty*)sender(),value);
}

void QPropertyListView::setUndoStack(QUndoStack *stack)
{
    m_undo_stack=stack;
}
