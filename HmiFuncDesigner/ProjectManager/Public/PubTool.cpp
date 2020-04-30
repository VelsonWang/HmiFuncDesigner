#include "PubTool.h"

PubTool::PubTool()
{

}

/*
* 拉升四边形
*/
void PubTool::SpreadRect(QRect *rc, int cx, int cy)
{
    rc->setLeft(rc->left() - cx);
    rc->setRight(rc->right() + cx);
    rc->setTop(rc->top() - cy);
    rc->setBottom(rc->bottom() + cy);
}

/*
* 移动四边形
*/
void PubTool::FloatRect(QRect *rc, int cx, int cy)
{
    rc->setLeft(rc->left() + cx);
    rc->setRight(rc->right() + cx);
    rc->setTop(rc->top() + cy);
    rc->setBottom(rc->bottom() + cy);
}

/*
* 设置四边型的值
*/
void PubTool::SetRect(QRect *rc, int left, int top, int right, int bottom)
{
    rc->setLeft(left);
    rc->setRight(right);
    rc->setTop(top);
    rc->setBottom(bottom);
}



void PubTool::Draw3DLowerRect(QPainter *painter, QRect rct)
{
    painter->save();
    QPen pen;
    pen.setColor(Qt::lightGray);
    painter->setPen(pen);

    painter->drawLine(rct.left(), rct.bottom(), rct.left(), rct.top());
    painter->drawLine(rct.left(), rct.top(), rct.right(), rct.top());

    pen.setColor(QColor(255, 255, 255));
    painter->setPen(pen);

    painter->drawLine(rct.right(), rct.top(), rct.right(), rct.bottom());
    painter->drawLine(rct.right(), rct.bottom(), rct.left(), rct.bottom());
    painter->restore();
}

void PubTool::Draw3DRaiseRect(QPainter *painter, QRect rct)
{
    painter->save();
    QPen pen;
    pen.setColor(QColor(255, 255, 255));
    painter->setPen(pen);

    painter->drawLine(rct.left(), rct.bottom(), rct.left(), rct.top());
    painter->drawLine(rct.left(), rct.top(), rct.right(), rct.top());

    pen.setColor(Qt::lightGray);
    painter->setPen(pen);

    painter->drawLine(rct.right(), rct.top(), rct.right(), rct.bottom());
    painter->drawLine(rct.right(), rct.bottom(), rct.left(), rct.bottom());
    painter->restore();
}

void PubTool::DrawFrameRect(QPainter *painter, int left, int top, int right, int bottom, QColor cl)
{
    QRect rct(left, top, right, bottom);
    DrawFrameRect(painter, rct, cl);
}

void PubTool::DrawFrameRect(QPainter *painter, QRect rct, QColor cl)
{
    painter->save();
    QPen pen;
    pen.setColor(cl);
    painter->setPen(pen);

    painter->drawLine(rct.left(), rct.bottom(), rct.left(), rct.top());
    painter->drawLine(rct.left(), rct.top(), rct.right(), rct.top());
    painter->drawLine(rct.right(), rct.top(), rct.right(), rct.bottom());
    painter->drawLine(rct.right(), rct.bottom(), rct.left(), rct.bottom());
    painter->restore();
}

void PubTool::FillFullRect(QPainter *painter, QRect rct, QColor cl)
{
    painter->save();
    QBrush brush(cl);
    rct.setRight(rct.right() + 1);
    rct.setBottom(rct.bottom() + 1) ;
    painter->fillRect(rct, brush);
    painter->restore();
}


