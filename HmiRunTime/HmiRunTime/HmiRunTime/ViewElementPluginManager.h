#ifndef VIEWELEMENTPLUGINMANAGER_H
#define VIEWELEMENTPLUGINMANAGER_H

#include <QMap>
class IHmiRunTimeViewPlugin;


class ViewElementPluginManager
{
public:
    static ViewElementPluginManager *getInstance() {
        static ViewElementPluginManager instance;
        return &instance;
    }

private:
    ViewElementPluginManager();
    ~ViewElementPluginManager();

public:
    void loadAllPlugin();
    IHmiRunTimeViewPlugin* loadPlugin(const QString &name);
    IHmiRunTimeViewPlugin* getPlugin(const QString &name);
    void releasePlugin();

    // 获取匹配元素名称的插件对象
    IHmiRunTimeViewPlugin* getPluginByElementName(const QString &szName);
    // 获取匹配元素ID的插件对象
    IHmiRunTimeViewPlugin* getPluginByElementID(int &iID);
    // 获取匹配元素ID String的插件对象
    IHmiRunTimeViewPlugin* getPluginByElementIDString(const QString &szIDString);

public:
    //------name-------plugin
    QMap<QString, IHmiRunTimeViewPlugin*> m_plugins;

};

#endif // VIEWELEMENTPLUGINMANAGER_H
