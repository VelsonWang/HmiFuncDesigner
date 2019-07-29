#ifndef ELEMENTGROUP_H
#define ELEMENTGROUP_H

#include <QGraphicsItemGroup>
#include "PublicDefine.h"

class ElementGroup : public QGraphicsItemGroup
{
public:
    ElementGroup();

    enum {Type = GroupItemType};

    int type() {
        return Type;
    }

    void setClickPosition(QPointF position);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // ELEMENTGROUP_H