void PubTool::DrawLineInRect(QPainter *painter, int sx, int sy, int dx, int dy, QColor cl, QRect rct)
{
    //只能处理垂直线和水平线
    if((sx != dx) && (sy != dy))
        return;
    painter->save();
    QPen pen;
    pen.setColor(cl);
    painter->setPen(pen);

    QPoint tempPt;

    if(IsInRect(rct, sx, sy, true) && IsInRect(rct, dx, dy, true))
    {
        painter->drawLine(sx, sy, dx, dy);
    }
    else if(IsInRect(rct, sx, sy, true))
    {
        tempPt = GetCrossOfOuterPt(dx, dy, rct);
        painter->drawLine(tempPt.x(), tempPt.y(), sx, sy);
    }
    else if(IsInRect(rct, dx, dy, true))
    {
        tempPt = GetCrossOfOuterPt(sx, sy, rct);
        painter->drawLine(tempPt.x(), tempPt.y(), dx, dy);
    }
    else
    {
        bool bCrossed = false;

        if((sx >= rct.left()) && (sx <= rct.right()))
        {
            if(((sy < rct.top()) && (dy > rct.bottom())) || ((dy < rct.top()) && (sy > rct.bottom())))
                bCrossed = true;
        }

        if((sy >= rct.top()) && (sy <= rct.bottom()))
        {
            if(((sx < rct.left()) && (dx > rct.right())) || ((dx < rct.left()) && (sx > rct.right())))
                bCrossed = true;
        }

        if(bCrossed)
        {
            tempPt = GetCrossOfOuterPt(sx, sy, rct);
            QPoint dPt = GetCrossOfOuterPt(dx, dy, rct);

            painter->drawLine(tempPt.x(), tempPt.y(), dPt.x(), dPt.y());
        }
    }
    painter->restore();
}


void PubTool::DrawFrameRectInRect(QPainter *painter, QRect rct, QColor cl, QRect pRct)
{
    painter->save();
    DrawLineInRect(painter, rct.left(), rct.bottom(), rct.left(), rct.top(), cl, pRct);
    DrawLineInRect(painter, rct.left(), rct.top(), rct.right(), rct.top(), cl, pRct);
    DrawLineInRect(painter, rct.right(), rct.top(), rct.right(), rct.bottom(), cl, pRct);
    DrawLineInRect(painter, rct.right(), rct.bottom(), rct.left(), rct.bottom(), cl, pRct);
    painter->restore();
}

void PubTool::DrawLine(QPainter *painter, QPoint ptStart, QPoint ptEnd, QColor cl)
{
    painter->save();
    QPen pen;
    pen.setColor(cl);
    painter->setPen(pen);
    painter->drawLine(ptStart.x(), ptStart.y(), ptEnd.x(), ptEnd.y());
    painter->restore();
}

void PubTool::DrawGrid(QPainter *painter,int w_grid,int h_grid, QRect rcSize)
{
    painter->save();
    int iWGridCount,iHGridCount;
    painter->setPen(QColor(64, 64, 64));

    iWGridCount = (rcSize.right() - rcSize.left())/ w_grid + 1;
    iHGridCount = (rcSize.bottom() - rcSize.top())/ h_grid + 1;

    for(int i=1; i<iHGridCount; i++)
    {
        for(int j=1; j<iWGridCount; j++)
        {
            painter->drawLine(j*w_grid, i*h_grid, j*w_grid+1, i*h_grid);
        }
    }
    painter->restore();
}


///////////////////////////////////////////////
//获取四边型外边结点和其边内侧的交点
QPoint PubTool::GetCrossOfOuterPt(int x, int y, QRect aRct)
{
    QPoint pt;

    if((x >= aRct.left()) && (x <= aRct.right()) && ((y <= aRct.top()) || (y >= aRct.bottom())))
    {
        pt.setX(x);

        if(y <= aRct.top())
            pt.setY(aRct.top());
        else
            pt.setY(aRct.bottom());
    }

    if((y >= aRct.top()) && (y <= aRct.bottom()) && ((x <= aRct.left()) || (x >= aRct.right())))
    {
        pt.setY(y);

        if(x <= aRct.left())
            pt.setX(aRct.left());
        else
            pt.setX(aRct.right());
    }

    return pt;
}


bool PubTool::IsInRect(QRect rct, QPoint pt, bool bBorder)
{
    return  IsInRect(rct, pt.x(), pt.y(), bBorder);
}

bool PubTool::IsInRect(QRect rct, int X, int Y, bool bBorder)
{
    bool bRet = false;

    if(bBorder)
        bRet = ((X >= rct.left()) && (X <= rct.right())
            && (Y >= rct.top()) && (Y <= rct.bottom()));
    else
        bRet = ((X > rct.left()) && (X < rct.right())
            && (Y > rct.top()) && (Y < rct.bottom()));

    return bRet;
}

