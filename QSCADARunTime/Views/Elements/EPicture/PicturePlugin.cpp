
#include "PicturePlugin.h"
#include "ElementPicture.h"


PicturePlugin::PicturePlugin()
{

}


/**
 * @brief PicturePlugin::initialize
 * @details 插件初始化
 */
void PicturePlugin::initialize()
{

}

/**
 * @brief PicturePlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString PicturePlugin::getPluginTypeName()
{
    return trUtf8("基本控件");
}


/**
 * @brief PicturePlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString PicturePlugin::getPluginName()
{
    return trUtf8("图片");
}


/**
 * @brief PicturePlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon PicturePlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/Picture.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief PicturePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString PicturePlugin::getElementName()
{
    return trUtf8("图片");
}

/**
 * @brief PicturePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int PicturePlugin::getElementID()
{
    return PictureItemType;
}


/**
 * @brief PicturePlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString PicturePlugin::getElementIDString()
{
    return "Picture";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* PicturePlugin::createElement()
{
    return new ElementPicture();
}
