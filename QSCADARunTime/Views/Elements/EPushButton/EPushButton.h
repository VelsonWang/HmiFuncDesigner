#ifndef EPUSHBUTTONPLUGIN_H
#define EPUSHBUTTONPLUGIN_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EPushButton : public QObject, IDrawGraphPage
{
public:
    EPushButton();
    static IDrawGraphPage* creatObjFunc()
    {
        return new EPushButton;
    }

    // 初始化
    void initialize() Q_DECL_OVERRIDE;
    // 获取元素名称
    QString getElementName() Q_DECL_OVERRIDE;
    // 获取元素ID
    int getElementID() Q_DECL_OVERRIDE;
    // 获取元素ID String
    QString getElementIDString() Q_DECL_OVERRIDE;
    // 创建元素
    Element* createElement() Q_DECL_OVERRIDE;


};

#endif // EPUSHBUTTONPLUGIN_H
