#ifndef QABSTRACTPLUGIN_H
#define QABSTRACTPLUGIN_H

#include "sharedlibglobal.h"

#include <QString>
#include <QObject>

class SHAREDLIB_EXPORT QPluginInterface
{
public:
    virtual void initPlugin() = 0; // 初始化插件
    virtual QString getPluginName() = 0; // 插件名称
    virtual QString getPluginType() = 0;
};

QT_BEGIN_NAMESPACE
#define QPluginInterface_iid "QPluginInterfaceIID"
Q_DECLARE_INTERFACE(QPluginInterface, QPluginInterface_iid)
QT_END_NAMESPACE


class SHAREDLIB_EXPORT QAbstractPlugin: public QObject,public QPluginInterface
{
    Q_OBJECT
    //Q_INTERFACES(QPluginInterface)
    //Q_PLUGIN_METADATA(IID QPluginInterface_iid FILE "qabstractplugin.json")
public:
    virtual void initPlugin(); // 初始化插件
    virtual QString getPluginName(); // 获取页面的名称
    virtual QString getPluginType();
};

#endif // QABSTRACTPLUGIN_H
