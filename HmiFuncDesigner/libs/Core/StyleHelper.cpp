#include "StyleHelper.h"
#include <QPixmapCache>
#include <QWidget>
#include <QRect>
#include <QPainter>
#include <QApplication>
#include <QPalette>
#include <QStyleOption>
#include <QObject>


#define DEFAULT_BASE_COLOR 0x808080 //默认基色

QColor StyleHelper::m_baseColor = DEFAULT_BASE_COLOR;




static int clamp(float x)//把x值控制在0到255之间
{
    const int val = x > 255 ? 255 : static_cast<int>(x);
    return val < 0 ? 0 : val;
}

QIcon StyleHelper::drawIndicatorIcon(const QPalette &palette,QStyle *style)
{
    QPixmap pix(14,14);

    pix.fill(Qt::transparent);

    QStyleOption branchOption;
    QRect r(QPoint(0,0),pix.size());
    branchOption.rect=QRect(2,2,9,9);
    branchOption.palette=palette;
    branchOption.state=QStyle::State_Children;

    QPainter p;
    p.begin(&pix);
    style->drawPrimitive(QStyle::PE_IndicatorBranch,&branchOption,&p);
    p.end();
    QIcon rc=pix;
    rc.addPixmap(pix,QIcon::Selected,QIcon::Off);
    branchOption.state |=QStyle::State_Open;

    pix.fill(Qt::transparent);
    p.begin(&pix);
    style->drawPrimitive(QStyle::PE_IndicatorBranch,&branchOption,&p);
    p.end();

    rc.addPixmap(pix,QIcon::Normal,QIcon::On);
    rc.addPixmap(pix,QIcon::Selected,QIcon::On);
    return rc;
}

//去中间色
QColor StyleHelper::mergedColors(const QColor &colorA, const QColor &colorB, int factor)
{
    //factor越大，中间色越靠近colorA
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
    tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
    tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
    return tmp;
}

qreal StyleHelper::sidebarFontSize()
{//字体大小
    return 7.5;
}

int StyleHelper::navigationWidgetHeight()
{//控件默认高度
    return 24;
}

QColor StyleHelper::buttonTextColor()
{//按钮颜色
    return QColor(0x4c4c4c);
}

QColor StyleHelper::sidebarHighlight()
{//边框高亮颜色
    return QColor(255, 255, 255, 40);
}

QColor StyleHelper::sidebarShadow()
{//边框阴影颜色
    return QColor(0, 0, 0, 40);
}

bool StyleHelper::usePixmapCache()
{//使用绘图缓存
    return true;
}

QPalette StyleHelper::sidebarFontPalette(const QPalette &original)
{//配置边框的画板
    QPalette palette = original;
    palette.setColor(QPalette::Active, QPalette::Text, panelTextColor());
    palette.setColor(QPalette::Active, QPalette::WindowText, panelTextColor());
    palette.setColor(QPalette::Inactive, QPalette::Text, panelTextColor().darker());
    palette.setColor(QPalette::Inactive, QPalette::WindowText, panelTextColor().darker());
    return palette;
}

QColor StyleHelper::panelTextColor(bool lightColored)
{//文字颜色
    if (!lightColored)
        return Qt::white;
    else
        return Qt::black;
}

QColor StyleHelper::baseColor(bool lightColored)
{//基色
    if (!lightColored)
        return m_baseColor;
    else
        return m_baseColor.lighter(230);
}

QColor StyleHelper::highlightColor(bool lightColored)
{//高亮颜色
    QColor result = baseColor(lightColored);
    if (!lightColored)
        result.setHsv(result.hue(),
                      clamp(result.saturation()),
                      clamp(result.value() * 1.16));
    else
        result.setHsv(result.hue(),
                      clamp(result.saturation()),
                      clamp(result.value() * 1.06));
    return result;
}

QColor StyleHelper::shadowColor(bool lightColored)
{//阴影颜色
    QColor result = baseColor(lightColored);
    result.setHsv(result.hue(),
                  clamp(result.saturation() * 1.1),
                  clamp(result.value() * 0.70));
    return result;
}

