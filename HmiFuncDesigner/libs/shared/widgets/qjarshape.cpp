#include "qjarshape.h"
#include "../qprojectcore.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>

QJarShape::QJarShape(QWidget *parent) : QWidget(parent)
{
    jarShape = "";
    font = QFont("Arial Black", 12);
    borderColor = QColor(0x00, 0x00, 0x00);
    lessThanLowerLimitColor = QColor(0x00, 0xff, 0x00);
    normalColor = QColor(0xff, 0xff, 0x00);
    greaterThanUpperLimitColor = QColor(0xff, 0x00, 0x00);
    showOnInitial = true;
    scaleNum = 5;
    maxValue = 100;
    upperLimitValue = 75;
    lowerLimitValue = 15;
    setPropertyInner();
}

void QJarShape::setPropertyInner()
{
//    setScaledContents(!m_bNoScale);
//    if(!m_bNoScale && (m_szImageFile != "")) {
//        setPixmap(QPixmap::fromImage(m_imageObj));
//    }

//    if(m_iBoardWidth > 0) {
//        QString szStyleSheet = "";
//        szStyleSheet += QString("border-width: %1px; border-style: solid;").arg(QString::number(m_iBoardWidth));
//        szStyleSheet += QString("border-color: rgb(%1, %2, %3);")
//                .arg(QString::number(m_boardColorObj.red()))
//                .arg(QString::number(m_boardColorObj.green()))
//                .arg(QString::number(m_boardColorObj.blue()));
//        this->setStyleSheet(szStyleSheet);
//    } else {
//        this->setStyleSheet("");
//    }
}



