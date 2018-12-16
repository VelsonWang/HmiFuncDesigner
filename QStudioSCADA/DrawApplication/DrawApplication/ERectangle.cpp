#include "ERectangle.h"

ERectangle::ERectangle()
{

}


ERectangle::~ERectangle()
{

}


/*
* 获取元素操作接口
*/
IElement* ERectangle::GetInterface()
{
    return static_cast<IElement *>(this);
}


/*
* 加载元素数据
*/
void ERectangle::load(QDomDocument &doc, QDomElement &parent)
{

}


/*
* 保存元素数据
*/
void ERectangle::save(QDomDocument &doc, QDomElement &parent)
{
    StdSave(doc, parent);

}


/*
* 绘制图形元素
*/
void ERectangle::darw(QPainter &painter)
{

}




/*
* 单击事件响应
*/
void ERectangle::mousePress(QMouseEvent * event)
{


}


/*
* 双击事件响应
*/
void ERectangle::mouseDoubleClick(QMouseEvent * event)
{


}


/*
* 左键拖动响应
*/
void ERectangle::mouseMove(QMouseEvent * event)
{


}


/*
* 鼠标释放
*/
void ERectangle::mouseRelease(QMouseEvent * event)
{


}


/*
* 元素是否选中
*/
bool ERectangle::isSelected()
{
    return m_bIsSelected;
}


/*
* 获取元素属性
*/
void ERectangle::GetElementProperty(QtTreePropertyBrowser *propertyBrowser,
                                    QtVariantPropertyManager *variantManager,
                                    QtVariantEditorFactory *variantFactory)
{
    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd())
    {
        delete itProp.key();
        itProp++;
    }
    propertyToId.clear();
    idToProperty.clear();

    propertyBrowser->clear();

    QtVariantProperty *propGraphType = variantManager->addProperty(QVariant::String, tr("图形类型"));
    propGraphType->setValue("线");
    addProperty(propertyBrowser, propGraphType, QLatin1String("图形类型"));

    QtVariantProperty *propName = variantManager->addProperty(QVariant::String, tr("图形名称"));
    propName->setValue(m_strName);
    addProperty(propertyBrowser, propName, QLatin1String("图形名称"));

    QtVariantProperty *propIsVisible = variantManager->addProperty(QVariant::Bool, tr("是否可见"));
    propIsVisible->setValue(m_bIsVisible);
    addProperty(propertyBrowser, propIsVisible, QLatin1String("是否可见"));

    QtVariantProperty *propX = variantManager->addProperty(QVariant::Int, tr("X坐标"));
    propX->setValue(m_iX);
    addProperty(propertyBrowser, propX, QLatin1String("xX坐标"));

    QtVariantProperty *propY = variantManager->addProperty(QVariant::Int, tr("Y坐标"));
    propY->setValue(m_iY);
    addProperty(propertyBrowser, propY, QLatin1String("Y坐标"));

    QtVariantProperty *propZ = variantManager->addProperty(QVariant::Int, tr("Z坐标"));
    propZ->setValue(m_iZ);
    addProperty(propertyBrowser, propZ, QLatin1String("Z坐标"));

    QtVariantProperty *propWidth = variantManager->addProperty(QVariant::Int, tr("宽度"));
    propWidth->setValue(m_iWidth);
    addProperty(propertyBrowser, propWidth, QLatin1String("宽度"));

    QtVariantProperty *propHeight = variantManager->addProperty(QVariant::Int, tr("高度"));
    propHeight->setValue(m_iHeight);
    addProperty(propertyBrowser, propHeight, QLatin1String("高度"));

    propertyBrowser->setFactoryForManager(variantManager, variantFactory);
    propertyBrowser->setPropertiesWithoutValueMarked(true);
    propertyBrowser->setRootIsDecorated(false);

}


/*
* 插槽： 属性值改变
*/
void ERectangle::PropertyValueChanged(QtProperty *property, const QVariant &value)
{
    if (!propertyToId.contains(property))
        return;

//    if (!currentItem)
//        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("图形类型"))
    {
        m_iGraphType = GRAPH_LINE;
    }
    else if (id == QLatin1String("图形名称"))
    {
        m_strName = value.toString();
    }
    else if (id == QLatin1String("是否可见"))
    {
        m_bIsVisible = value.toBool();
    }
    else if (id == QLatin1String("X坐标"))
    {
        m_iX = value.toInt();
    }
    if (id == QLatin1String("Y坐标"))
    {
        m_iY =value.toInt();
    }
    else if (id == QLatin1String("Z坐标"))
    {
        m_iZ = value.toInt();
    }
    else if (id == QLatin1String("宽度"))
    {
        m_iWidth = value.toInt();
    }
    else if (id == QLatin1String("高度"))
    {
        m_iHeight = value.toInt();
    }

}

