#ifndef PUBTOOL_H
#define PUBTOOL_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QColor>

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

    //////////////////////////////////////////////////////////////
    //绘图工具函数
    static void Draw3DLowerRect(QPainter *painter, QRect rct);
    static void Draw3DRaiseRect(QPainter *painter, QRect rct);
    static void DrawFrameRect(QPainter *painter, int left, int top, int right, int bottom, QColor cl);
    static void DrawFrameRect(QPainter *painter, QRect rct, QColor cl);
    static void FillFullRect(QPainter *painter, QRect rct, QColor cl);
    static void DrawLineInRect(QPainter *painter, int sx, int sy, int dx, int dy, QColor cl, QRect rct);
    static void DrawFrameRectInRect(QPainter *painter, QRect rct, QColor cl, QRect pRct);
    static void DrawLine(QPainter *painter, QPoint ptStart, QPoint ptEnd, QColor cl);
    static void DrawGrid(QPainter *painter,int w_grid, int h_grid, QRect rcSize);
    static QPoint GetCrossOfOuterPt(int x, int y, QRect aRct);
    static bool IsInRect(QRect rct, QPoint pt, bool bBorder = true);
    static bool IsInRect(QRect rct, int X, int Y, bool bBorder = true);
    static bool IsInRect(QRect rct, QRect subRct, bool bBorder = true);
    static bool IsCrossRect(QRect aRct, QRect bRct, bool bBorder = true);

};

#endif // PUBTOOL_H