void QJarShape::drawJarShape(QPainter *painter)
{
#define TOP_BOTTOM_INTERVAL   4
#define JAR_THICKNESS         1
#define SCALE_WIDTH           3

    int iTextWidth = 0, iTextHeight = 0;
    int iFlags ;
    QRect textRect, scalerRect, jarRect, dispRect;
    int iTmp1 = 0, iTmp3 = 0, iTmp2 = 0;
    int iLowPosition = 0, iUpperPosition = 0;
    float fTmp;
    QString szTmp = "";
    int iRadius,i,x1,y1,x2,y2;
    QPen newPen;
    bool bFirstInto1 = false, bFirstInto2 = false;

    QFontMetrics fm(font);
    //iTextWidth = fm.boundingRect(jarShape_).width();
    //iTextHeight = fm.boundingRect(jarShape_).height();
    iTextWidth = fm.averageCharWidth();
    iTextHeight = fm.height();

    QRect eleRect = QRect(0, 0, width(), height());
    // draw title
    textRect.setLeft(eleRect.left());
    textRect.setTop(eleRect.top());
    textRect.setRight(eleRect.left());
    textRect.setBottom(eleRect.top() + iTextHeight);

    // draw scale meter
    iTmp1 = 1;
    iTmp2 = 1;
    iTmp3 = 0;

    while(1) {
        if( iTmp3 / 10 ) {
            iTmp1++;
            iTmp3 = iTmp3 / 10;
        } else {
            break;
        }
    }

    iTmp3 = static_cast<int>(maxValue);
    while(1) {
        if( iTmp3 / 10 ) {
            iTmp2++;
            iTmp3 = iTmp3 / 10;
        } else {
            break;
        }
    }

    newPen = QPen(borderColor, 1, Qt::SolidLine);
    painter->setPen(newPen);

    iTmp3 = (iTmp1 > iTmp2) ? iTmp1 : iTmp2;

    scalerRect.setLeft(eleRect.left());
    if ( jarShape != "" ) {
        scalerRect.setTop(textRect.bottom() + TOP_BOTTOM_INTERVAL);
    } else {
        scalerRect.setTop(textRect.bottom());
    }

    scalerRect.setRight(eleRect.left() + iTmp3 * iTextWidth+3*SCALE_WIDTH);
    scalerRect.setBottom(eleRect.bottom());

    painter->drawLine(scalerRect.right(), scalerRect.bottom(), scalerRect.right(), scalerRect.top());

    iTmp2= 0;
    iTmp3 = static_cast<int>(maxValue / scaleNum);

    fTmp = std::max(scalerRect.bottom() - scalerRect.top() - 6, 0);
    fTmp = (float)((double)(scaleNum*fTmp)/(double)maxValue);
    iLowPosition = fTmp * ((double)lowerLimitValue / (double)scaleNum);
    iUpperPosition = fTmp * ((double)upperLimitValue / (double)scaleNum);

    fTmp = scalerRect.bottom() - scalerRect.top();
    fTmp = (float)((double)(scaleNum * fTmp) / (double)maxValue);

    while(1) {
        iFlags = Qt::AlignRight | Qt::AlignVCenter;

        if ( iTmp2 == 0 ) {
            dispRect.setLeft(scalerRect.left());
            dispRect.setTop(scalerRect.bottom() - iTextHeight);
            dispRect.setRight(scalerRect.right());
            dispRect.setBottom(scalerRect.bottom());
            painter->drawLine(scalerRect.right(), scalerRect.bottom(),
                              scalerRect.right() - 2 * SCALE_WIDTH, scalerRect.bottom());
            szTmp = QString::number(iTmp2 * scaleNum);
            dispRect.setRight(dispRect.right() - 2 * SCALE_WIDTH);
            painter->drawText(dispRect, iFlags, szTmp);
        } else {
            if ( !(iTmp2 % 5) || (iTmp2 == iTmp3) ) {
                painter->drawLine(scalerRect.right(), scalerRect.bottom() - iTmp2 * fTmp,
                                  scalerRect.right() - 2 * SCALE_WIDTH, scalerRect.bottom() - iTmp2 * fTmp);
            } else {
                painter->drawLine(scalerRect.right(), scalerRect.bottom() - iTmp2 * fTmp,
                                  scalerRect.right() - SCALE_WIDTH, scalerRect.bottom() - iTmp2 * fTmp);
            }

            if( !(iTmp2 % 5) || (iTmp2 == iTmp3) ) {
                if(iTmp2 == iTmp3 ) {
                    szTmp = QString::number(maxValue);
                } else {
                    szTmp = QString::number(iTmp2 * scaleNum);
                }

                dispRect.setLeft(scalerRect.left());
                dispRect.setTop(scalerRect.bottom() - (iTmp2 * fTmp + iTextHeight / 2));
                dispRect.setRight(scalerRect.right());
                dispRect.setBottom(dispRect.top() + iTextHeight);

                if ( dispRect.top() < scalerRect.top() ) {
                    dispRect.setTop(scalerRect.top());
                    dispRect.setBottom(dispRect.top() + iTextHeight);
                }

                dispRect.setRight(dispRect.right() - 2 * SCALE_WIDTH);
                painter->drawText(dispRect, iFlags, szTmp);
            }
        }

        if ( iTmp2 == iTmp3 ) {
            break;
        }

        iTmp2++;
    }


    // begin to fill in the jar
    jarRect = eleRect;
    if(jarShape != "") {
        jarRect.setTop(textRect.bottom() + TOP_BOTTOM_INTERVAL);
        iFlags = Qt::AlignCenter;

        if((textRect.right() - textRect.left() + 1) < (eleRect.right() - (scalerRect.right() + 1) + 1)) {
            textRect.setLeft(scalerRect.right() + 1);
        }

        textRect.setRight(eleRect.right());
        painter->drawText(textRect, iFlags, jarShape);
    }

    jarRect.setLeft(scalerRect.right() + 1);

    if( (jarRect.right() - jarRect.left()) > (jarRect.bottom() - jarRect.top())) {
        iRadius = (jarRect.bottom() - jarRect.top()) / 4;
    } else {
        iRadius = (jarRect.right() - jarRect.left()) / 4;
    }


    fTmp = (maxValue / (scalerRect.bottom() - scalerRect.top()));

    // modify the jarRect because that the width of pen is more than one;
    jarRect.adjust(3, 3, -3, -3);
    newPen = QPen(lessThanLowerLimitColor, 2, Qt::SolidLine);
    painter->setPen(newPen);

    bFirstInto1 = true;
    bFirstInto2 = true;

    for(i = 0; ;i++) {
        // lower
        if(i < iLowPosition) {
            if( i< iRadius) {
                x1=  jarRect.left();
                y1 = jarRect.bottom() - i;
                x2 = jarRect.left() + iRadius;
                y2 = jarRect.bottom()- iRadius;

                while(1) {
                    if( iRadius*iRadius >= ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) ){
                        break;
                    } else {
                        x1++;
                    }
                }
                QPoint pointStart(x1, y1);

                x1 = jarRect.right();
                y1 =  jarRect.bottom() - i;
                x2 =  jarRect.right() - iRadius;
                y2 =  jarRect.bottom() - iRadius;
                while(1) {
                    if( iRadius* iRadius >= ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) ) {
                        break;
                    } else {
                        x1--;
                    }
                }
                QPoint pointEnd(x1, y1);
                painter->drawLine(pointStart, pointEnd);
            } else {
                painter->drawLine(jarRect.left(), jarRect.bottom() - i, jarRect.right(), jarRect.bottom() - i);
            }

        } else if( (i >= iLowPosition)  && (i < iUpperPosition) ) { // normal
            if(bFirstInto1) {
                bFirstInto1 = false;
                newPen = QPen(normalColor, 2, Qt::SolidLine);
                painter->setPen(newPen);
            }

            if(i<iRadius) {
                x1= jarRect.left();
                y1 = jarRect.bottom() - i;
                x2 = jarRect.left() + iRadius;
                y2 = jarRect.bottom() - iRadius;
                while(1) {
                    if( iRadius*iRadius >= ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) ) {
                        break;
                    } else {
                        x1++;
                    }
                }
                QPoint pointStart(x1, y1);

                x1 = jarRect.right();
                y1 =  jarRect.bottom() - i;
                x2 =  jarRect.right() - iRadius;
                y2 =  jarRect.bottom() - iRadius;
                while(1) {
                    if( iRadius* iRadius >= ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) ) {
                        break;
                    } else {
                        x1--;
                    }
                }
                QPoint pointEnd(x1, y1);
                painter->drawLine(pointStart, pointEnd);
            } else {
                painter->drawLine(jarRect.left(), jarRect.bottom()-i,
                                  jarRect.right(), jarRect.bottom()-i);
            }
        }
        else if( (i>=iUpperPosition) &&
                 (i<= (iUpperPosition + (jarRect.bottom() - jarRect.top() - iUpperPosition)/2 )) ) { // upper
            if(bFirstInto2) {
                bFirstInto2 = false;
                newPen = QPen(greaterThanUpperLimitColor, 2, Qt::SolidLine);
                painter->setPen(newPen);
            }

            if ( i < iRadius ) {
                x1= jarRect.left();
                y1 = jarRect.bottom() - i;
                x2 = jarRect.left() + iRadius;
                y2 = jarRect.bottom() - iRadius;

                while(1) {
                    if( iRadius*iRadius >= ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) ) {
                        break;
                    } else {
                        x1++;
                    }
                }
                QPoint pointStart(x1, y1);

                x1 = jarRect.right();
                y1 =  jarRect.bottom() - i;
                x2 =  jarRect.right() - iRadius;
                y2 =  jarRect.bottom() - iRadius;
                while(1) {
                    if( iRadius* iRadius >= ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) ) {
                        break;
                    } else {
                        x1--;
                    }
                }

                QPoint pointEnd(x1, y1);
                painter->drawLine(pointStart, pointEnd);
            } else {
                painter->drawLine(jarRect.left(), jarRect.bottom() - i,
                                  jarRect.right(), jarRect.bottom() - i);
            }
        } else {
            break;
        }
    }

    // draw JarShape
    // jarRect
    iTmp1 = 2;
    newPen = QPen(borderColor);
    painter->setPen(newPen);
    painter->drawLine(jarRect.left() - iTmp1, jarRect.top() - iTmp1,
                        jarRect.right() + iTmp1, jarRect.top() - iTmp1);

    ////////////////////////////////////////////////////////////////////////////

    newPen = QPen(borderColor, 2, Qt::SolidLine);
    painter->setPen(newPen);
    // because of the width of pen is more than one ,so adjust the jarRect
    jarRect.adjust(-1, -1, 1, 1);
    for(iTmp1 =0; iTmp1<JAR_THICKNESS; iTmp1++) {
        // draw left side of jar
        painter->drawLine(jarRect.left() + iTmp1, jarRect.top(),
                          jarRect.left() + iTmp1, jarRect.bottom() - iRadius);

        dispRect.setLeft(jarRect.left() + iTmp1);
        dispRect.setTop(jarRect.bottom() - 2 * iRadius+iTmp1);
        dispRect.setRight(dispRect.left() + 2 * iRadius-iTmp1);
        dispRect.setBottom(jarRect.bottom() - iTmp1 - 1);

        int startAngle = 180 * 16;
        int spanAngle = 90 * 16;
        painter->drawArc(dispRect, startAngle, spanAngle);
    }

    for(iTmp1=0; iTmp1<JAR_THICKNESS; iTmp1++) {
        // draw right side of jar
        painter->drawLine(jarRect.right() - iTmp1,jarRect.top(),
                          jarRect.right() - iTmp1,jarRect.bottom() - iRadius);

        dispRect.setLeft(jarRect.right() - 2 * iRadius+iTmp1);
        dispRect.setTop(jarRect.bottom() - 2 * iRadius+iTmp1);
        dispRect.setRight(jarRect.right() - iTmp1 - 1);
        dispRect.setBottom(jarRect.bottom() - iTmp1 - 1);

        int startAngle = 270 * 16;
        int spanAngle = 90 * 16;
        painter->drawArc(dispRect, startAngle, spanAngle);
    }
    painter->drawLine(jarRect.left() + iRadius, jarRect.bottom(),
                      jarRect.right() - iRadius, jarRect.bottom());
}



