
#include "InputEditPlugin.h"
#include "ElementInputEdit.h"


InputEditPlugin::InputEditPlugin()
{

}


/**
 * @brief InputEditPlugin::initialize
 * @details 插件初始化
 */
void InputEditPlugin::initialize()
{

}

/**
 * @brief InputEditPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString InputEditPlugin::getPluginTypeName()
{
    return trUtf8("基本控件");
}


/**
 * @brief InputEditPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString InputEditPlugin::getPluginName()
{
    return trUtf8("输入编辑框");
}


/**
 * @brief InputEditPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon InputEditPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/InputEdit.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief InputEditPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString InputEditPlugin::getElementName()
{
    return trUtf8("输入编辑框");
}

/**
 * @brief InputEditPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int InputEditPlugin::getElementID()
{
    return InputEditItemType;
}


/**
 * @brief InputEditPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString InputEditPlugin::getElementIDString()
{
    return "InputEdit";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* InputEditPlugin::createElement(const QString &projPath)
{
    return new ElementInputEdit(projPath);
}
