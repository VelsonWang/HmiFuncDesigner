#include "propertymodel.h"

PropertyModel::PropertyModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    headlineProperty.append(tr("属性"));
    headlineValue.append(tr("值"));
}

void PropertyModel::addProperty(Property *prop) {
    if (!prop) {
        return;
    }
    beginInsertRows(QModelIndex(), propsList.size(), propsList.size());
    propsList.append(prop);
    endInsertRows();
}

QVariant PropertyModel::data(const QModelIndex &index, int role) const {

    Property *tmpProperty = getProperty(index);

    if (!tmpProperty) {
        return QVariant();
    }
    else {
        return tmpProperty->data(index.column(),role);
    }
}


bool PropertyModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    Property *tmpProperty = getProperty(index);
    if (index.column() == 1 && tmpProperty) {
        tmpProperty->setData(value,role);
        emit onDataChangedByEditor(tmpProperty);
        emit dataChanged(index,index);
    }
    return true;
}

Qt::ItemFlags PropertyModel::flags(const QModelIndex &index) const {
    Property *tmpProperty = getProperty(index);
    if (!tmpProperty) {
        return Qt::NoItemFlags;
    }
    else {
        return tmpProperty->flags(index.column());
    }
}

QVariant PropertyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) return headlineProperty;
        else if (section == 1) return headlineValue;
    }
    else
        return QVariant();

    return QVariant();
}

int PropertyModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return propsList.size();
}

int PropertyModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

Property *PropertyModel::getProperty(const QModelIndex &index) const {
    if (index.isValid() && index.row() < propsList.count()) {
        Property *prop = propsList[index.row()];
        if (prop) {
            return prop;
        }
    }
    return nullptr;
}

Property *PropertyModel::getProperty(int row) const {
    if (row < propsList.count()) {
        Property *prop = propsList[row];
        if (prop) {
            return prop;
        }
    }
    return nullptr;
}

void PropertyModel::resetModel() {
    beginResetModel();
    propsList.clear();
    endResetModel();
}
