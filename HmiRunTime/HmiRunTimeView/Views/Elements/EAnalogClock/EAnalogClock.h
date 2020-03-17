#ifndef EANALOGCLOCK_H
#define EANALOGCLOCK_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EAnalogClock : public QObject, IDrawGraphPage
{
public:
    EAnalogClock();
    static IDrawGraphPage* creatObjFunc()
    {
        return new EAnalogClock;
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

#endif // EANALOGCLOCK_H
