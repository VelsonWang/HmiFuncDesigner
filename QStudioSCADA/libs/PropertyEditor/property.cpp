#include "property.h"
#include "propertymodel.h"

Property::Property(const QString &pname)
    : name(pname)
{
}

Property::~Property() {

}

bool Property::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QAbstractItemDelegate *delegate) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(index);
    Q_UNUSED(delegate);

    return false;
}

QVariant Property::data(int column, int role) {

    if (column == ColumnProperty && Qt::DisplayRole == role)
        return QVariant(name);
    else if (column == ColumnData && (Qt::DisplayRole == role || Qt::EditRole == role)) {
        return value;
    }
    else
        return QVariant();
}

bool Property::setData(const QVariant &data, int role) {

    bool tmpResult = false;

    if (Qt::EditRole == role) {
        tmpResult = (value != data);
        setValue(data);
    }

    return tmpResult;
}

QWidget* Property::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate) {

    Q_UNUSED(parent)
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    return NULL;
}

bool Property::setEditorData(QWidget *editor) {

    Q_UNUSED(editor)

    return false;
}

QVariant Property::getEditorData(QWidget *editor) const {

    Q_UNUSED(editor)

    return QVariant();
}

Qt::ItemFlags Property::flags(int column) {

    if (column == ColumnProperty) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else if (column == ColumnData) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    else
        return Qt::NoItemFlags;
}

void Property::setValue(const QVariant &value) {
    this->value = value;
}

QVariant Property::getValue() const {
    return value;
}

void Property::setName(const QString &name) {
    this->name = name;
}

QString Property::getName() const {
    return name;
}

void Property::setId(uint id) {
    this->id = id;
}

uint Property::getId() const {
    return id;
}

void Property::setPropertyModel(PropertyModel *pModel)
{
    pPropertyModel_ = pModel;
}

PropertyModel *Property::getPropertyModel()
{
    return pPropertyModel_;
}
