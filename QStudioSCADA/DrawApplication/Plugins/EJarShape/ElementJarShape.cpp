#include "ElementJarShape.h"
#include "TagManager.h"
#include "PubTool.h"
#include <cfloat>
#include <QFontMetrics>
#include <algorithm>
#include <QDebug>


int ElementJarShape::iLastIndex_ = 1;

ElementJarShape::ElementJarShape(const QString &szProjPath, const QString &szProjName)
    : Element(szProjPath, szProjName)
{
    elementId = QString(tr("JarShape_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    jarShape_ = "";
    internalElementType = tr("JarShape");
    elementIcon = QIcon(":/images/JarShape.png");
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
    TagManager::setProjectPath(szProjectPath_);
    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementJarShape::regenerateElementId()
{
    elementId = QString(tr("JarShape_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementJarShape::release
 * @details 释放占用的资源
 */
void ElementJarShape::release()
{

}


QRectF ElementJarShape::boundingRect() const
{
    qreal extra = 5;
    QRectF QRect(elementRect.toRect());
    return QRect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementJarShape::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
    return path;
}

void ElementJarShape::createPropertyList()
{
    // ID
    idProperty_ = new TextProperty(tr("ID"));
    idProperty_->setId(EL_ID);
    idProperty_->setReadOnly(true);
    propList.insert(propList.end(), idProperty_);

    // 标题
    titleProperty_ = new EmptyProperty(tr("标题"));
    propList.insert(propList.end(), titleProperty_);

	// 选择变量
	tagSelectProperty_ = new ListProperty(tr("选择变量"));
	tagSelectProperty_->setId(EL_TAG);
	QStringList varList;
	TagManager::getAllTagName(TagManager::getProjectPath(), varList);
	tagSelectProperty_->setList(varList);
	propList.insert(propList.end(), tagSelectProperty_);

    // 罐形容器标题
    jarShapeProperty_ = new TextProperty(tr("容器标题"));
    jarShapeProperty_->setId(EL_TEXT);
    jarShapeProperty_->setValue(jarShape_);
    propList.insert(propList.end(), jarShapeProperty_);

    // 字体
    fontProperty_ = new FontProperty(tr("字体"));
    fontProperty_->setId(EL_FONT);
    fontProperty_->setValue(font_);
    propList.insert(propList.end(), fontProperty_);

    // 文本颜色
    textColorProperty_ = new ColorProperty(tr("文本颜色"));
    textColorProperty_->setId(EL_FONT_COLOR);
    textColorProperty_->setValue(textColor);
    propList.insert(propList.end(), textColorProperty_);

    // 罐体颜色
    borderColorProperty_ = new ColorProperty(tr("罐体颜色"));
    borderColorProperty_->setId(EL_BORDER_COLOR);
    borderColorProperty_->setValue(borderColor_);
    propList.insert(propList.end(), borderColorProperty_);

    // 低于下限颜色
    lessThanLowerLimitColorProperty_ = new ColorProperty(tr("低于下限颜色"));
    lessThanLowerLimitColorProperty_->setId(EL_LESS_THAN_LOWER_LIMIT_COLOR);
    lessThanLowerLimitColorProperty_->setValue(lessThanLowerLimitColor_);
    propList.insert(propList.end(), lessThanLowerLimitColorProperty_);

    // 正常液面颜色
    normalColorProperty_ = new ColorProperty(tr("正常液面颜色"));
    normalColorProperty_->setId(EL_NORMAL_COLOR);
    normalColorProperty_->setValue(normalColor_);
    propList.insert(propList.end(), normalColorProperty_);

    // 高于上限颜色
    greaterThanUpperLimitColorProperty_ = new ColorProperty(tr("高于上限颜色"));
    greaterThanUpperLimitColorProperty_->setId(EL_GREATER_THAN_UPPER_LIMIT_COLOR);
    greaterThanUpperLimitColorProperty_->setValue(greaterThanUpperLimitColor_);
    propList.insert(propList.end(), greaterThanUpperLimitColorProperty_);

    // 上限值
    upperLimitValueProperty_ = new DoubleProperty(tr("上限值"));
    upperLimitValueProperty_->setSettings(DBL_MIN, DBL_MAX, 5);
    upperLimitValueProperty_->setId(EL_UPPER_LIMIT_VALUE);
    upperLimitValueProperty_->setValue(upperLimitValue_);
    propList.insert(propList.end(), upperLimitValueProperty_);

    // 下限值
    lowerLimitValueProperty_ = new DoubleProperty(tr("下限值"));
    lowerLimitValueProperty_->setSettings(DBL_MIN, DBL_MAX, 5);
    lowerLimitValueProperty_->setId(EL_LOWER_LIMIT_VALUE);
    lowerLimitValueProperty_->setValue(lowerLimitValue_);
    propList.insert(propList.end(), lowerLimitValueProperty_);

    // 满量程值-刻度最大值
    maxValueProperty_ = new DoubleProperty(tr("满量程值"));
    maxValueProperty_->setSettings(DBL_MIN, DBL_MAX, 5);
    maxValueProperty_->setId(EL_SCALE_VALUE_MAX);
    maxValueProperty_->setValue(maxValue_);
    propList.insert(propList.end(), maxValueProperty_);

    // 刻度个数
    scaleNumProperty_ = new IntegerProperty(tr("刻度个数"));
    scaleNumProperty_->setSettings(0, 5000);
    scaleNumProperty_->setId(EL_SCALE_NUM);
    scaleNumProperty_->setValue(scaleNum_);
    propList.insert(propList.end(), scaleNumProperty_);

    // 初始可见性
    showOnInitialProperty_ = new BoolProperty(tr("初始可见性"));
    showOnInitialProperty_->setId(EL_SHOW_ON_INITIAL);
    showOnInitialProperty_->setTrueText(tr("显示"));
    showOnInitialProperty_->setFalseText(tr("不显示"));
    showOnInitialProperty_->setValue(showOnInitial_);
    propList.insert(propList.end(), showOnInitialProperty_);

    // 坐标 X
    xCoordProperty = new IntegerProperty(tr("坐标 X"));
    xCoordProperty->setSettings(0, 5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(), xCoordProperty);

    // 坐标 Y
    yCoordProperty = new IntegerProperty(tr("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0, 5000);
    propList.insert(propList.end(), yCoordProperty);

    // Z 值
    zValueProperty = new IntegerProperty(tr("Z 值"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000, 1000);
    propList.insert(propList.end(), zValueProperty);

    // 宽度
    widthProperty = new IntegerProperty(tr("宽度"));
    widthProperty->setId(EL_WIDTH);
    widthProperty->setSettings(0, 5000);
    propList.insert(propList.end(), widthProperty);

    // 高度
    heightProperty = new IntegerProperty(tr("高度"));
    heightProperty->setId(EL_HEIGHT);
    heightProperty->setSettings(0, 5000);
    propList.insert(propList.end(), heightProperty);
}

void ElementJarShape::updateElementProperty(uint id, const QVariant &value)
{
    switch (id) {
    case EL_ID:
        elementId = value.toString();
        break;
	case EL_TAG:
		szTagSelected_ = value.toString();
		break;
    case EL_TEXT:
        jarShape_ = value.toString();
        break;
    case EL_FONT:
        font_ = value.value<QFont>();
        break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_BORDER_COLOR:
        borderColor_ = value.value<QColor>();
        break;
    case EL_LESS_THAN_LOWER_LIMIT_COLOR:
        lessThanLowerLimitColor_ = value.value<QColor>();
        break;
    case EL_NORMAL_COLOR:
        normalColor_ = value.value<QColor>();
        break;
    case EL_GREATER_THAN_UPPER_LIMIT_COLOR:
        greaterThanUpperLimitColor_ = value.value<QColor>();
        break;
    case EL_UPPER_LIMIT_VALUE:
        upperLimitValue_ = value.toDouble();
        break;
    case EL_LOWER_LIMIT_VALUE:
        lowerLimitValue_ = value.toDouble();
        break;
    case EL_SCALE_VALUE_MAX:
        maxValue_ = value.toDouble();
        break;
    case EL_SCALE_NUM:
        scaleNum_ = value.toInt();
        break;
    case EL_SHOW_ON_INITIAL:
        showOnInitial_ = value.toBool();
        break;
    case EL_X:
        elementXPos = value.toInt();
        setElementXPos(elementXPos);
        break;
    case EL_Y:
        elementYPos = value.toInt();
        setElementYPos(elementYPos);
        break;
    case EL_Z_VALUE:
        elementZValue = value.toInt();
        setZValue(elementZValue);
        break;
    case EL_WIDTH:
        elementWidth = value.toInt();
        updateBoundingElement();
        break;
    case EL_HEIGHT:
        elementHeight = value.toInt();
        updateBoundingElement();
        break;
    }

    update();
    scene()->update();
}

void ElementJarShape::updatePropertyModel()
{
    idProperty_->setValue(elementId);
	tagSelectProperty_->setValue(szTagSelected_);
    jarShapeProperty_->setValue(jarShape_);
    fontProperty_->setValue(font_);
    textColorProperty_->setValue(textColor);
    borderColorProperty_->setValue(borderColor_);
    lessThanLowerLimitColorProperty_->setValue(lessThanLowerLimitColor_);
    normalColorProperty_->setValue(normalColor_);
    greaterThanUpperLimitColorProperty_->setValue(greaterThanUpperLimitColor_);
    upperLimitValueProperty_->setValue(upperLimitValue_);
    lowerLimitValueProperty_->setValue(lowerLimitValue_);
    maxValueProperty_->setValue(maxValue_);
    scaleNumProperty_->setValue(scaleNum_);
    showOnInitialProperty_->setValue(showOnInitial_);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
}

void ElementJarShape::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementJarShape::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementJarShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 绘制罐形容器
    drawJarShape(painter);

    if (isSelected()) {
        painter->setPen(QPen(borderColor));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
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

    QFontMetrics fm(font_);
    //iTextWidth = fm.boundingRect(jarShape_).width();
    //iTextHeight = fm.boundingRect(jarShape_).height();
    iTextWidth = fm.averageCharWidth();
    iTextHeight = fm.height();

    QRect eleRect = elementRect.toRect();
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

    fTmp = std::max(scalerRect.bottom() - scalerRect.top() - 6, 0);
    fTmp = (float)((double)(scaleNum_*fTmp)/(double)maxValue_);
    iLowPosition = fTmp * ((double)lowerLimitValue_ / (double)scaleNum_);
    iUpperPosition = fTmp * ((double)upperLimitValue_ / (double)scaleNum_);

    fTmp = scalerRect.bottom() - scalerRect.top();
    fTmp = (float)((double)(scaleNum_*fTmp)/(double)maxValue_);

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
        else if( (i>=iUpperPosition) &&
                 (i<= (iUpperPosition + (jarRect.bottom() - jarRect.top() - iUpperPosition)/2 )) ) { // upper
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

void ElementJarShape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();

    if (resizing) {
        setCursor(Qt::SizeFDiagCursor);

        switch (rd) {
        case RdBottomRight:
            elementRect.setBottomRight(mousePoint);
            elementWidth = static_cast<int>(qAbs(elementRect.topLeft().x() - elementRect.bottomRight().x()));
            elementHeight = static_cast<int>(qAbs(elementRect.topLeft().y() - elementRect.bottomRight().y()));
            break;
        case RdTopLeft:
            elementRect.setTopLeft(mousePoint);
            setElementXPos(static_cast<int>(mapToScene(elementRect.topLeft()).x()));
            setElementYPos(static_cast<int>(mapToScene(elementRect.topLeft()).y()));
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.bottomRight()).x())));
            setElementHeight(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).y() - mapToScene(elementRect.bottomRight()).y())));
            updateBoundingElement();
            break;
        case RdNone:
            QGraphicsObject::mouseMoveEvent(event);
            break;
        }

        scene()->update();
        return;
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        // 限制矩形区域
        RestrictedRectangularRegion();
    }
}


void ElementJarShape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
        rd = RdBottomRight;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else {
        resizing = false;
        rd = RdNone;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

void ElementJarShape::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    elementXPos = static_cast<int>(pos().x());
    elementYPos = static_cast<int>(pos().y());
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementJarShape::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}


void ElementJarShape::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
	writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("jarShape", jarShape_);
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("lessThanLowerLimitColor", lessThanLowerLimitColor_.name());
    writer.writeAttribute("normalColor", normalColor_.name());
    writer.writeAttribute("greaterThanUpperLimitColor", greaterThanUpperLimitColor_.name());
    writer.writeAttribute("upperLimitValue", QString::number(upperLimitValue_));
    writer.writeAttribute("lowerLimitValue", QString::number(lowerLimitValue_));
    writer.writeAttribute("maxValue", QString::number(maxValue_));
    writer.writeAttribute("scaleNum", QString::number(scaleNum_));
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeEndElement();
}

void ElementJarShape::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
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
        setZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    updateBoundingElement();
    updatePropertyModel();
}

void ElementJarShape::writeData(QDataStream &out)
{
    out << this->elementId
		<< this->szTagSelected_
        << this->jarShape_
        << this->font_.toString()
        << this->textColor
        << this->borderColor_
        << this->lessThanLowerLimitColor_
        << this->normalColor_
        << this->greaterThanUpperLimitColor_
        << this->upperLimitValue_
        << this->lowerLimitValue_
        << this->maxValue_
        << this->scaleNum_
        << this->showOnInitial_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight;
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
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
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
    this->updatePropertyModel();
}

QDataStream &operator<<(QDataStream &out,const ElementJarShape &ele)
{
    out << ele.elementId
        << ele.szTagSelected_
        << ele.jarShape_
        << ele.font_
        << ele.textColor
        << ele.borderColor_
        << ele.lessThanLowerLimitColor_
        << ele.normalColor_
        << ele.greaterThanUpperLimitColor_
        << ele.upperLimitValue_
        << ele.lowerLimitValue_
        << ele.maxValue_
        << ele.scaleNum_
        << ele.showOnInitial_
        << ele.x()
        << ele.y()
        << ele.zValue()
        << ele.elementWidth
        << ele.elementHeight;
    return out;
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
    int index = ele.getIndexFromIDString(id);
    if(ele.iLastIndex_ < index) {
        ele.iLastIndex_ = index;
    }
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
    ele.updatePropertyModel();

    return in;
}

