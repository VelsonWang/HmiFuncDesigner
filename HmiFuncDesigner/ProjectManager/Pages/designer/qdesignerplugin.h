#ifndef QDESIGNERPLUGIN_H
#define QDESIGNERPLUGIN_H

#include "../../libs/core/qabstractpage.h"

#define DESIGNER_PLUGIN_NAME "Plugin.Page.Designer"

class QDesignerPlugin : public QAbstractPage
{
    Q_OBJECT
    Q_INTERFACES(QPluginInterface)
    Q_PLUGIN_METADATA(IID QPluginInterface_iid FILE "qdesignerplugin.json")
public:
    QString     get_plugin_name();//插件名称
    QString     get_page_name();//获取页面的名称
    QIcon       get_page_icon();//获取页面的图标
    int         get_index();//获取页面索引，用于在标签中的位置
    bool        get_enabled();//工程关闭的时候是否启用这个页面
protected:
    void        create_widget();
};

#endif // QPROJECTPLUGIN_H