QColor StyleHelper::borderColor(bool lightColored)
{//窗体颜色
    QColor result = baseColor(lightColored);
    result.setHsv(result.hue(),
                  result.saturation(),
                  result.value() / 2);
    return result;
}

void StyleHelper::setBaseColor(const QColor &newcolor)
{//设置基色
    QColor color;
    color.setHsv(newcolor.hue(),
                 newcolor.saturation() * 0.7,
                 64 + newcolor.value() / 3);

    if (color.isValid() && color != m_baseColor) {
        m_baseColor = color;
        foreach (QWidget *w, QApplication::topLevelWidgets())//设置完了就刷新窗口
            w->update();
    }
}

void StyleHelper::verticalGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect, bool lightColored)
{//实际绘制横向渐变色
    QColor highlight = highlightColor(lightColored);//高亮颜色
    QColor shadow = shadowColor(lightColored);//阴影颜色
    QLinearGradient grad(spanRect.topRight(), spanRect.topLeft());//横向渐变色实例
    grad.setColorAt(0, highlight.lighter(117));//颜色1
    grad.setColorAt(1, shadow.darker(109));//颜色2
    p->fillRect(rect, grad);//填充渐变色

    QColor light(255, 255, 255, 80);
    p->setPen(light);
    p->drawLine(rect.topRight() - QPoint(1, 0), rect.bottomRight() - QPoint(1, 0));//画高亮线条
    QColor dark(0, 0, 0, 90);
    p->setPen(dark);
    p->drawLine(rect.topLeft(), rect.bottomLeft());//画阴影线条
}

void StyleHelper::verticalGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect, bool lightColored)
{//横向渐变色绘制
    if (usePixmapCache()) {
        QString key;
        QColor keyColor = baseColor(lightColored);
        key.sprintf("mh_vertical %d %d %d %d %d",
                    spanRect.width(), spanRect.height(), clipRect.width(),
                    clipRect.height(), keyColor.rgb());;

        QPixmap pixmap;
        if (!QPixmapCache::find(key, pixmap)) {
            pixmap = QPixmap(clipRect.size());
            QPainter p(&pixmap);
            QRect rect(0, 0, clipRect.width(), clipRect.height());
            verticalGradientHelper(&p, spanRect, rect, lightColored);
            p.end();
            QPixmapCache::insert(key, pixmap);//绘制的图像保存到缓存中去
        }

        painter->drawPixmap(clipRect.topLeft(), pixmap);//绘制图像
    } else {
        verticalGradientHelper(painter, spanRect, clipRect, lightColored);//直接绘制
    }
}

void StyleHelper::horizontalGradientHelper(QPainter *p, const QRect &spanRect, const
                                           QRect &rect, bool lightColored)
{//纵向渐变色绘制，原理基本横向绘制
    if (lightColored) {
        QLinearGradient shadowGradient(rect.topLeft(), rect.bottomLeft());
        shadowGradient.setColorAt(0, 0xf0f0f0);
        shadowGradient.setColorAt(1, 0xcfcfcf);
        p->fillRect(rect, shadowGradient);
        return;
    }

    QColor base = baseColor(lightColored);
    QColor highlight = highlightColor(lightColored);
    QColor shadow = shadowColor(lightColored);
    QLinearGradient grad(rect.topLeft(), rect.bottomLeft());
    grad.setColorAt(0, highlight.lighter(100));
    if (rect.height() == navigationWidgetHeight()) {
        grad.setColorAt(0.4, highlight);
        grad.setColorAt(0.401, base);
    }
    grad.setColorAt(1, shadow);
    p->fillRect(rect, grad);

    QLinearGradient shadowGradient(spanRect.topLeft(), spanRect.topRight());
    shadowGradient.setColorAt(0, QColor(0, 0, 0, 30));
    QColor lighterHighlight;
    lighterHighlight = highlight.lighter(100);
    lighterHighlight.setAlpha(80);
    shadowGradient.setColorAt(0.7, lighterHighlight);
    shadowGradient.setColorAt(1, QColor(0, 0, 0, 40));
    p->fillRect(rect, shadowGradient);
}