void QJarShape::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    // 绘制罐形容器
    drawJarShape(&painter);
    QWidget::paintEvent(event);
}

void QJarShape::resizeEvent(QResizeEvent *event)
{
    setPropertyInner();
    QWidget::resizeEvent(event);
}

bool QJarShape::getShowOnInitial() const
{
    return showOnInitial;
}

void QJarShape::setShowOnInitial(bool value)
{
    if(value != showOnInitial) {
        showOnInitial = value;
        this->update();
    }
}

int QJarShape::getScaleNum() const
{
    return scaleNum;
}

void QJarShape::setScaleNum(int value)
{
    if(value != scaleNum) {
        scaleNum = value;
        this->update();
    }
}

double QJarShape::getMaxValue() const
{
    return maxValue;
}

void QJarShape::setMaxValue(double value)
{
    if(value != maxValue) {
        maxValue = value;
        this->update();
    }
}

double QJarShape::getLowerLimitValue() const
{
    return lowerLimitValue;
}

void QJarShape::setLowerLimitValue(double value)
{
    if(value != lowerLimitValue) {
        lowerLimitValue = value;
        this->update();
    }
}

double QJarShape::getUpperLimitValue() const
{
    return upperLimitValue;
}

void QJarShape::setUpperLimitValue(double value)
{
    if(value != upperLimitValue) {
        upperLimitValue = value;
        this->update();
    }
}

