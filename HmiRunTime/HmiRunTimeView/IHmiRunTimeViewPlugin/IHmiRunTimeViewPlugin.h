#ifndef IHMIRUNTIMEVIEWPLUGIN_H
#define IHMIRUNTIMEVIEWPLUGIN_H


#include <QStringList>
#include <QIcon>
#include "../Public/Element.h"
#include "../Public/PublicDefine.h"


/*
* 注意本类不要派生自QObject
*/
class IHmiRunTimeViewPlugin
{
public:
    virtual ~IHmiRunTimeViewPlugin() {}

    // 初始化
    virtual void initialize() = 0;
    // 获取元素名称
    virtual QString getElementName() = 0;
    // 获取元素ID
    virtual int getElementID() = 0;
    // 获取元素ID String
    virtual QString getElementIDString() = 0;
    // 创建元素
    virtual Element* createElement(QWidget *owner=0) = 0;
};


QT_BEGIN_NAMESPACE

#define HmiRunTimeViewPluginInterface_iid "HmiRunTime.HmiRunTimeView.PluginInterface"
Q_DECLARE_INTERFACE(IHmiRunTimeViewPlugin, HmiRunTimeViewPluginInterface_iid)

QT_END_NAMESPACE

#endif // IHMIRUNTIMEVIEWPLUGIN_H
