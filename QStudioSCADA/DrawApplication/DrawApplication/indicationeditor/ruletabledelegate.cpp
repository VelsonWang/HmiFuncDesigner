#include "ruletabledelegate.h"
#include "coloreditor.h"
#include <QColor>
#include <QLineEdit>

RuleTableDelegate::RuleTableDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *RuleTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    if (index.isValid()) {

        if (index.column() == 0 || index.column() == 2) {
            QLineEdit *lineEdit = new QLineEdit;
            return lineEdit;
        }
        else if (index.column() == 1 || index.column() == 3 || index.column() == 4) {
            ColorEditor *editor = new ColorEditor(parent);
            editor->setColor(QColor());
            return editor;
        }
    }

    return QItemDelegate::createEditor(parent,option,index);
}

void RuleTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

    if (index.isValid() && editor) {

        if (index.column() == 1 || index.column() == 3 || index.column() == 4) {
            ColorEditor *tmpWidget = qobject_cast<ColorEditor*>(editor);
            QColor value = index.model()->data(index,Qt::DisplayRole).value<QColor>();
            tmpWidget->setColor(value);
        }
        else if (index.column() == 0 || index.column() == 2) {
            QLineEdit *tmpWidget = qobject_cast<QLineEdit*>(editor);
            QString value = index.model()->data(index,Qt::DisplayRole).toString();
            tmpWidget->setText(value);
        }
    }
    else {
        QItemDelegate::setEditorData(editor,index);
    }
}

void RuleTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {

    if (index.isValid() && editor) {

        if (index.column() == 1 || index.column() == 3 || index.column() == 4) {
            ColorEditor *tmpWidget = qobject_cast<ColorEditor*>(editor);
            model->setData(index,tmpWidget->getColor(),Qt::EditRole);
        }
        else if (index.column() == 0 || index.column() == 2) {
            QLineEdit *tmpWidget = qobject_cast<QLineEdit*>(editor);
            model->setData(index,tmpWidget->text(),Qt::EditRole);
        }
    }
    else {
        QItemDelegate::setModelData(editor,model,index);
    }
}

void RuleTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
