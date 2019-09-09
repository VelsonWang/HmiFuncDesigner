
#include "EIndicationLamp.h"
#include "ElementIndicationLamp.h"


EIndicationLamp::EIndicationLamp()
{

}


/**
 * @brief EIndicationLamp::initialize
 * @details 初始化
 */
void EIndicationLamp::initialize()
{

}


/**
 * @brief EIndicationLamp::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EIndicationLamp::getElementName()
{
    return tr("指示灯");
}

/**
 * @brief EIndicationLamp::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EIndicationLamp::getElementID()
{
    return IndicationLampItemType;
}


/**
 * @brief EIndicationLamp::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EIndicationLamp::getElementIDString()
{
    return "IndicationLamp";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EIndicationLamp::createElement(QWidget *owner)
{
    ElementIndicationLamp *pEle = new ElementIndicationLamp();
    pEle->setOwnerWidget(owner);
    return pEle;
}