bool PubTool::IsInRect(QRect rct, QRect subRct, bool bBorder)
{
    bool bRet = false;

    if(bBorder)
        bRet = ((subRct.left() >= rct.left()) && (subRct.right() <= rct.right())
            && (subRct.top() >= rct.top()) && (subRct.bottom() <= rct.bottom()));
    else
        bRet = ((subRct.left() > rct.left()) && (subRct.right() < rct.right())
            && (subRct.top() > rct.top()) && (subRct.bottom() < rct.bottom()));

    return bRet;
}

bool PubTool::IsCrossRect(QRect aRct, QRect bRct, bool bBorder)
{
    //////////////////////////////////////////////////
    //   *****************
    //   *               *
    //   *          *****************
    //   *          *    *          *
    //   *****************          *
    //			    *               *
    //              *****************
    //////////////////////////////////////////////////
    if(IsInRect(bRct, aRct.left(), aRct.top(), bBorder))
        return true;

    if(IsInRect(bRct, aRct.right(), aRct.top(), bBorder))
        return true;

    if(IsInRect(bRct, aRct.right(), aRct.bottom(), bBorder))
        return true;

    if(IsInRect(bRct, aRct.left(), aRct.bottom(), bBorder))
        return true;

    //////////////////////////////////////////////////
    if(IsInRect(aRct, bRct.left(), bRct.top(), bBorder))
        return true;

    if(IsInRect(aRct, bRct.right(), bRct.top(), bBorder))
        return true;

    if(IsInRect(aRct, bRct.right(), bRct.bottom(), bBorder))
        return true;

    if(IsInRect(aRct, bRct.left(), bRct.bottom(), bBorder))
        return true;
    //////////////////////////////////////////////////


    //////////////////////////////////////////////////
    //			*************
    //			*			*
    //	*****************************
    //	*		*			*		*
    //	*		*			*		*
    //	*****************************
    //			*			*
    //			*************
    //////////////////////////////////////////////////
    if(!bBorder)
    {
        if((aRct.left() > bRct.left()) && (aRct.right() < bRct.right())
                && (aRct.top() < bRct.top()) && (aRct.bottom() > bRct.bottom()))
            return true;

        if((aRct.left() < bRct.left()) && (aRct.right() > bRct.right())
                && (aRct.top() > bRct.top()) && (aRct.bottom() < bRct.bottom()))
            return true;
    }
    else
    {
        if((aRct.left() >= bRct.left()) && (aRct.right() <= bRct.right())
                && (aRct.top() <= bRct.top()) && (aRct.bottom() >= bRct.bottom()))
            return true;

        if((aRct.left() <= bRct.left()) && (aRct.right() >= bRct.right())
                && (aRct.top() >= bRct.top()) && (aRct.bottom() <= bRct.bottom()))
            return true;
    }
    //////////////////////////////////////////////////

    return false;
}


bool PubTool::Draw3DFrame(QPainter *painter, QRect rect, QColor leftTopColor,
                          QColor rightBottomColor, QColor fillColor)
{
    painter->save();

    QBrush brush(fillColor);
    painter->fillRect(rect, brush);

    QRect tmpRect;
    const int iWidthFrame =1;

    // draw left top corner
    QPen pen = QPen(QBrush(leftTopColor), iWidthFrame, Qt::SolidLine);
    painter->setPen(pen);
    tmpRect = PubTool::DeflateRect(rect, iWidthFrame/2);

    painter->drawLine(tmpRect.left(), tmpRect.bottom(), tmpRect.left(), tmpRect.top());
    painter->drawLine(tmpRect.left(), tmpRect.top(), tmpRect.right(), tmpRect.top());

    // draw right bottom;
    pen = QPen(QBrush(rightBottomColor), iWidthFrame, Qt::SolidLine);
    painter->drawLine(tmpRect.left(), tmpRect.bottom(), tmpRect.right(), tmpRect.bottom());
    painter->drawLine(tmpRect.right(), tmpRect.bottom(), tmpRect.right(), tmpRect.top());

    painter->restore();

    return true;
}



