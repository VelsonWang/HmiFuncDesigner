#ifndef ERECT_H
#define ERECT_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class ERect : public QObject, IDrawGraphPage
{
public:
    ERect();
    static IDrawGraphPage* creatObjFunc()
    {
        return new ERect;
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

#endif // ERECT_H