void StyleHelper::horizontalGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect, bool lightColored)
{//纵向绘制渐变色，原理基本同横向绘制
    if (StyleHelper::usePixmapCache()) {
        QString key;
        QColor keyColor = baseColor(lightColored);
        key.sprintf("mh_horizontal %d %d %d %d %d %d",
                    spanRect.width(), spanRect.height(), clipRect.width(),
                    clipRect.height(), keyColor.rgb(), spanRect.x());

        QPixmap pixmap;
        if (!QPixmapCache::find(key, pixmap)) {
            pixmap = QPixmap(clipRect.size());
            QPainter p(&pixmap);
            QRect rect = QRect(0, 0, clipRect.width(), clipRect.height());
            horizontalGradientHelper(&p, spanRect, rect, lightColored);
            p.end();
            QPixmapCache::insert(key, pixmap);
        }

        painter->drawPixmap(clipRect.topLeft(), pixmap);

    } else {
        horizontalGradientHelper(painter, spanRect, clipRect, lightColored);
    }
}

void StyleHelper::menuGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect)
{//绘制菜单渐变色
    QLinearGradient grad(spanRect.topLeft(), spanRect.bottomLeft());
    QColor menuColor = mergedColors(StyleHelper::baseColor(), QColor(244, 244, 244), 25);
    grad.setColorAt(0, menuColor.lighter(112));
    grad.setColorAt(1, menuColor);
    p->fillRect(rect, grad);
}

void StyleHelper::backgroundGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect)
{//绘制背景渐变色
    QLinearGradient grad(spanRect.topLeft(), spanRect.bottomLeft());
    QColor menuColor = mergedColors(StyleHelper::baseColor(), QColor(220, 220, 220), 20);
    grad.setColorAt(0, menuColor.lighter(150));
    grad.setColorAt(0.7, menuColor);
    grad.setColorAt(1,menuColor.lighter(120));
    painter->fillRect(clipRect, grad);
}