void PubTool::FillColorRect(QPainter *painter, QRect rect, QColor color)
{
    rect.setRight(rect.right() + 1);
    rect.setBottom(rect.bottom() + 1);
    painter->fillRect(rect, QBrush(color));
}


/**
 * @brief PubTool::DeleteEndZeroOfDecimal
 * @details 删除数字字符串小数点后面的无效零字符
 * @param szStr 数字字符串
 * @return 删除小数点后面的无效零后数字字符串
 */
QString PubTool::DeleteEndZeroOfDecimal(const QString szStr)
{
    QString szTmp = szStr;
    bool bDotFound = (szTmp.indexOf(".") >= 0);

    if(!bDotFound) {
        return szTmp;
    }

    if(szTmp.endsWith("0")) {
        while(szTmp.endsWith("0")) {
            szTmp = szTmp.remove(szTmp.length() - 1, 1);
        }
    }

    if(szTmp.endsWith(".")) {
        szTmp = szTmp.remove(szTmp.length() - 1, 1);
    }

    return szTmp;
}


/**
 * @brief PubTool::DeleteFrontZeroOfDecimal
 * @details 删除数字字符串前面的无效零字符
 * @param szStr 数字字符串
 * @return 删除小数点前面的无效零后数字字符串
 */
QString PubTool::DeleteFrontZeroOfDecimal(const QString szStr)
{
    QString szTmp = szStr;
    if(szTmp.startsWith("0")) {
        while(szTmp.startsWith("0")) {
            szTmp = szTmp.remove(0, 1);
        }
    }
    return szTmp;
}


void PubTool::draw3DRect(QPainter& pnt, QColor& col, int x, int y, int w, int h, bool up)
{
    QColor light = col.lighter(135);
    QColor dark = col.darker(140);
    pnt.setPen(up ? light : dark);
    pnt.drawLine(x, y, x+w, y);
    pnt.drawLine(x, y, x, y+h);
    pnt.setPen(up ? dark : light);
    pnt.drawLine(x, y+h, x+w, y+h);
    pnt.drawLine(x+w, y, x+w, y+h);
    pnt.setPen(col);
    pnt.drawPoint(x+w, y);
    pnt.drawPoint(x, y+h);
}

void PubTool::draw3DRect(QPainter& pnt, const QColor& colLight, const QColor& colDark, int x, int y, int w, int h, bool up)
{
    QColor light = colLight;
    QColor dark = colDark;
    pnt.setPen(up ? light : dark);
    pnt.drawLine(x, y, x + w, y);
    pnt.drawLine(x, y, x, y + h);

    pnt.setPen(up ? dark : light);
    pnt.drawLine(x, y + h, x + w, y + h);
    pnt.drawLine(x + w, y, x + w, y + h);

    pnt.setPen(colLight);
    pnt.drawPoint(x+w, y);
    pnt.drawPoint(x, y+h);
}

void PubTool::drawRectangle(QPainter& pnt, const QRect& rect, const QColor& pen, const QColor& brush)
{
    if (brush.isValid())
        pnt.fillRect(rect, QBrush(brush));

    if (pen != brush && pen.isValid())
        draw3DRect(pnt, pen, pen, rect.x(), rect.y(), rect.width(), rect.height(), true);
}

void PubTool::drawTriangle(QPainter& pnt, const QPoint& pt0, const QPoint& pt1, const QPoint& pt2, const QColor& color)
{
//    pnt.setRenderHint(QPainter::Antialiasing);
    QPen oldPen = pnt.pen();
    QBrush oldBrush = pnt.brush();

    pnt.setPen(color);
    pnt.setBrush(QBrush(color));

    QPoint pts[] = {pt0, pt1, pt2};
    pnt.drawPolygon(pts, 3);

    pnt.setBrush(oldBrush);
    pnt.setPen(oldPen);
}

