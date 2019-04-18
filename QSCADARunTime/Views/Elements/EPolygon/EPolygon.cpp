
#include "EPolygon.h"
#include "ElementPolygon.h"


EPolygon::EPolygon()
{

}


/**
 * @brief EPolygon::initialize
 * @details 初始化
 */
void EPolygon::initialize()
{

}


/**
 * @brief EPolygon::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EPolygon::getElementName()
{
    return trUtf8("多边形");
}

/**
 * @brief EPolygon::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EPolygon::getElementID()
{
    return PolygonItemType;
}


/**
 * @brief EPolygon::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EPolygon::getElementIDString()
{
    return "Polygon";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EPolygon::createElement(QWidget *owner)
{
    ElementPolygon *pEle = new ElementPolygon();
    pEle->setOwnerWidget(owner);
    return pEle;
}
