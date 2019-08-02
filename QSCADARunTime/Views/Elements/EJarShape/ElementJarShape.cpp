#include "ElementJarShape.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "SCADARunTime.h"
#include <QDateTime>
#include <QDate>
#include <QFontMetrics>
#include <cfloat>
#include <QDebug>



#define SCALE_LENTH         4
#define BIT_SCALE_LENTH     6
#define MIN_BAR_LENTH       5


ElementJarShape::ElementJarShape()
    : Element()
{
    elementId = tr("JarShape");
    internalElementType = tr("JarShape");
    font_ = QFont("Arial Black", 12);
    borderColor_ = QColor(0x00, 0x00, 0x00);
    lessThanLowerLimitColor_ = QColor(0x00, 0xff, 0x00);
    normalColor_ = QColor(0xff, 0xff, 0x00);
    greaterThanUpperLimitColor_ = QColor(0xff, 0x00, 0x00);
    showOnInitial_ = true;
    scaleNum_ = 5;
    maxValue_ = 100;
    upperLimitValue_ = 75;
    lowerLimitValue_ = 15;
    jarShape_ = "";
    init();
}

QRectF ElementJarShape::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementJarShape::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementJarShape::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementJarShape::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementJarShape::paint(QPainter *painter)
{
    if(!showOnInitial_) {
        return;
    }

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    // 绘制罐形容器
    drawJarShape(painter);
}


