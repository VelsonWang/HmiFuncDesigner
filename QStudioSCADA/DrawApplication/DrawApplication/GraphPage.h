#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H

#include <QList>
#include <QJsonObject>
#include "IElement.h"


/*
* 描述一个画面
*/
class GraphPage
{
public:
    GraphPage();
    // 添加一个图形元素
    void AddGraphElement(StdElement *e);
    // 删除一个图形元素
    void DelGraphElement(StdElement *e);
    // 加载元素
    void load(const QJsonObject &json);
    // 保存元素
    void save(QJsonObject &json);

public:
    QList<StdElement *> m_ElementList; // 图形元素表

};

#endif // GRAPHPAGE_H
