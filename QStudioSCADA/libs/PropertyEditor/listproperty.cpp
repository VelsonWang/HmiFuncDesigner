#include "listproperty.h"
#include <QComboBox>
#include <QListView>

ListProperty::ListProperty(const QString &name)
    : Property(name) {
    value.setValue(QString());
    value.convert(QVariant::String);
}

QWidget *ListProperty::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &options,
                                    const QAbstractItemDelegate *delegate) {
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    QComboBox *tmpEditor = new QComboBox(parent);
    QListView *pListView = new QListView();

    tmpEditor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pListView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    tmpEditor->setView(pListView);
    tmpEditor->addItems(szList_);
    tmpEditor->setCurrentText(value.toString());

    return tmpEditor;
}

QVariant ListProperty::getEditorData(QWidget *editor) const {
    QComboBox *tmpEditor = qobject_cast<QComboBox*>(editor);

    if (tmpEditor) {
        return tmpEditor->currentText();
    }

    return QVariant(QString());
}

void ListProperty::setList(const QStringList &list) {
    szList_ = list;
    if(szList_.count() > 0)
        value.setValue(szList_.at(0));
}
