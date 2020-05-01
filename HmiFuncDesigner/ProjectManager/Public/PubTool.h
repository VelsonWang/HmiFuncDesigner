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
    static bool Draw3DFrame(QPainter *painter, QRect rect, QColor leftTopColor,
                            QColor rightBottomColor, QColor fillColor);

    static void FillColorRect(QPainter *painter, QRect rect, QColor color);

    // 删除数字字符串小数点后面的无效零字符
    static QString DeleteEndZeroOfDecimal(const QString szStr);
    // 删除数字字符串前面的无效零字符
    static QString DeleteFrontZeroOfDecimal(const QString szStr);

    inline static QRect InflateRect(QRect &rect, int offset);
    inline static QRect DeflateRect(QRect &rect, int offset);

public:
    static void draw3DRect(QPainter& pnt, QColor& col, int x, int y, int w, int h, bool up);
    static void draw3DRect(QPainter& pnt, const QColor& colLight, const QColor& colDark, int x, int y, int w, int h, bool up);
    static void drawRectangle(QPainter& pnt, const QRect& rc, const QColor& pen, const QColor& brush);
    static void drawTriangle(QPainter& pnt, const QPoint& pt0, const QPoint& pt1, const QPoint& pt2, const QColor& color);

    static void drawGradientFill(QPainter& pnt, const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz);

    static QRgb lightColor(QRgb clr, int nTint);
    static QRgb darkColor(QRgb clr, int nShade);

    static void RGBtoHSL(QRgb clr, double& h, double& s, double& l);

    static QRgb HSLtoRGB(double h, double s, double l);

    static QRgb blendColors(QRgb crA, QRgb crB, double fAmountA);

private:
    static double HueToRGB(double temp1, double temp2, double temp3);
    static int HueToRGB(int m1, int m2, int h);
    static QLinearGradient generateGradient(const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz);

};



inline QRect PubTool::InflateRect(QRect &rect, int offset)
{
    QRect tmpRect;
    if ( rect.left() < rect.right() )
    {
        tmpRect.setLeft(rect.left() - offset);
        tmpRect.setRight(rect.right() + offset);
    }
    else
    {
        tmpRect.setLeft(rect.left() + offset);
        tmpRect.setRight(rect.right()- offset);
    }

    if ( rect.top() < rect.bottom() )
    {
        tmpRect.setTop(rect.top() - offset);
        tmpRect.setBottom(rect.bottom() + offset);
    }
    else
    {
        tmpRect.setTop(rect.top() + offset);
        tmpRect.setBottom(rect.bottom() - offset);
    }
    return tmpRect;
}


////////////////////////////////////////////////////////////////////////////////


inline QRect PubTool::DeflateRect(QRect &rect, int offset)
{
    QRect tmpRect;
    if ( rect.left() < rect.right() )
    {
        tmpRect.setLeft(rect.left()  + offset);
        tmpRect.setRight(rect.right() - offset);
    }
    else
    {
        tmpRect.setLeft(rect.left()  - offset);
        tmpRect.setRight(rect.right() + offset);
    }

    if ( rect.top() < rect.bottom() )
    {
        tmpRect.setTop(rect.top() + offset);
        tmpRect.setBottom(rect.bottom() - offset);
    }
    else
    {
        tmpRect.setTop(rect.top() - offset);
        tmpRect.setBottom(rect.bottom() + offset);
    }
    return tmpRect;
}

#endif // PUBTOOL_H