void ElementJarShape::drawJarShape(QPainter *painter)
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

    // 变量当前值
    double dTagValue = 0.0;
    QString szTagValue = "#";

    // 已关联变量
    if ( szTagSelected_ != "" ) {

        QString szTagID = RealTimeDB::getIdByTagName(szTagSelected_);
        if (szTagID != "") {
            szTagValue = RealTimeDB::GetDataString(szTagID);
        } else {
            szTagValue = "#";
        }
    }

    if ( szTagValue != "#" ) {
        bool ok;
        double dVal = szTagValue.toDouble(&ok);
        if ( ok ) {
            dTagValue = dVal;
        }
    }


    QFontMetrics fm(font_);
    //iTextWidth = fm.boundingRect(jarShape_).width();
    //iTextHeight = fm.boundingRect(jarShape_).height();
    iTextWidth = fm.averageCharWidth();
    iTextHeight = fm.height();

    QRect eleRect = elementRect_.toRect();
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

    iTmp3 = static_cast<int>(maxValue_);
    while(1) {
        if( iTmp3 / 10 ) {
            iTmp2++;
            iTmp3 = iTmp3 / 10;
        } else {
            break;
        }
    }

    newPen = QPen(borderColor_, 1, Qt::SolidLine);
    painter->setPen(newPen);

    iTmp3 = (iTmp1 > iTmp2) ? iTmp1 : iTmp2;

    scalerRect.setLeft(eleRect.left());
    if ( jarShape_ != "" ) {
        scalerRect.setTop(textRect.bottom() + TOP_BOTTOM_INTERVAL);
    } else {
        scalerRect.setTop(textRect.bottom());
    }

    scalerRect.setRight(eleRect.left() + iTmp3 * iTextWidth+3*SCALE_WIDTH);
    scalerRect.setBottom(eleRect.bottom());

    painter->drawLine(scalerRect.right(), scalerRect.bottom(), scalerRect.right(), scalerRect.top());

    iTmp2= 0;
    iTmp3 = static_cast<int>(maxValue_/scaleNum_);

    fTmp = __max(scalerRect.bottom() - scalerRect.top() - 6, 0);
    fTmp = (float)((double)(scaleNum_*fTmp)/(double)maxValue_);
    iLowPosition = fTmp * ((double)lowerLimitValue_ / (double)scaleNum_);
    iUpperPosition = fTmp * ((double)upperLimitValue_ / (double)scaleNum_);

    fTmp = scalerRect.bottom() - scalerRect.top();
    fTmp = (float)((double)(scaleNum_*fTmp)/(double)maxValue_);

    if(dTagValue > maxValue_)
        dTagValue = maxValue_;

    if(dTagValue < 0)
         dTagValue = 0;

    dTagValue = fTmp * (dTagValue / (double)scaleNum_);

    while(1) {
        iFlags = Qt::AlignRight | Qt::AlignVCenter;

        if ( iTmp2 == 0 ) {
            dispRect.setLeft(scalerRect.left());
            dispRect.setTop(scalerRect.bottom() - iTextHeight);
            dispRect.setRight(scalerRect.right());
            dispRect.setBottom(scalerRect.bottom());
            painter->drawLine(scalerRect.right(), scalerRect.bottom(),
                              scalerRect.right() - 2 * SCALE_WIDTH, scalerRect.bottom());
            szTmp = QString::number(iTmp2 * scaleNum_);
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
                    szTmp = QString::number(maxValue_);
                } else {
                    szTmp = QString::number(iTmp2 * scaleNum_);
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
    if(jarShape_ != "") {
        jarRect.setTop(textRect.bottom() + TOP_BOTTOM_INTERVAL);
        iFlags = Qt::AlignCenter;

        if((textRect.right() - textRect.left() + 1) < (eleRect.right() - (scalerRect.right() + 1) + 1)) {
            textRect.setLeft(scalerRect.right() + 1);
        }

        textRect.setRight(eleRect.right());
        painter->drawText(textRect, iFlags, jarShape_);
    }

    jarRect.setLeft(scalerRect.right() + 1);

    if( (jarRect.right() - jarRect.left()) > (jarRect.bottom() - jarRect.top())) {
        iRadius = (jarRect.bottom() - jarRect.top()) / 4;
    } else {
        iRadius = (jarRect.right() - jarRect.left()) / 4;
    }


    fTmp = (maxValue_/(scalerRect.bottom() - scalerRect.top()));

    // modify the jarRect because that the width of pen is more than one;
    jarRect.adjust(3, 3, -3, -3);
    newPen = QPen(lessThanLowerLimitColor_, 2, Qt::SolidLine);
    painter->setPen(newPen);

    bFirstInto1 = true;
    bFirstInto2 = true;

    iTmp3 = static_cast<int>(dTagValue);
    for(i = 0; i<iTmp3; i++) {
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
                newPen = QPen(normalColor_, 2, Qt::SolidLine);
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
        else if( i >= iUpperPosition ) { // upper
            if(bFirstInto2) {
                bFirstInto2 = false;
                newPen = QPen(greaterThanUpperLimitColor_, 2, Qt::SolidLine);
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
    newPen = QPen(borderColor_);
    painter->setPen(newPen);
    painter->drawLine(jarRect.left() - iTmp1, jarRect.top() - iTmp1,
                        jarRect.right() + iTmp1, jarRect.top() - iTmp1);

    ////////////////////////////////////////////////////////////////////////////

    newPen = QPen(borderColor_, 2, Qt::SolidLine);
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


void ElementJarShape::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementJarShape::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementJarShape::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementJarShape::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("jarShape")) {
        jarShape_ = attributes.value("jarShape").toString();
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("lessThanLowerLimitColor")) {
        lessThanLowerLimitColor_ = QColor(attributes.value("lessThanLowerLimitColor").toString());
    }

    if (attributes.hasAttribute("normalColor")) {
        normalColor_ = QColor(attributes.value("normalColor").toString());
    }

    if (attributes.hasAttribute("greaterThanUpperLimitColor")) {
        greaterThanUpperLimitColor_ = QColor(attributes.value("greaterThanUpperLimitColor").toString());
    }

    if (attributes.hasAttribute("upperLimitValue")) {
        upperLimitValue_ = attributes.value("upperLimitValue").toDouble();
    }

    if (attributes.hasAttribute("lowerLimitValue")) {
        lowerLimitValue_ = attributes.value("lowerLimitValue").toDouble();
    }

    if (attributes.hasAttribute("maxValue")) {
        maxValue_ = attributes.value("maxValue").toDouble();
    }

    if (attributes.hasAttribute("scaleNum")) {
        scaleNum_ = attributes.value("scaleNum").toInt();
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("x")) {
        setElementXPos(attributes.value("x").toString().toInt());
    }

    if (attributes.hasAttribute("y")) {
        setElementYPos(attributes.value("y").toString().toInt());
    }

    if (attributes.hasAttribute("z")) {
        setElementZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    updateBoundingElement();
}


void ElementJarShape::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QString jarShape;
    QString font;
    QColor textColor;
    QColor borderColor;
    QColor lessThanLowerLimitColor;
    QColor normalColor;
    QColor greaterThanUpperLimitColor;
    double upperLimitValue;
    double lowerLimitValue;
    double maxValue;
    int scaleNum;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> szTagSelected
       >> jarShape
       >> font
       >> textColor
       >> borderColor
       >> lessThanLowerLimitColor
       >> normalColor
       >> greaterThanUpperLimitColor
       >> upperLimitValue
       >> lowerLimitValue
       >> maxValue
       >> scaleNum
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    this->setElementId(id);
    this->szTagSelected_ = szTagSelected;
    this->jarShape_ = jarShape;
    this->font_ = font;
    this->textColor = textColor;
    this->borderColor_ = borderColor;
    this->lessThanLowerLimitColor_ = lessThanLowerLimitColor;
    this->normalColor_ = normalColor;
    this->greaterThanUpperLimitColor_ = greaterThanUpperLimitColor;
    this->upperLimitValue_ = upperLimitValue;
    this->lowerLimitValue_ = lowerLimitValue;
    this->maxValue_ = maxValue;
    this->scaleNum_ = scaleNum;
    this->showOnInitial_ = showOnInitial;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in, ElementJarShape &ele)
{
    QString id;
    QString szTagSelected;
    QString jarShape;
    QString font;
    QColor textColor;
    QColor borderColor;
    QColor lessThanLowerLimitColor;
    QColor normalColor;
    QColor greaterThanUpperLimitColor;
    double upperLimitValue;
    double lowerLimitValue;
    double maxValue;
    int scaleNum;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> szTagSelected
       >> jarShape
       >> font
       >> textColor
       >> borderColor
       >> lessThanLowerLimitColor
       >> normalColor
       >> greaterThanUpperLimitColor
       >> upperLimitValue
       >> lowerLimitValue
       >> maxValue
       >> scaleNum
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    ele.setElementId(id);
    ele.szTagSelected_ = szTagSelected;
    ele.jarShape_ = jarShape;
    ele.font_ = font;
    ele.textColor = textColor;
    ele.borderColor_ = borderColor;
    ele.lessThanLowerLimitColor_ = lessThanLowerLimitColor;
    ele.normalColor_ = normalColor;
    ele.greaterThanUpperLimitColor_ = greaterThanUpperLimitColor;
    ele.upperLimitValue_ = upperLimitValue;
    ele.lowerLimitValue_ = lowerLimitValue;
    ele.maxValue_ = maxValue;
    ele.scaleNum_ = scaleNum;
    ele.showOnInitial_ = showOnInitial;
    ele.setElementXPos(static_cast<int>(xpos));
    ele.setElementYPos(static_cast<int>(ypos));
    ele.setElementZValue(static_cast<int>(zvalue));
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.updateBoundingElement();

    return in;
}





