#ifndef RTDBPLUGIN_H
#define RTDBPLUGIN_H

#include "../../libs/core/qabstractpage.h"

#define PLUGIN_NAME "Plugin.Page.RTDBWin"

class RTDBPlugin : public QAbstractPage
{
    Q_OBJECT
    Q_INTERFACES(QPluginInterface)
    Q_PLUGIN_METADATA(IID QPluginInterface_iid FILE "rtdbplugin.json")
public:
    QString getPluginName(); // 插件名称
    QString getPageName(); // 获取页面的名称
    QIcon getPageIcon(); // 获取页面的图标
    int getIndex(); // 获取页面索引，用于在标签中的位置
    bool getEnabled(); // 工程关闭的时候是否启用这个页面

protected:
    void createWidget();
};

#endif
