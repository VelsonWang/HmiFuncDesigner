#ifndef EELLIPSEPLUGIN_H
#define EELLIPSEPLUGIN_H

#include <QObject>
#include "./../../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h"


class EllipsePlugin : public QObject, IHmiRunTimeViewPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID HmiRunTimeViewPluginInterface_iid FILE "EEllipse.json")
    Q_INTERFACES(IHmiRunTimeViewPlugin)

public:
    EllipsePlugin();

    /////////////////////<plugin operation>////////////////////////////

    // 初始化
    virtual void initialize() Q_DECL_OVERRIDE;
    // 获取元素名称
    virtual QString getElementName() Q_DECL_OVERRIDE;
    // 获取元素ID
    virtual int getElementID() Q_DECL_OVERRIDE;
    // 获取元素ID String
    virtual QString getElementIDString() Q_DECL_OVERRIDE;
    // 创建元素
    virtual Element* createElement(QWidget *owner=0) Q_DECL_OVERRIDE;

    /////////////////////<plugin operation>////////////////////////////



};

#endif // EELLIPSEPLUGIN_H
