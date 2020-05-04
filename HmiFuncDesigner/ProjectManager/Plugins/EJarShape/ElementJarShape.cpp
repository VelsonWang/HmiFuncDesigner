#include "ElementJarShape.h"
#include "ProjectData.h"
#include "PubTool.h"
#include <cfloat>
#include <QFontMetrics>
#include <algorithm>
#include "variantmanager.h"
#include "editbasicpropertydialog.h"

int ElementJarShape::iLastIndex_ = 1;

ElementJarShape::ElementJarShape(const QString &szProjPath,
                                 const QString &szProjName,
                                 QtVariantPropertyManager *propertyMgr)
    : Element(szProjPath, szProjName, propertyMgr)
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
    init();
    if(ProjectData::getInstance()->getDBPath() == "")
        ProjectData::getInstance()->createOrOpenProjectData(szProjectPath_, szProjectName_);
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
    ProjectData::releaseInstance();
}


QRectF ElementJarShape::boundingRect() const
{
    qreal extra = 5;
    QRectF QRect(elementRect.toRect());
    return QRect.normalized().adjusted(-extra,-extra,extra,extra);
}


void ElementJarShape::createPropertyList()
{
    propList.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;

    // ID
    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    // 选择变量
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("选择变量"));
    tagNames_.clear();
    ProjectData::getInstance()->getAllTagName(tagNames_);
    if(tagNames_.size() > 0) szTagSelected_ = tagNames_.at(0);
    property->setAttribute(QLatin1String("enumNames"), tagNames_);
    addProperty(property, QLatin1String("tag"));

    // 罐形容器标题
    property = variantPropertyManager_->addProperty(QVariant::String, tr("容器标题"));
    addProperty(property, QLatin1String("text"));

    // 字体
    property = variantPropertyManager_->addProperty(QVariant::Font, tr("字体"));
    addProperty(property, QLatin1String("font"));

    // 文本颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("文本颜色"));
    addProperty(property, QLatin1String("textColor"));

    // 罐体颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("罐体颜色"));
    addProperty(property, QLatin1String("borderColor"));

    // 低于下限颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("低于下限颜色"));
    addProperty(property, QLatin1String("lessThanLowerLimitColor"));

    // 正常液面颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("正常液面颜色"));
    addProperty(property, QLatin1String("normalColor"));

    // 高于上限颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("高于上限颜色"));
    addProperty(property, QLatin1String("greaterThanUpperLimitColor"));

    // 上限值
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("上限值"));
    property->setAttribute(QLatin1String("minimum"), DBL_MIN);
    property->setAttribute(QLatin1String("maximum"), DBL_MAX);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("upperLimitValue"));

    // 下限值
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("下限值"));
    property->setAttribute(QLatin1String("minimum"), DBL_MIN);
    property->setAttribute(QLatin1String("maximum"), DBL_MAX);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("lowerLimitValue"));

    // 满量程值-刻度最大值
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("满量程值"));
    property->setAttribute(QLatin1String("minimum"), DBL_MIN);
    property->setAttribute(QLatin1String("maximum"), DBL_MAX);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("maxValue"));

    // 刻度个数
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("刻度个数"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("scaleNum"));

    // 初始可见性
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始可见性"));
    addProperty(property, QLatin1String("showOnInitial"));

    // 坐标 X
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("坐标 X"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("xCoord"));

    // 坐标 Y
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("坐标 Y"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("yCoord"));

    // Z 值
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("Z 值"));
    property->setAttribute(QLatin1String("minimum"), -1000);
    property->setAttribute(QLatin1String("maximum"), 1000);
    addProperty(property, QLatin1String("zValue"));

    // 宽度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("宽度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("width"));

    // 高度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("高度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("height"));
}

void ElementJarShape::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("text")) {
        jarShape_ = value.toString();
    } else if (id == QLatin1String("font")) {
        font_ = value.value<QFont>();
    } else if (id == QLatin1String("textColor")) {
        textColor = value.value<QColor>();
    } else if (id == QLatin1String("borderColor")) {
        borderColor_ = value.value<QColor>();
    } else if (id == QLatin1String("lessThanLowerLimitColor")) {
        lessThanLowerLimitColor_ = value.value<QColor>();
    } else if (id == QLatin1String("normalColor")) {
        normalColor_ = value.value<QColor>();
    } else if (id == QLatin1String("greaterThanUpperLimitColor")) {
        greaterThanUpperLimitColor_ = value.value<QColor>();
    } else if (id == QLatin1String("upperLimitValue")) {
        upperLimitValue_ = value.toDouble();
    } else if (id == QLatin1String("lowerLimitValue")) {
        lowerLimitValue_ = value.toDouble();
    } else if (id == QLatin1String("maxValue")) {
        maxValue_ = value.toDouble();
    } else if (id == QLatin1String("scaleNum")) {
        scaleNum_ = value.toInt();
    } else if (id == QLatin1String("showOnInitial")) {
        showOnInitial_ = value.toBool();
    } else if (id == QLatin1String("xCoord")) {
        elementXPos = value.toInt();
        setElementXPos(elementXPos);
    } else if (id == QLatin1String("yCoord")) {
        elementYPos = value.toInt();
        setElementYPos(elementYPos);
    } else if (id == QLatin1String("zValue")) {
        elementZValue = value.toInt();
        setZValue(elementZValue);
    } else if (id == QLatin1String("width")) {
        elementWidth = value.toInt();
        updateBoundingElement();
    } else if (id == QLatin1String("height")) {
        elementHeight = value.toInt();
        updateBoundingElement();
    }

    scene()->update();
    update();
}

