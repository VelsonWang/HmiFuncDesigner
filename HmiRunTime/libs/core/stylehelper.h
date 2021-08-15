#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include "corelibglobal.h"

#include <QColor>
#include <QPalette>
#include <QStyle>

/*
 *这个类用于界面的绘制，它提供了一些界面绘制的基本函数和基本参数
 *通过控制这个类的参数来控制界面的样式
 */

class CORELIB_EXPORT StyleHelper
{
public:
    static QIcon drawIndicatorIcon(const QPalette &palette,QStyle *style);
    static int navigationWidgetHeight();//获取默认控件高度
    static qreal sidebarFontSize();//字体大小
    static QPalette sidebarFontPalette(const QPalette &original);//配置画板

    static QColor baseColor(bool lightColored = false);//获取基色
    static QColor panelTextColor(bool lightColored = false);//获取文字颜色
        static QColor highlightColor(bool lightColored = false);//获取高亮颜色
    static QColor shadowColor(bool lightColored = false);//获取阴影颜色
    static QColor borderColor(bool lightColored = false);//获取窗体颜色
    static QColor buttonTextColor();//获取按钮文字颜色
    static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50);//获取中间色

    static QColor sidebarHighlight();//获取边框高亮颜色
    static QColor sidebarShadow();//获取边框阴影颜色
    static void setBaseColor(const QColor &color);//设置基色

    //绘制箭头
    static void drawArrow(QStyle::PrimitiveElement element, QPainter *painter, const QStyleOption *option);

    //绘制横向渐变色
    static void horizontalGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect, bool lightColored = false);
    //绘制纵向渐变色
    static void verticalGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect, bool lightColored = false);
    //绘制菜单渐变色
    static void menuGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect);
    //是否使用缓存机制
    static bool usePixmapCache();
    //绘制带有阴影的图标
    static void drawIconWithShadow(const QIcon &icon, const QRect &rect, QPainter *p, QIcon::Mode iconMode,
                                   int radius = 3, const QColor &color = QColor(0, 0, 0, 130),
                                   const QPoint &offset = QPoint(1, -2));
    //绘制居中的图片
    static void drawCornerImage(const QImage &img, QPainter *painter, QRect rect,
                         int left = 0, int top = 0, int right = 0, int bottom = 0);
    //给图片染色
    static void tintImage(QImage &img, const QColor &tintColor);
    //绘制背景渐变色
    static void backgroundGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect);

protected:
    //纵向渐变色实际绘制函数
    static void verticalGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect, bool lightColored);
    //横向渐变色实际绘制函数
    static void horizontalGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect, bool lightColored);
    //菜单渐变色实际绘制函数
    static void menuGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect);
private:
    static QColor m_baseColor;//基色，其它颜色都是从这里转换过去的
};
#endif // STYLEHELPER_H
