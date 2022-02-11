﻿#ifndef IDRAWAPPLICATIONPLUGIN_H
#define IDRAWAPPLICATIONPLUGIN_H


#include <QStringList>
#include <QIcon>
#include "../Public/Element.h"
#include "../Public/PublicDefine.h"
#include "qtpropertymanager.h"
#include "qsoftcore.h"

/*
* 注意本类不要派生自QObject
*/
class IDrawApplicationPlugin
{
public:
    virtual ~IDrawApplicationPlugin() {}

    // 插件初始化
    virtual void initialize() = 0;
    // 插件类型名称
    virtual QString getPluginTypeName() = 0;
    // 插件名称
    virtual QString getPluginName() = 0;
    // 获取元素的图标
    virtual QIcon getElementIcon() = 0;
    // 获取元素名称
    virtual QString getElementName() = 0;
    // 获取元素ID
    virtual int getElementID() = 0;
    // 获取元素ID String
    virtual QString getElementIDString() = 0;
    // 创建元素
    virtual Element* createElement(QSoftCore* pSoftCoreObj, QtVariantPropertyManager *propertyMgr) = 0;
};


QT_BEGIN_NAMESPACE

#define DrawApplicationPluginInterface_iid "HmiFuncDesigner.DrawApplication.PluginInterface"
Q_DECLARE_INTERFACE(IDrawApplicationPlugin, DrawApplicationPluginInterface_iid)

QT_END_NAMESPACE

#endif // IDRAWAPPLICATIONPLUGIN_H
