#include "EPicturePlugin.h"
#include "ElementPicture.h"


EPicturePlugin::EPicturePlugin()
{

}


/**
 * @brief EPicturePlugin::initialize
 * @details 插件初始化
 */
void EPicturePlugin::initialize()
{

}


/**
 * @brief EPicturePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EPicturePlugin::getElementName()
{
    return tr("图片");
}

/**
 * @brief EPicturePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EPicturePlugin::getElementID()
{
    return PictureItemType;
}


/**
 * @brief EPicturePlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EPicturePlugin::getElementIDString()
{
    return "Picture";
}

/**
 * @brief EPicturePlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EPicturePlugin::createElement(QWidget *owner)
{
    ElementPicture *pEle = new ElementPicture();
    pEle->setOwnerWidget(owner);
    return pEle;
}


