#include "introspection.h"

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

#include "inflection.h"

QString tableNameForObject(QObject* o)
{
    const QMetaObject* meta = o->metaObject();
    QString tableName = meta->className();
    tableName = Utils::tableize(tableName);
    return tableName;
}

QList<QString> propertyListForObject(QObject* o)
{
    QList<QString> list;

    const QMetaObject* meta = o->metaObject();
    for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
        list << QString::fromLatin1(meta->property(i).name());
    }

    return list;
}

QVariantHash  propertyValuesForObject(QObject* o)
{
    QVariantHash hash;

    const QMetaObject* meta = o->metaObject();
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty property = meta->property(i);
        QString propertyName = QString::fromLatin1(property.name());
        if (propertyName == "objectName") {
            continue;
        }

        hash.insert(propertyName, property.read(o));
    }
    return hash;
}



