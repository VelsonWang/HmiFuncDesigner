#ifndef SWITCHBUTTONPLUGIN_H
#define SWITCHBUTTONPLUGIN_H

#include <QObject>
#include "../IDrawApplicationPlugin/IDrawApplicationPlugin.h"


class SwitchButtonPlugin : public QObject, IDrawApplicationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DrawApplicationPluginInterface_iid FILE "SwitchButton.json")
    Q_INTERFACES(IDrawApplicationPlugin)

public:
    SwitchButtonPlugin();

    /////////////////////<plugin operation>////////////////////////////

    // 插件初始化
    void initialize() Q_DECL_OVERRIDE;
    // 插件类型名称
    QString getPluginTypeName() Q_DECL_OVERRIDE;
    // 插件名称
    QString getPluginName() Q_DECL_OVERRIDE;
    // 获取元素的图标
    QIcon getElementIcon() Q_DECL_OVERRIDE;
    // 获取元素名称
    QString getElementName() Q_DECL_OVERRIDE;
    // 获取元素ID
    int getElementID() Q_DECL_OVERRIDE;
    // 获取元素ID String
    QString getElementIDString() Q_DECL_OVERRIDE;
    // 创建元素
    Element* createElement(const QString &szProjPath,
                           const QString &szProjName,
                           QtVariantPropertyManager *propertyMgr) Q_DECL_OVERRIDE;

    /////////////////////<plugin operation>////////////////////////////



};

#endif // SWITCHBUTTONPLUGIN_H
