#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <QAbstractTableModel>
#include "property.h"

class PropertyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PropertyModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Property *getProperty(const QModelIndex &) const;
    void addProperty(Property *prop);
    void resetModel();

private:
    QString headlineProperty;
    QString headlineValue;
    QList <Property*> propsList;

signals:
    void onDataChangedByEditor(Property *prop);
};

#endif // PROPERTYMODEL_H
