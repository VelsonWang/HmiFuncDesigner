#include "functionproperty.h"

FunctionProperty::FunctionProperty(const QString &pname)
    : Property(pname)
{
    value.setValue(QStringList());
    value.convert(QVariant::StringList);
}

QVariant FunctionProperty::data(int column, int role)
{
    if (column == ColumnData && (Qt::DisplayRole == role)) {
        return value.toStringList().join("|");
    } else if(Qt::EditRole == role) {
        return QVariant();
    } else {
        return Property::data(column, role);
    }
}

QWidget* FunctionProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate) {

    Q_UNUSED(options)
    Q_UNUSED(delegate)

    FunctionPropertyEditor *tmpWidget = new FunctionPropertyEditor(parent);
    tmpWidget->setSupportEvents(supportEvents_);
    tmpWidget->setFunctions(value.toStringList());
    setPropertyEditor(tmpWidget);

    return tmpWidget;
}

bool FunctionProperty::setEditorData(QWidget* editor)
{
    FunctionPropertyEditor* tmpWidget = qobject_cast<FunctionPropertyEditor*>(editor);
    if(tmpWidget)
        tmpWidget->setFunctions(value.toStringList());
    else
        return false;

    return true;
}

QVariant FunctionProperty::getEditorData(QWidget* editor) const
{
    FunctionPropertyEditor* tmpWidget = qobject_cast<FunctionPropertyEditor*>(editor);
    if(tmpWidget)
        return tmpWidget->getFunctions();

    return QVariant();
}

/**
 * @brief FunctionProperty::setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void FunctionProperty::setSupportEvents(QStringList events) {
    supportEvents_ = events;
}