void StyleHelper::drawArrow(QStyle::PrimitiveElement element, QPainter *painter, const QStyleOption *option)
{//绘制箭头
    // From windowsstyle but modified to enable AA
    if (option->rect.width() <= 1 || option->rect.height() <= 1)
        return;

    QRect r = option->rect;
    int size = qMin(r.height(), r.width());
    QPixmap pixmap;
    QString pixmapName;
    pixmapName.sprintf("arrow-%s-%d-%d-%d-%ld",
                       "$qt_ia",
                       uint(option->state), element,
                       size, (long int)option->palette.cacheKey());
    if (!QPixmapCache::find(pixmapName, pixmap)) {
        int border = size/5;
        int sqsize = 2*(size/2);
        QImage image(sqsize, sqsize, QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPainter imagePainter(&image);
        imagePainter.setRenderHint(QPainter::Antialiasing, true);
        imagePainter.translate(0.5, 0.5);
        QPolygon a;
        switch (element) {
        case QStyle::PE_IndicatorArrowUp:
            a.setPoints(3, border, sqsize/2,  sqsize/2, border,  sqsize - border, sqsize/2);
            break;
        case QStyle::PE_IndicatorArrowDown:
            a.setPoints(3, border, sqsize/2,  sqsize/2, sqsize - border,  sqsize - border, sqsize/2);
            break;
        case QStyle::PE_IndicatorArrowRight:
            a.setPoints(3, sqsize - border, sqsize/2,  sqsize/2, border,  sqsize/2, sqsize - border);
            break;
        case QStyle::PE_IndicatorArrowLeft:
            a.setPoints(3, border, sqsize/2,  sqsize/2, border,  sqsize/2, sqsize - border);
            break;
        default:
            break;
        }

        int bsx = 0;
        int bsy = 0;

        if (option->state & QStyle::State_Sunken) {
            bsx = qApp->style()->pixelMetric(QStyle::PM_ButtonShiftHorizontal);
            bsy = qApp->style()->pixelMetric(QStyle::PM_ButtonShiftVertical);
        }

        QRect bounds = a.boundingRect();
        int sx = sqsize / 2 - bounds.center().x() - 1;
        int sy = sqsize / 2 - bounds.center().y() - 1;
        imagePainter.translate(sx + bsx, sy + bsy);

        if (!(option->state & QStyle::State_Enabled)) {
            QColor foreGround(150, 150, 150, 150);
            imagePainter.setBrush(option->palette.mid().color());
            imagePainter.setPen(option->palette.mid().color());
        } else {
            QColor shadow(0, 0, 0, 100);
            imagePainter.translate(0, 1);
            imagePainter.setPen(shadow);
            imagePainter.setBrush(shadow);
            QColor foreGround(255, 255, 255, 210);
            imagePainter.drawPolygon(a);
            imagePainter.translate(0, -1);
            imagePainter.setPen(foreGround);
            imagePainter.setBrush(foreGround);
        }
        imagePainter.drawPolygon(a);
        imagePainter.end();
        pixmap = QPixmap::fromImage(image);
        QPixmapCache::insert(pixmapName, pixmap);
    }
    int xOffset = r.x() + (r.width() - size)/2;
    int yOffset = r.y() + (r.height() - size)/2;
    painter->drawPixmap(xOffset, yOffset, pixmap);
}

void StyleHelper::menuGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect)
{//菜单渐变色
    if (usePixmapCache()) {
        QString key;
        key.sprintf("mh_menu %d %d %d %d %d",
                    spanRect.width(), spanRect.height(), clipRect.width(),
                    clipRect.height(), StyleHelper::baseColor().rgb());

        QPixmap pixmap;
        if (!QPixmapCache::find(key, pixmap)) {
            pixmap = QPixmap(clipRect.size());
            QPainter p(&pixmap);
            QRect rect = QRect(0, 0, clipRect.width(), clipRect.height());
            menuGradientHelper(&p, spanRect, rect);
            p.end();
            QPixmapCache::insert(key, pixmap);
        }

        painter->drawPixmap(clipRect.topLeft(), pixmap);
    } else {
        menuGradientHelper(painter, spanRect, clipRect);
    }
}

// Draws a cached pixmap with shadow
void StyleHelper::drawIconWithShadow(const QIcon &icon, const QRect &rect,
                                     QPainter *p, QIcon::Mode iconMode, int radius, const QColor &color, const QPoint &offset)
{//绘制图标
    QPixmap cache;
    QString pixmapName = QString::fromLatin1("icon %0 %1 %2").arg(icon.cacheKey()).arg(iconMode).arg(rect.height());

    if (!QPixmapCache::find(pixmapName, cache)) {
        QPixmap px = icon.pixmap(rect.size());
        cache = QPixmap(px.size() + QSize(radius * 2, radius * 2));
        cache.fill(Qt::transparent);

        QPainter cachePainter(&cache);
        if (iconMode == QIcon::Disabled) {
            QImage im = px.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y=0; y<im.height(); ++y) {
                QRgb *scanLine = (QRgb*)im.scanLine(y);
                for (int x=0; x<im.width(); ++x) {
                    QRgb pixel = *scanLine;
                    char intensity = qGray(pixel);
                    *scanLine = qRgba(intensity, intensity, intensity, qAlpha(pixel));
                    ++scanLine;
                }
            }
            px = QPixmap::fromImage(im);
        }

        // Draw shadow
        QImage tmp(px.size() + QSize(radius * 2, radius * 2 + 1), QImage::Format_ARGB32_Premultiplied);
        tmp.fill(Qt::transparent);

        QPainter tmpPainter(&tmp);
        tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
        tmpPainter.drawPixmap(QPoint(radius, radius), px);
        tmpPainter.end();

        // blur the alpha channel
        QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
        blurred.fill(Qt::transparent);
        //QPainter blurPainter(&blurred);
        //qt_blurImage(&blurPainter, tmp, radius, false, true);
        //blurPainter.end();

        tmp = blurred;

        // blacken the image...
        tmpPainter.begin(&tmp);
        tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tmpPainter.fillRect(tmp.rect(), color);
        tmpPainter.end();

        tmpPainter.begin(&tmp);
        tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tmpPainter.fillRect(tmp.rect(), color);
        tmpPainter.end();

        // draw the blurred drop shadow...
        cachePainter.drawImage(QRect(0, 0, cache.rect().width(), cache.rect().height()), tmp);

        // Draw the actual pixmap...
        cachePainter.drawPixmap(QPoint(radius, radius) + offset, px);
        QPixmapCache::insert(pixmapName, cache);
    }

    QRect targetRect = cache.rect();
    targetRect.moveCenter(rect.center());
    p->drawPixmap(targetRect.topLeft() - offset, cache);
}