QColor QJarShape::getGreaterThanUpperLimitColor() const
{
    return greaterThanUpperLimitColor;
}

void QJarShape::setGreaterThanUpperLimitColor(const QColor &value)
{
    if(value != greaterThanUpperLimitColor) {
        greaterThanUpperLimitColor = value;
        this->update();
    }
}

QColor QJarShape::getNormalColor() const
{
    return normalColor;
}

void QJarShape::setNormalColor(const QColor &value)
{
    if(value != normalColor) {
        normalColor = value;
        this->update();
    }
}

QColor QJarShape::getLessThanLowerLimitColor() const
{
    return lessThanLowerLimitColor;
}

void QJarShape::setLessThanLowerLimitColor(const QColor &value)
{
    if(value != lessThanLowerLimitColor) {
        lessThanLowerLimitColor = value;
        this->update();
    }
}

QColor QJarShape::getBorderColor() const
{
    return borderColor;
}

void QJarShape::setBorderColor(const QColor &value)
{
    if(value != borderColor) {
        borderColor = value;
        this->update();
    }
}

QFont QJarShape::getFont() const
{
    return font;
}

void QJarShape::setFont(const QFont &value)
{
    if(value != font) {
        font = value;
        this->update();
    }
}

QString QJarShape::getJarShape() const
{
    return jarShape;
}

void QJarShape::setJarShape(const QString &value)
{
    if(value != jarShape) {
        jarShape = value;
        this->update();
    }
}

QString QJarShape::getTagSelected() const
{
    return tagSelected;
}

void QJarShape::setTagSelected(const QString &value)
{
    if(value != tagSelected) {
        tagSelected = value;
        this->update();
    }
}


