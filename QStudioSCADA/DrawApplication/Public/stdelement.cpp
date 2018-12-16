
#include "IElement.h"

StdElement::StdElement()
{
    m_iGraphType = GRAPH_NONE;
    m_bIsSelected = m_bMouseLeftButtonDown = false;
    m_ptMouseLeftButtonNew = m_ptMouseLeftButtonOld = QPoint(0, 0);
}


StdElement::~StdElement()
{

}

/*
* 加载元素数据
*/
void StdElement::StdLoad(const QJsonObject &json)
{
    m_iGraphType = (tGraphType)json["GraphType"].toInt();
    m_strName = json["Name"].toString();
    m_bIsVisible = json["IsVisible"].toBool();
    m_iX = json["X"].toInt();
    m_iY = json["Y"].toInt();
    m_iZ = json["Z"].toInt();
}


/*
* 保存元素数据
*/
void StdElement::StdSave(QJsonObject &json)
{
    json["GraphType"] = (int)m_iGraphType;
    json["Name"] = m_strName;
    json["IsVisible"] = m_bIsVisible;
    json["X"] = m_iX;
    json["Y"] = m_iY;
    json["Z"] = m_iZ;
}


/*
*  增加属性
*/
void StdElement::addProperty(QtTreePropertyBrowser *propertyBrowser,
                             QtVariantProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyBrowser->addProperty(property);
    if (idToExpanded.contains(id))
        propertyBrowser->setExpanded(item, idToExpanded[id]);
}


bool StdElement::event(QEvent *e)
{
    return QObject::event(e);
}

