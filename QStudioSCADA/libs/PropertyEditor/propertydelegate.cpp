#include "propertydelegate.h"
#include <QApplication>
#include <QPainter>
#include <QtDebug>
#include "property.h"

PropertyDelegate::PropertyDelegate(QObject *parent) :
    QStyledItemDelegate(parent),rowHeight(0),addRowHeight(false)
{
}

QWidget *PropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QWidget *tmpWidget = NULL;

    if (index.isValid()) {
        Property *tmpProperty = reinterpret_cast<Property*>(index.internalPointer());
        tmpWidget = tmpProperty->createEditor(parent,option,this);
    }

    return tmpWidget ? tmpWidget : QStyledItemDelegate::createEditor(parent,option,index);
}

void PropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

    bool done = false;

    if (index.isValid() && editor) {
        Property *tmpProperty = reinterpret_cast<Property*>(index.internalPointer());
        done = tmpProperty->setEditorData(editor);
    }

    if (!done) {
        QStyledItemDelegate::setEditorData(editor,index);
    }
}

void PropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {

    QVariant tmpData;

    if (index.isValid() && editor) {

        Property *tmpProperty = reinterpret_cast<Property*>(index.internalPointer());
        tmpData = tmpProperty->getEditorData(editor);
    }

    if (tmpData.isNull()) {

        QStyledItemDelegate::setModelData(editor,model,index);
    }
    else {
        model->setData(index,tmpData);
    }
}

void PropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    bool done = false;
    if(index.isValid() && index.column() == 1)
        done = reinterpret_cast<Property*>(index.internalPointer())->paint(painter, option, index, this);

    if(!done)
        QStyledItemDelegate::paint(painter, option, index);

    QColor tmpPenColor = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor,&option));
    QPen tmpOldPen = painter->pen();
    painter->setPen(QPen(tmpPenColor));
    painter->drawLine(option.rect.right(), option.rect.y(), option.rect.right(), option.rect.bottom());
    painter->drawLine(option.rect.x(), option.rect.bottom(), option.rect.right(), option.rect.bottom());
    painter->setPen(tmpOldPen);
}

QSize PropertyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QSize tmpStandardSizeHint = QStyledItemDelegate::sizeHint(option, index);
    tmpStandardSizeHint.setHeight(tmpStandardSizeHint.height() + 1);

    if(rowHeight > 0)
        return QSize(tmpStandardSizeHint.width(), addRowHeight ? tmpStandardSizeHint.height() + rowHeight : rowHeight);
    else
        return tmpStandardSizeHint;
}