void PubTool::drawGradientFill(QPainter& pnt, const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz)
{
    Q_UNUSED(bHorz);
    pnt.save();
    pnt.setBrush(generateGradient(rect, crFrom, crTo, bHorz));
    pnt.setPen(Qt::transparent);
    pnt.drawRect(rect);
    pnt.restore();
}

QRgb PubTool::lightColor(QRgb clr, int nTint)
{
    double h, s, l;

    RGBtoHSL(clr, h, s, l);
    clr = HSLtoRGB(h, s, l + (1.0 - l) * (double)nTint / 100.0);

    return clr;
}

QRgb PubTool::darkColor(QRgb clr, int nShade)
{
    double h, s, l;

    RGBtoHSL(clr, h, s, l);
    clr = HSLtoRGB(h, s, l * (1 + (double)nShade / 100.0));

    return clr;
}

void PubTool::RGBtoHSL(QRgb clr, double& h, double& s, double& l)
{
    double r = (double)qRed(clr)/255.0;
    double g = (double)qGreen(clr)/255.0;
    double b = (double)qBlue(clr)/255.0;

    double maxcolor = qMax(r, qMax(g, b));
    double mincolor = qMin(r, qMin(g, b));

    l = (maxcolor + mincolor)/2;

    if (maxcolor == mincolor) {
        h = 0;
        s = 0;
    } else {
        if (l < 0.5)
            s = (maxcolor-mincolor)/(maxcolor + mincolor);
        else
            s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

        if (r == maxcolor)
            h = (g-b)/(maxcolor-mincolor);
        else if (g == maxcolor)
            h = 2.0+(b-r)/(maxcolor-mincolor);
        else
            h = 4.0+(r-g)/(maxcolor-mincolor);

        h /= 6.0;
        if (h < 0.0)
            h += 1;
    }
}

QRgb PubTool::HSLtoRGB(double h, double s, double l)
{
    double r, g, b;
    double temp1, temp2;

    if (s == 0) {
        r = g = b = l;
    } else {
        if (l < 0.5)
            temp2 = l*(1.0 + s);
        else
            temp2 = l + s-l*s;

        temp1 = 2.0 * l-temp2;

        r = HueToRGB(temp1, temp2, h + 1.0/3.0);
        g = HueToRGB(temp1, temp2, h);
        b = HueToRGB(temp1, temp2, h - 1.0/3.0);
    }

    return qRgb((int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0));
}

QRgb PubTool::blendColors(QRgb crA, QRgb crB, double fAmountA)
{
    double fAmountB = (1.0 - fAmountA);
    int btR = (int)(qRed(crA) * fAmountA + qRed(crB) * fAmountB);
    int btG = (int)(qGreen(crA) * fAmountA + qGreen(crB) * fAmountB);
    int btB = (int)(qBlue(crA) * fAmountA + qBlue(crB) * fAmountB);

    return qRgb(qMin(255, btR), qMin(255, btG), qMin(255, btB));
}

double PubTool::HueToRGB(double temp1, double temp2, double temp3)
{
    if (temp3 < 0)
        temp3 = temp3 + 1.0;
    if (temp3 > 1)
        temp3 = temp3-1.0;

    if (6.0*temp3 < 1)
        return (temp1+(temp2-temp1)*temp3 * 6.0);

    else if (2.0*temp3 < 1)
        return temp2;

    else if (3.0*temp3 < 2.0)
        return (temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0);

    return temp1;
}


int PubTool::HueToRGB(int m1, int m2, int h)
{
    if (h < 0)
        h += 255;

    if (h > 255)
        h -= 255;

    if ((6 * h) < 255)
        return ((m1 + ((m2 - m1) / 255 * h * 6)) / 255);

    if ((2 * h) < 255)
        return m2 / 255;

    if ((3 * h) < (2 * 255))
        return ((m1 + (m2 - m1) / 255 * ((255 * 2 / 3) - h) * 6) / 255);

    return (m1 / 255);
}

QLinearGradient PubTool::generateGradient(const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz)
{
    QLinearGradient gradient(rect.topLeft(), bHorz ? rect.bottomLeft() : rect.topRight());
    gradient.setColorAt(0.0, crFrom);
    gradient.setColorAt(1.0, crTo);
    return gradient;
}



