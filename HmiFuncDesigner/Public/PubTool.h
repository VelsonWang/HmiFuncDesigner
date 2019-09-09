#ifndef PUBTOOL_H
#define PUBTOOL_H

#include <QRect>
#include <QString>
#include <QStringList>

class PubTool
{
public:
    PubTool();

public:
    //拉升四边形
    static void SpreadRect(QRect *rc, int cx, int cy);
    //移动四边形
    static void FloatRect(QRect *rc, int cx, int cy);
    //设置四边型的值
    static void SetRect(QRect *rc, int left, int top, int right, int bottom);

};

#endif // PUBTOOL_H
