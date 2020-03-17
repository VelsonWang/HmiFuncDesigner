
#ifndef IDRAWGRAPHPAGE_H
#define IDRAWGRAPHPAGE_H

#include <QWidget>
#include <QStringList>
#include <QIcon>
#include "Public/Element.h"
#include "Public/PublicDefine.h"

/*
* 注意本类不要派生自QObject
*/
class IDrawGraphPage
{
public:
    virtual ~IDrawGraphPage() {}

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

typedef IDrawGraphPage* (*CreateObjFunc)();

#endif // IDRAWGRAPHPAGE_H
