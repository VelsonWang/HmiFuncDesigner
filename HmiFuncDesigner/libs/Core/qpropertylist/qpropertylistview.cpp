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

QWidget *QPropertyEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    QWidget *wid = NULL;
    if(index.column() == 1) {
        QTreeWidgetItem *item = listView->indexToItem(index);
        QAbstractProperty *property = listView->itemToProperty(item);
        if(property != NULL && property->getAttribute(ATTR_EDITABLE).toBool()) {
            wid = new QPropertyBaseEditor(property, listView->undoStack, parent);
        }
    }
    return wid;
}


void QPropertyEditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect.adjusted(0, 0, 0, -1));
}

QSize QPropertyEditorDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    size.setHeight(21);
    size.rwidth() += 4;
    return size;
}

void QPropertyEditorDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(model)
    Q_UNUSED(index)
}

void QPropertyEditorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTreeWidgetItem* item = listView->indexToItem(index);
    QStyleOptionViewItem opt = option;
    QColor c;
    if(item != NULL) {
        QAbstractProperty *p = listView->itemToProperty(item);
        if(p != NULL) {
            if(p->modified() && index.column() == 0 && p->getParent() == NULL) {
                opt.font.setBold(true);
                opt.fontMetrics = QFontMetrics(opt.font);
            }
            QModelIndex temp = index;
            while(temp.isValid() && !listView->isGroupItem(listView->indexToItem(temp))) {
                temp = temp.parent();
            }
            if(temp.isValid()) {
                QTreeWidgetItem *tempItem = listView->indexToItem(temp);
                c = listView->colors.at(listView->groupIndex(tempItem) % listView->colors.count()).first;
            } else {
                c = listView->colors.at(0).first;
            }

            if(opt.features & QStyleOptionViewItem::Alternate) {
                c = c.lighter(112);
            }

        } else {
            c = opt.palette.color(QPalette::Dark);
            opt.palette.setColor(QPalette::Text, opt.palette.color(QPalette::BrightText));
        }
    }

    painter->fillRect(option.rect, c);
    opt.state &= ~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter, opt, index);
    opt.palette.setCurrentColorGroup(QPalette::Active);
    QColor color = static_cast<QRgb>(qApp->style()->styleHint(QStyle::SH_Table_GridLineColor, &opt));
    painter->setPen(color);

    if(!listView->isGroupItem(item)) {
        if(index.column() == 0) {
            int right = option.direction == Qt::LeftToRight ? option.rect.right() : option.rect.left();
            painter->drawLine(right, option.rect.y(), right, option.rect.bottom());
        }
    }
}


QPropertyListView::QPropertyListView(QWidget *parent) : QTreeWidget(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setIconSize(QSize(18, 18));
    setColumnCount(2);
    QStringList labels;
    labels.append(tr("属性")); // tr("Property")
    labels.append(tr("值")); // tr("Value")
    setHeaderLabels(labels);
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setRootIsDecorated(false);
    //header()->setMovable(false);

    QVector<QColor> vectColors;
    vectColors.reserve(6);
    vectColors.push_back(QColor(255, 230, 191));
    vectColors.push_back(QColor(255, 255, 191));
    vectColors.push_back(QColor(191, 255, 191));
    vectColors.push_back(QColor(199, 255, 255));
    vectColors.push_back(QColor(234, 191, 255));
    vectColors.push_back(QColor(255, 191, 239));
    colors.reserve(vectColors.count());
    const int darknessFactor = 250;
    for(int i = 0; i < vectColors.count(); i++) {
        QColor c = vectColors.at(i);
        colors.push_back(qMakePair(c, c.darker(darknessFactor)));
    }

    delegate = new QPropertyEditorDelegate(this);
    setItemDelegate(delegate);

    expandIcon = StyleHelper::drawIndicatorIcon(this->palette(), this->style());

    QPalette p = this->palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    this->setPalette(p);
}

void QPropertyListView::setPropertys(const QList<QAbstractProperty *> &props)
{
    clearAll();
    propertys = props;
    updateView();
    this->expandAll();
}

void QPropertyListView::clearAll()
{
    foreach(QAbstractProperty* pro, propertys) {
        disconnect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(onPropertyChanged()));
        disconnect(pro, SIGNAL(edit_value(QVariant)), this, SLOT(onPropertyEdit(QVariant)));
    }

    this->clear();
    groupItems.clear();
    propertyToWidgetItem.clear();
    widgetItemToProperty.clear();
    groups.clear();
}

