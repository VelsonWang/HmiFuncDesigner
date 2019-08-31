
#include "variantmanager.h"
#include <QDebug>

class FunctionPropertyType
{
};

Q_DECLARE_METATYPE(FunctionPropertyType)

int VariantManager::functionTypeId()
{
    return qMetaTypeId<FunctionPropertyType>();
}

bool VariantManager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == functionTypeId())
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int VariantManager::valueType(int propertyType) const
{
    if (propertyType == functionTypeId())
        return QVariant::String;
    return QtVariantPropertyManager::valueType(propertyType);
}

QVariant VariantManager::value(const QtProperty *property) const
{
    if (theValues.contains(property))
        return theValues[property].value;
    return QtVariantPropertyManager::value(property);
}

QStringList VariantManager::attributes(int propertyType) const
{
    if (propertyType == functionTypeId()) {
        QStringList attr;
        attr << QLatin1String("supportevents");
        return attr;
    }
    return QtVariantPropertyManager::attributes(propertyType);
}

int VariantManager::attributeType(int propertyType, const QString &attribute) const
{
    if (propertyType == functionTypeId()) {
        if (attribute == QLatin1String("supportevents"))
            return QVariant::String;
        return 0;
    }
    return QtVariantPropertyManager::attributeType(propertyType, attribute);
}

QVariant VariantManager::attributeValue(const QtProperty *property, const QString &attribute)
{
    if (theValues.contains(property)) {
        if (attribute == QLatin1String("supportevents"))
            return theValues[property].attribute;
        return QVariant();
    }
    return QtVariantPropertyManager::attributeValue(property, attribute);
}

QString VariantManager::valueText(const QtProperty *property) const
{
    if (theValues.contains(property))
        return theValues[property].value;
    return QtVariantPropertyManager::valueText(property);
}

void VariantManager::setValue(QtProperty *property, const QVariant &val)
{
    if (theValues.contains(property)) {
        if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
            return;
        QString str = val.value<QString>();
        Data d = theValues[property];
        if (d.value == str)
            return;
        d.value = str;
        theValues[property] = d;
        emit propertyChanged(property);
        emit valueChanged(property, str);
        return;
    }
    QtVariantPropertyManager::setValue(property, val);
}

void VariantManager::setAttribute(QtProperty *property,
                const QString &attribute, const QVariant &val)
{
    if (theValues.contains(property)) {
        if (attribute == QLatin1String("supportevents")) {
            qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
            if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
                return;
            QString str = val.value<QString>();
            Data d = theValues[property];
            qDebug() << __FILE__ << __LINE__ << __FUNCTION__
                     << attribute
                     << d.attribute
                     << str;
            if (d.attribute == str)
                return;
            qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
            d.attribute = str;
            theValues[property] = d;
            emit attributeChanged(property, attribute, str);
        }
        return;
    }
    QtVariantPropertyManager::setAttribute(property, attribute, val);
}

void VariantManager::initializeProperty(QtProperty *property)
{
    if (propertyType(property) == functionTypeId())
        theValues[property] = Data();
    QtVariantPropertyManager::initializeProperty(property);
}

void VariantManager::uninitializeProperty(QtProperty *property)
{
    theValues.remove(property);
    QtVariantPropertyManager::uninitializeProperty(property);
}

