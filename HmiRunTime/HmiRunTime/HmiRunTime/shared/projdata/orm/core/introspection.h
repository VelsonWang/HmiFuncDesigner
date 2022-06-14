#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include <QtCore/QString>
#include <QtCore/QVariantHash>

class QObject;

QString tableNameForObject(QObject* obj);
QList<QString> propertyListForObject(QObject* obj);
QVariantHash propertyValuesForObject(QObject* obj);

#endif

