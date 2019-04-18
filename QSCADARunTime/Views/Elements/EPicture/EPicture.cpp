
#include "EPicture.h"
#include "ElementPicture.h"


EPicture::EPicture()
{

}


/**
 * @brief EPicture::initialize
 * @details 初始化
 */
void EPicture::initialize()
{

}


/**
 * @brief EPicture::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EPicture::getElementName()
{
    return trUtf8("图片");
}

/**
 * @brief EPicture::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EPicture::getElementID()
{
    return PictureItemType;
}


/**
 * @brief EPicture::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EPicture::getElementIDString()
{
    return "Picture";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EPicture::createElement(QWidget *owner)
{
    ElementPicture *pEle = new ElementPicture();
    pEle->setOwnerWidget(owner);
    return pEle;
}
