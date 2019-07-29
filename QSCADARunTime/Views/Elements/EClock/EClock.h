#ifndef ECLOCK_H
#define ECLOCK_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EClock : public QObject, IDrawGraphPage
{
public:
    EClock();
    static IDrawGraphPage* creatObjFunc()
    {
        return new EClock;
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

#endif // ECLOCK_H
