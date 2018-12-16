#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QObject>
#include "../IDrawApplicationPlugin/IDrawApplicationPlugin.h"


class ELinePlugin : public QObject, IDrawApplicationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DrawApplicationPluginInterface_iid FILE "ELine.json")
    Q_INTERFACES(IDrawApplicationPlugin)

public:
    ELinePlugin();

    /////////////////////<plugin operation>////////////////////////////

    // 插件初始化
    void Initialize() Q_DECL_OVERRIDE;
    // 插件类型
    QString GetPluginType() Q_DECL_OVERRIDE;
    // 插件ID
    tGraphType GetPluginID() Q_DECL_OVERRIDE;
    // 插件名称
    QString GetPluginName() Q_DECL_OVERRIDE;
    // 获取元素的图标
    QIcon GetElementIcon() Q_DECL_OVERRIDE;
    // 获取元素名称
    QString GetElementName() Q_DECL_OVERRIDE;
    // 创建元素
    StdElement* CreateElement() Q_DECL_OVERRIDE;

    /////////////////////<plugin operation>////////////////////////////



};

#endif // MODBUSRTU_H
