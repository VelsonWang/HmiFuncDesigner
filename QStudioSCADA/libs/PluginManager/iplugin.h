#ifndef IPLUGIN
#define IPLUGIN


#include <QString>
#include <QObject>

class IPlugin
{
public:
    virtual ~IPlugin() {}
    // 插件初始化
    virtual void Initialize() = 0;
    // 插件类型
    virtual QString GetPluginType() = 0;
    // 插件名称
    virtual QString GetPluginName() = 0;


};



QT_BEGIN_NAMESPACE

#define IPlugin_iid "QStudioSCADA.PluginInterface"

Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)
QT_END_NAMESPACE





#endif // IPLUGIN