void QPropertyListView::updateView()
{
    foreach(QAbstractProperty* pro, propertys) {
        insertItem(pro);
    }
}

void QPropertyListView::insertItem(QAbstractProperty *property, QAbstractProperty *parent)
{
    if(property == NULL) {
        return;
    }
    if(!property->getAttribute(ATTR_VISIBLE).toBool()) {
        return;
    }
    QTreeWidgetItem *parentItem;
    if(parent == NULL) {
        QString str = property->getAttribute("group").toString();
        if(str == "") {
            str = tr("普通"); // tr("Common")
        }
        parentItem = groupItems.value(str);
        if(parentItem == NULL) {
            parentItem = new QTreeWidgetItem(this);
            parentItem->setText(0, str);
            parentItem->setIcon(0, expandIcon);
            parentItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            groupItems.insert(str, parentItem);
            groups.append(parentItem);
        }
    } else {
        parentItem = propertyToWidgetItem.value(parent);
    }
    if(parentItem == NULL) {
        return;
    }
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
    item->setText(0, property->getAttribute("show_name").toString());
    item->setToolTip(0, property->getAttribute("show_name").toString());
    item->setText(1, property->getValueText());
    item->setToolTip(1, property->getValueText());
    item->setIcon(1, property->getValueIcon());
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    widgetItemToProperty.insert(item, property);
    propertyToWidgetItem.insert(property, item);
    connect(property, SIGNAL(value_chaged(QVariant, QVariant)),
            this, SLOT(onPropertyChanged()));
    connect(property, SIGNAL(edit_value(QVariant)),
            this, SLOT(onPropertyEdit(QVariant)));

    QList<QAbstractProperty*> pros = property->getChildren();
    foreach(QAbstractProperty* pro, pros) {
        insertItem(pro, property);
    }
}

QTreeWidgetItem *QPropertyListView::indexToItem(const QModelIndex &index) const
{
    return itemFromIndex(index);
}

QAbstractProperty  *QPropertyListView::itemToProperty(QTreeWidgetItem *item) const
{
    return widgetItemToProperty.value(item, NULL);
}

bool QPropertyListView::isGroupItem(QTreeWidgetItem *item)const
{
    return groupItems.values().contains(item);
}

int QPropertyListView::groupIndex(QTreeWidgetItem *item) const
{
    return groups.indexOf(item);
}

void QPropertyListView::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);

    QTreeWidgetItem *item = itemAt(event->pos());
    if(item) {
        if(groupItems.values().contains(item)) {
            if(event->pos().x() + header()->offset() < 20) {
                item->setExpanded(!item->isExpanded());
            }
        } else {
            if(header()->logicalIndexAt(event->pos()) == 1 && event->button() == Qt::LeftButton) {
                editItem(item, 1);
            }
        }
    }
}

void QPropertyListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QModelIndex temp = index;
    while(temp.isValid() && !isGroupItem(indexToItem(temp))) {
        temp = temp.parent();
    }
    QTreeWidgetItem* item = indexToItem(index);
    QTreeWidgetItem* tempItem = indexToItem(temp);
    QStyleOptionViewItem opt = options;
    QColor c;
    if(item != NULL) {
        QAbstractProperty *p = this->itemToProperty(item);
        if(p != NULL) {

            c = colors.at(groupIndex(tempItem) % colors.count()).first;
            painter->fillRect(options.rect, c);
            opt.palette.setColor(QPalette::AlternateBase, c.lighter(112));
        } else {
            c = opt.palette.color(QPalette::Dark);
            painter->fillRect(options.rect, c);
            opt.palette.setColor(QPalette::AlternateBase, c);
        }
    }

    QTreeWidget::drawRow(painter, opt, index);

    QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor));
    painter->setPen(color);
    painter->drawLine(opt.rect.x(), opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());

}

void QPropertyListView::onPropertyChanged()
{
    QAbstractProperty* pro = (QAbstractProperty*)sender();
    QTreeWidgetItem *item = propertyToWidgetItem.value(pro);
    if(item == NULL) {
        return;
    }
    item->setText(1, pro->getValueText());
    item->setToolTip(1, pro->getValueText());
    item->setIcon(1, pro->getValueIcon());
    this->update(indexFromItem(item));
    emit active();
}

void QPropertyListView::onPropertyEdit(const QVariant &value)
{
    emit notifyPropertyEdit((QAbstractProperty*)sender(), value);
}

void QPropertyListView::setUndoStack(QUndoStack *stack)
{
    undoStack = stack;
}
