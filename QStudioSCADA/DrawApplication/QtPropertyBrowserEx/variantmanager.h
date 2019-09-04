
#ifndef VARIANTMANAGER_H
#define VARIANTMANAGER_H

#include "qtvariantproperty.h"


class VariantManager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    VariantManager(QObject *parent = Q_NULLPTR) : QtVariantPropertyManager(parent) { }

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    virtual QStringList attributes(int propertyType) const;
    virtual int attributeType(int propertyType, const QString &attribute) const;
    virtual QVariant attributeValue(const QtProperty *property, const QString &attribute) const;

    static int functionTypeId();
    static int tagColorListTypeId();
    static int TagTextListTypeId();
    static int filePathTypeId();

public slots:
    virtual void setValue(QtProperty *property, const QVariant &val);
    virtual void setAttribute(QtProperty *property,
                const QString &attribute, const QVariant &value);

protected:
    virtual QString valueText(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);

private:
    struct Data {
        QString value;
        QString attribute;
    };
    QMap<const QtProperty *, Data> theValues;
};



#endif
