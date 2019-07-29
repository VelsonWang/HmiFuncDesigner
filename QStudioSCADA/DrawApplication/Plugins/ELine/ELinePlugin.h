#ifndef ELINEPLUGIN_H
#define ELINEPLUGIN_H

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
    Element* createElement(const QString &szProjPath, const QString &szProjName) Q_DECL_OVERRIDE;

    /////////////////////<plugin operation>////////////////////////////



};

#endif // ELINEPLUGIN_H
