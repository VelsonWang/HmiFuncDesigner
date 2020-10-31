#include "AnalogClockPlugin.h"
#include "ElementAnalogClock.h"


AnalogClockPlugin::AnalogClockPlugin()
{

}


/**
 * @brief AnalogClockPlugin::initialize
 * @details 插件初始化
 */
void AnalogClockPlugin::initialize()
{

}

/**
 * @brief AnalogClockPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString AnalogClockPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief AnalogClockPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString AnalogClockPlugin::getPluginName()
{
    return tr("模拟时钟");
}


/**
 * @brief AnalogClockPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon AnalogClockPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/AnalogClock.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief AnalogClockPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString AnalogClockPlugin::getElementName()
{
    return tr("模拟时钟");
}

/**
 * @brief AnalogClockPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int AnalogClockPlugin::getElementID()
{
    return AnalogClockItemType;
}


/**
 * @brief AnalogClockPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString AnalogClockPlugin::getElementIDString()
{
    return "AnalogClock";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* AnalogClockPlugin::createElement(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
{
    return new ElementAnalogClock(pProjDataObj, propertyMgr);
}
