
#include "variantmanager.h"

class FunctionPropertyType
{
};
Q_DECLARE_METATYPE(FunctionPropertyType)

class TagColorListPropertyType
{
};
Q_DECLARE_METATYPE(TagColorListPropertyType)

class TagTextListPropertyType
{
};
Q_DECLARE_METATYPE(TagTextListPropertyType)



int VariantManager::functionTypeId()
{
    return qMetaTypeId<FunctionPropertyType>();
}

int VariantManager::tagColorListTypeId()
{
    return qMetaTypeId<TagColorListPropertyType>();
}

int VariantManager::TagTextListTypeId()
{
    return qMetaTypeId<TagTextListPropertyType>();
}

bool VariantManager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == functionTypeId() ||
            propertyType == tagColorListTypeId() ||
            propertyType == TagTextListTypeId())
        return true;

    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int VariantManager::valueType(int propertyType) const
{
    if (propertyType == functionTypeId() ||
            propertyType == tagColorListTypeId() ||
            propertyType == TagTextListTypeId())
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
    } else if (propertyType == tagColorListTypeId()) {
        return QtVariantPropertyManager::attributes(propertyType);
    } else if (propertyType == TagTextListTypeId()) {
        return QtVariantPropertyManager::attributes(propertyType);
    }

    return QtVariantPropertyManager::attributes(propertyType);
}

int VariantManager::attributeType(int propertyType, const QString &attribute) const
{
    if (propertyType == functionTypeId()) {
        if (attribute == QLatin1String("supportevents"))
            return QVariant::String;
        return 0;
    } else if (propertyType == tagColorListTypeId()) {
        return QtVariantPropertyManager::attributeType(propertyType, attribute);
    } else if (propertyType == TagTextListTypeId()) {
        return QtVariantPropertyManager::attributeType(propertyType, attribute);
    }

    return QtVariantPropertyManager::attributeType(propertyType, attribute);
}

QVariant VariantManager::attributeValue(const QtProperty *property,
                                        const QString &attribute) const
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
                                  const QString &attribute,
                                  const QVariant &val)
{
    if (theValues.contains(property)) {
        if (attribute == QLatin1String("supportevents")) {
            if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
                return;
            QString str = val.value<QString>();
            Data d = theValues[property];

            if (d.attribute == str)
                return;

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
    if (propertyType(property) == functionTypeId() ||
            propertyType(property) == tagColorListTypeId() ||
            propertyType(property) == TagTextListTypeId()) {
        theValues[property] = Data();
    }
    QtVariantPropertyManager::initializeProperty(property);
}

void VariantManager::uninitializeProperty(QtProperty *property)
{
    theValues.remove(property);
    QtVariantPropertyManager::uninitializeProperty(property);
}