void ElementJarShape::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(elementId);
    }

    property = idToProperty_[QLatin1String("tag")];
    if(property != Q_NULLPTR) {
        property->setValue(tagNames_.indexOf(szTagSelected_));
    }

    property = idToProperty_[QLatin1String("text")];
    if(property != Q_NULLPTR) {
        property->setValue(jarShape_);
    }

    property = idToProperty_[QLatin1String("font")];
    if(property != Q_NULLPTR) {
        property->setValue(font_);
    }

    property = idToProperty_[QLatin1String("textColor")];
    if(property != Q_NULLPTR) {
        property->setValue(textColor);
    }

    property = idToProperty_[QLatin1String("borderColor")];
    if(property != Q_NULLPTR) {
        property->setValue(borderColor_);
    }

    property = idToProperty_[QLatin1String("lessThanLowerLimitColor")];
    if(property != Q_NULLPTR) {
        property->setValue(lessThanLowerLimitColor_);
    }

    property = idToProperty_[QLatin1String("normalColor")];
    if(property != Q_NULLPTR) {
        property->setValue(normalColor_);
    }

    property = idToProperty_[QLatin1String("greaterThanUpperLimitColor")];
    if(property != Q_NULLPTR) {
        property->setValue(greaterThanUpperLimitColor_);
    }

    property = idToProperty_[QLatin1String("upperLimitValue")];
    if(property != Q_NULLPTR) {
        property->setValue(upperLimitValue_);
    }

    property = idToProperty_[QLatin1String("lowerLimitValue")];
    if(property != Q_NULLPTR) {
        property->setValue(lowerLimitValue_);
    }

    property = idToProperty_[QLatin1String("maxValue")];
    if(property != Q_NULLPTR) {
        property->setValue(maxValue_);
    }

    property = idToProperty_[QLatin1String("scaleNum")];
    if(property != Q_NULLPTR) {
        property->setValue(scaleNum_);
    }

    property = idToProperty_[QLatin1String("showOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(showOnInitial_);
    }

    property = idToProperty_[QLatin1String("xCoord")];
    if(property != Q_NULLPTR) {
        property->setValue(elementXPos);
    }

    property = idToProperty_[QLatin1String("yCoord")];
    if(property != Q_NULLPTR) {
        property->setValue(elementYPos);
    }

    property = idToProperty_[QLatin1String("zValue")];
    if(property != Q_NULLPTR) {
        property->setValue(elementZValue);
    }

    property = idToProperty_[QLatin1String("width")];
    if(property != Q_NULLPTR) {
        property->setValue(elementWidth);
    }

    property = idToProperty_[QLatin1String("height")];
    if(property != Q_NULLPTR) {
        property->setValue(elementHeight);
    }
}

void ElementJarShape::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
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

    // 绘制选中状态
    paintSelected(painter, 1);
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


/**
 * @brief ElementJarShape::mouseDoubleClickEvent
 * @details 罐形容器控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementJarShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setSelectedTag(szTagSelected_);
    dlg.setJarShape(jarShape_);
    dlg.setUpperLimitValue(upperLimitValue_);
    dlg.setLowerLimitValue(lowerLimitValue_);
    dlg.setMaxValue(maxValue_);
    dlg.setScaleNum(scaleNum_);
    if(dlg.exec() == QDialog::Accepted) {
        szTagSelected_ = dlg.selectedTag();
        jarShape_ = dlg.jarShape();
        upperLimitValue_ = dlg.upperLimitValue();
        lowerLimitValue_ = dlg.lowerLimitValue();
        maxValue_ = dlg.maxValue();
        scaleNum_ = dlg.scaleNum();

        // 更新属性表
        VariantManager *pVariantManager = dynamic_cast<VariantManager *>(variantPropertyManager_);
        if(pVariantManager != Q_NULLPTR) {
            QtTreePropertyBrowser *pPropertyEditor = pVariantManager->getPropertyEditor();
            if(pPropertyEditor != Q_NULLPTR) {
                pPropertyEditor->clear();
                this->updatePropertyModel();
                QListIterator<QtProperty*> iter(this->getPropertyList());
                while (iter.hasNext()) {
                    pPropertyEditor->addProperty(iter.next());
                }
            }
        }

        scene()->update();
        update();
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
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