// Draws a CSS-like border image where the defined borders are not stretched
void StyleHelper::drawCornerImage(const QImage &img, QPainter *painter, QRect rect,
                                  int left, int top, int right, int bottom)
{//绘制图片
    QSize size = img.size();
    if (top > 0) { //top
        painter->drawImage(QRect(rect.left() + left, rect.top(), rect.width() -right - left, top), img,
                           QRect(left, 0, size.width() -right - left, top));
        if (left > 0) //top-left
            painter->drawImage(QRect(rect.left(), rect.top(), left, top), img,
                               QRect(0, 0, left, top));
        if (right > 0) //top-right
            painter->drawImage(QRect(rect.left() + rect.width() - right, rect.top(), right, top), img,
                               QRect(size.width() - right, 0, right, top));
    }
    //left
    if (left > 0)
        painter->drawImage(QRect(rect.left(), rect.top()+top, left, rect.height() - top - bottom), img,
                           QRect(0, top, left, size.height() - bottom - top));
    //center
    painter->drawImage(QRect(rect.left() + left, rect.top()+top, rect.width() -right - left,
                             rect.height() - bottom - top), img,
                       QRect(left, top, size.width() -right -left,
                             size.height() - bottom - top));
    if (right > 0) //right
        painter->drawImage(QRect(rect.left() +rect.width() - right, rect.top()+top, right, rect.height() - top - bottom), img,
                           QRect(size.width() - right, top, right, size.height() - bottom - top));
    if (bottom > 0) { //bottom
        painter->drawImage(QRect(rect.left() +left, rect.top() + rect.height() - bottom,
                                 rect.width() - right - left, bottom), img,
                           QRect(left, size.height() - bottom,
                                 size.width() - right - left, bottom));
        if (left > 0) //bottom-left
            painter->drawImage(QRect(rect.left(), rect.top() + rect.height() - bottom, left, bottom), img,
                               QRect(0, size.height() - bottom, left, bottom));
        if (right > 0) //bottom-right
            painter->drawImage(QRect(rect.left() + rect.width() - right, rect.top() + rect.height() - bottom, right, bottom), img,
                               QRect(size.width() - right, size.height() - bottom, right, bottom));
    }
}

// Tints an image with tintColor, while preserving alpha and lightness
void StyleHelper::tintImage(QImage &img, const QColor &tintColor)
{//染色
    QPainter p(&img);
    p.setCompositionMode(QPainter::CompositionMode_Screen);

    for (int x = 0; x < img.width(); ++x) {
        for (int y = 0; y < img.height(); ++y) {
            QRgb rgbColor = img.pixel(x, y);
            int alpha = qAlpha(rgbColor);
            QColor c = QColor(rgbColor);

            if (alpha > 0) {
                c.toHsl();
                qreal l = c.lightnessF();
                QColor newColor = QColor::fromHslF(tintColor.hslHueF(), tintColor.hslSaturationF(), l);
                newColor.setAlpha(alpha);
                img.setPixel(x, y, newColor.rgba());
            }
        }
    }
}
