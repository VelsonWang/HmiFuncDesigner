#ifndef ESWITCHBUTTONPLUGIN_H
#define ESWITCHBUTTONPLUGIN_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class ESwitchButton : public QObject, IDrawGraphPage
{
public:
    ESwitchButton();
    static IDrawGraphPage* creatObjFunc()
    {
        return new ESwitchButton;
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
    Element* createElement(QWidget *owner=nullptr) Q_DECL_OVERRIDE;


};

#endif // ESWITCHBUTTONPLUGIN_H
