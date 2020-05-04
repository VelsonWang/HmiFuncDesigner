#include "ElementValueStick.h"
#include "ProjectData.h"
#include "PubTool.h"
#include "variantmanager.h"
#include <cfloat>
#include <QFontMetrics>
#include <algorithm>
#include "editbasicpropertydialog.h"

#define SCALE_LENTH         4
#define BIT_SCALE_LENTH     6
#define MIN_BAR_LENTH       5


int ElementValueStick::iLastIndex_ = 1;

ElementValueStick::ElementValueStick(const QString &szProjPath,
                                     const QString &szProjName,
                                     QtVariantPropertyManager *propertyMgr)
    : Element(szProjPath, szProjName, propertyMgr)
{
    elementId = QString(tr("ValueStick_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("ValueStick");
    elementIcon = QIcon(":/images/ValueStick.png");
    scaleDir_ = tr("从左到右");
    scalePos_ = tr("左上方");
    font_ = QFont("Arial Black", 12);
    backgroundColor_ = QColor(0xf0, 0xf0, 0xf0);
    foregroundColor_ = QColor(0xff, 0x00, 0x00);
    scaleColor_ = QColor(0x00, 0x00, 0xff);
    showRuler_ = true;
    showScale_ = true;
    showOnInitial_ = true;
    scaleNum_ = 10;
    maxValue_ = 100;
    minValue_ = 0;
    init();
    if(ProjectData::getInstance()->getDBPath() == "")
        ProjectData::getInstance()->createOrOpenProjectData(szProjectPath_, szProjectName_);
    createPropertyList();
    updatePropertyModel();
}

void ElementValueStick::regenerateElementId()
{
    elementId = QString(tr("ValueStick_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementValueStick::release
 * @details 释放占用的资源
 */
void ElementValueStick::release()
{
    ProjectData::releaseInstance();
}


QRectF ElementValueStick::boundingRect() const
{
    qreal extra = 5;
    QRectF QRect(elementRect.toRect());
    return QRect.normalized().adjusted(-extra,-extra,extra,extra);
}


void ElementValueStick::createPropertyList()
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

    // 刻度最大值
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("最大值"));
    property->setAttribute(QLatin1String("minimum"), DBL_MIN);
    property->setAttribute(QLatin1String("maximum"), DBL_MAX);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("maxValue"));

    // 刻度最小值
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("最小值"));
    property->setAttribute(QLatin1String("minimum"), DBL_MIN);
    property->setAttribute(QLatin1String("maximum"), DBL_MAX);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("minValue"));

    // 刻度个数
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("刻度个数"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("scaleNum"));

    // 背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("背景颜色"));
    addProperty(property, QLatin1String("backgroundColor"));

    // 前景颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("前景颜色"));
    addProperty(property, QLatin1String("foregroundColor"));

    // 标尺颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("标尺颜色"));
    addProperty(property, QLatin1String("scaleColor"));

    // 标尺方向
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("标尺方向"));
    scaleDirList_.clear();
    scaleDirList_ << tr("从左到右") << tr("从右到左") << tr("从上到下") << tr("从下到上");
    property->setAttribute(QLatin1String("enumNames"), scaleDirList_);
    addProperty(property, QLatin1String("scaleDir"));

    // 标尺位置
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("标尺位置"));
    scalePosList_.clear();
    scalePosList_ << tr("右下方") << tr("左上方");
    property->setAttribute(QLatin1String("enumNames"), scalePosList_);
    addProperty(property, QLatin1String("scalePos"));

    // 字体
    property = variantPropertyManager_->addProperty(QVariant::Font, tr("字体"));
    addProperty(property, QLatin1String("font"));

    // 文本颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("文本颜色"));
    addProperty(property, QLatin1String("textColor"));

    // 显示标尺
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("显示标尺"));
    addProperty(property, QLatin1String("showRuler"));

    // 显示刻度
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("显示刻度"));
    addProperty(property, QLatin1String("showScale"));

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

void ElementValueStick::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("maxValue")) {
        maxValue_ = value.toDouble();
    } else if (id == QLatin1String("minValue")) {
        minValue_ = value.toDouble();
    } else if (id == QLatin1String("scaleNum")) {
        scaleNum_ = value.toInt();
    } else if (id == QLatin1String("backgroundColor")) {
        backgroundColor_ = value.value<QColor>();
    } else if (id == QLatin1String("foregroundColor")) {
        foregroundColor_ = value.value<QColor>();
    } else if (id == QLatin1String("scaleColor")) {
        scaleColor_ = value.value<QColor>();
    } else if (id == QLatin1String("scaleDir")) {
        scaleDir_ = scaleDirList_.at(value.toInt());
    } else if (id == QLatin1String("scalePos")) {
        scalePos_ = scalePosList_.at(value.toInt());
    } else if (id == QLatin1String("font")) {
        font_ = value.value<QFont>();
    } else if (id == QLatin1String("textColor")) {
        textColor = value.value<QColor>();
    } else if (id == QLatin1String("showRuler")) {
        showRuler_ = value.toBool();
    }  else if (id == QLatin1String("showScale")) {
        showScale_ = value.toBool();
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

void ElementValueStick::updatePropertyModel()
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

    property = idToProperty_[QLatin1String("maxValue")];
    if(property != Q_NULLPTR) {
        property->setValue(maxValue_);
    }

    property = idToProperty_[QLatin1String("minValue")];
    if(property != Q_NULLPTR) {
        property->setValue(minValue_);
    }

    property = idToProperty_[QLatin1String("scaleNum")];
    if(property != Q_NULLPTR) {
        property->setValue(scaleNum_);
    }

    property = idToProperty_[QLatin1String("backgroundColor")];
    if(property != Q_NULLPTR) {
        property->setValue(backgroundColor_);
    }

    property = idToProperty_[QLatin1String("foregroundColor")];
    if(property != Q_NULLPTR) {
        property->setValue(foregroundColor_);
    }

    property = idToProperty_[QLatin1String("scaleColor")];
    if(property != Q_NULLPTR) {
        property->setValue(scaleColor_);
    }

    property = idToProperty_[QLatin1String("scaleDir")];
    if(property != Q_NULLPTR) {
        property->setValue(scaleDirList_.indexOf(scaleDir_));
    }

    property = idToProperty_[QLatin1String("scalePos")];
    if(property != Q_NULLPTR) {
        property->setValue(scalePosList_.indexOf(scalePos_));
    }

    property = idToProperty_[QLatin1String("font")];
    if(property != Q_NULLPTR) {
        property->setValue(font_);
    }

    property = idToProperty_[QLatin1String("textColor")];
    if(property != Q_NULLPTR) {
        property->setValue(textColor);
    }

    property = idToProperty_[QLatin1String("showRuler")];
    if(property != Q_NULLPTR) {
        property->setValue(showRuler_);
    }

    property = idToProperty_[QLatin1String("showScale")];
    if(property != Q_NULLPTR) {
        property->setValue(showScale_);
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

void ElementValueStick::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementValueStick::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementValueStick::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 绘制数值棒图
    drawValueStick(painter);

    // 绘制选中状态
    paintSelected(painter, 1);
}


void ElementValueStick::drawScalarStick(QPainter *painter,
                                        QRect rect,
                                        QRect textRect,
                                        QRect scaleRect,
                                        double dMinValue,
                                        double dMaxValue,
                                        int iScaleNum,
                                        QColor fillColor,
                                        bool scaleVisible,
                                        QColor scaleColor,
                                        QString scaleDirect,
                                        QString scalePosition)
{

    if(!scaleVisible)
        return;

    QFontMetrics fm(font_);
    int iTextHeight = fm.height();
    QString szMinValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", dMinValue));
    QString szMaxValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", dMaxValue));

    int iMinValueTextWidth = fm.boundingRect(szMinValue).width();
    int iMaxValueTextWidth = fm.boundingRect(szMaxValue).width();

    int iContHeight = rect.height();
    int iContWidth = rect.width();

    int iFlags = 0;

    // 绘制标值
    QRect leftRect, rightRect, upRect, downRect;
    if(textRect.top() != textRect.bottom()) {
        QRect aRect;
        aRect = textRect;
        if ( scaleDirect == QString("LeftToRight") ) {
            aRect.setRight(aRect.left() + iMinValueTextWidth - 1);
            iFlags = Qt::AlignLeft | Qt::AlignVCenter;
            painter->drawText(aRect, iFlags, szMinValue);
            leftRect = aRect;
            aRect.setRight(textRect.right());
            aRect.setLeft(aRect.right() - iMaxValueTextWidth + 1);
            iFlags = Qt::AlignRight | Qt::AlignVCenter;
            painter->drawText(aRect, iFlags, szMaxValue);
            rightRect = aRect;
        } else if ( scaleDirect == QString("RightToLeft")) {
            aRect.setRight(aRect.left() + iMaxValueTextWidth - 1);
            iFlags = Qt::AlignLeft | Qt::AlignVCenter;
            painter->drawText(aRect, iFlags, szMaxValue);
            leftRect = aRect;
            aRect.setRight(textRect.right());
            aRect.setLeft(aRect.right() - iMinValueTextWidth + 1);
            iFlags = Qt::AlignRight | Qt::AlignVCenter;
            painter->drawText(aRect, iFlags, szMinValue);
            rightRect = aRect;
        } else if ( scaleDirect == QString("BottomToTop") ) {
            iFlags = Qt::AlignLeft | Qt::AlignVCenter;
            if(scalePosition == QString("LeftTop"))
                iFlags = Qt::AlignRight | Qt::AlignVCenter;
            aRect.setBottom(aRect.top() + iTextHeight - 1);
            painter->drawText(aRect, iFlags, szMaxValue);
            upRect = aRect;
            aRect.setBottom(textRect.bottom());
            aRect.setTop(aRect.bottom() - iTextHeight + 1);
            painter->drawText(aRect, iFlags, szMinValue);
            downRect = aRect;
        } else if ( scaleDirect ==  QString("TopToBottom") ) {
            iFlags = Qt::AlignLeft | Qt::AlignVCenter;
            if(scalePosition == QString("LeftTop"))
                iFlags = Qt::AlignRight | Qt::AlignVCenter;
            aRect.setBottom(aRect.top() + iTextHeight - 1);
            painter->drawText(aRect, iFlags, szMinValue);
            upRect = aRect;
            aRect.setBottom(textRect.bottom());
            aRect.setTop(aRect.bottom() - iTextHeight + 1);
            painter->drawText(aRect, iFlags, szMaxValue);
            downRect = aRect;
        }

    }

    // 绘制刻度
    iFlags = Qt::AlignCenter | Qt::AlignVCenter;
    bool bContiueDrawValueText = true;
    bool bRealDrawValueText = false;

    if ( scaleRect.top() != scaleRect.bottom() ) {
        float fContWidth = static_cast<float>(iContWidth);
        float fContHeight = static_cast<float>(iContHeight);
        float fScaleNum = static_cast<float>(iScaleNum);
        float fScaleW = fContWidth / fScaleNum;
        float fScaleH = fContHeight / fScaleNum;
        int xPos = 0, yPos = 0;
        for(int k = 0; k <= iScaleNum; k++) {
            float fK = static_cast<float>(k);
            if ( scaleDirect == QString("LeftToRight") ) {
                xPos = scaleRect.right();
                if(k < iScaleNum) {
                    xPos = scaleRect.left() + static_cast<int>(fK * fScaleW);
                }
            } else if ( scaleDirect ==  QString("RightToLeft") ) {
                xPos = scaleRect.left();
                if(k < iScaleNum) {
                    xPos = scaleRect.right() - static_cast<int>(fK * fScaleW);
                }
            } else if ( scaleDirect ==  QString("BottomToTop") ) {
                yPos = scaleRect.top();
                if(k < iScaleNum) {
                    yPos = scaleRect.bottom() - static_cast<int>(fK * fScaleH);
                }
            } else if ( scaleDirect ==  QString("TopToBottom") ) {
                yPos = scaleRect.bottom();
                if(k < iScaleNum) {
                    yPos = scaleRect.top() + static_cast<int>(fK * fScaleH);
                }
            }

            //绘制中间粗略刻度值
            if ( textRect.top() != textRect.bottom() && k > 0 && k < iScaleNum && bContiueDrawValueText ) {
                QRect aRect = textRect;
                double dbValue = dMinValue + k * (double)(dMaxValue - dMinValue) / (double)iScaleNum;

                QString szMaxValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", dbValue));
                iMaxValueTextWidth = fm.boundingRect(szMaxValue).width();

                if ( scaleDirect == QString("LeftToRight") || scaleDirect ==  QString("RightToLeft") ) {
                    aRect.setLeft(xPos - iMaxValueTextWidth / 2);
                    aRect.setRight(xPos + iMaxValueTextWidth / 2);

                    if ( aRect.left() <= leftRect.right() || aRect.right() >= rightRect.left() ) {
                        bContiueDrawValueText = false;
                        continue;
                    }
                } else {
                    aRect.setLeft(aRect.left() + 2);
                    aRect.setTop(yPos - iTextHeight / 2);
                    aRect.setBottom(yPos + iTextHeight / 2);
                    iFlags = Qt::AlignLeft | Qt::AlignVCenter;
                    if(scalePosition == QString("LeftTop")) {
                        iFlags = Qt::AlignRight | Qt::AlignVCenter;
                    }

                    if ( aRect.top() <= upRect.bottom() || aRect.bottom() >= downRect.top() ) {
                        bContiueDrawValueText = false;
                        continue;
                    }
                }

                QRect aRectTemp= aRect;
                aRectTemp.setRight(aRectTemp.right() + 2);
                painter->drawText(aRectTemp, iFlags, szMaxValue);
                bRealDrawValueText = true;
            }

            if ( scalePosition == QString("LeftTop") ) {
                QPen pen = QPen(QBrush(scaleColor), 1, Qt::SolidLine);
                painter->setPen(pen);
                if ( scaleDirect == QString("LeftToRight") || scaleDirect == QString("RightToLeft") ) {
                    painter->drawLine(xPos, scaleRect.bottom(), xPos, scaleRect.bottom() - SCALE_LENTH + 1);
                } else {
                    painter->drawLine(scaleRect.right(), yPos, scaleRect.right() - SCALE_LENTH + 1, yPos);
                }
            } else {
                if(scaleDirect == QString("LeftToRight") || scaleDirect == QString("RightToLeft")) {
                    painter->drawLine(xPos, scaleRect.top(), xPos, scaleRect.top() + SCALE_LENTH - 1);
                } else {
                    painter->drawLine(scaleRect.left(), yPos, scaleRect.left() + SCALE_LENTH - 1, yPos);
                }
            }

        }

        // 清除残缺不齐的中间刻度显示值
        if ( !bContiueDrawValueText && bRealDrawValueText ) {
            QRect aRect;
            memcpy(&aRect, &textRect, sizeof(QRect));
            if((scaleDirect == QString("LeftToRight")) || (scaleDirect == QString("RightToLeft"))) {
                aRect.setLeft(leftRect.right() + 1);
                aRect.setRight(rightRect.left() - 1);
            } else {
                aRect.setTop(upRect.bottom() + 1);
                aRect.setBottom(downRect.top() - 1);
                if(aRect.bottom() >= aRect.top())
                    PubTool::FillColorRect(painter, aRect, fillColor);
            }
        }
    }
}


void ElementValueStick::drawValueStick(QPainter *painter)
{
    QColor color3DShadow = QColor(0x0F, 0x0F, 0x0F);
    QColor color3DHiLight = QColor(0xF0, 0xF0, 0xF0);

    // 绘制边框
    painter->setPen(QPen(QColor(61, 123, 173), 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(elementRect);

    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    QRect barRect, scalRect, textRect;
    QFontMetrics fm(font_);
    int iTextHeight = fm.height();
    QString szMinValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", minValue_));
    QString szMaxValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", maxValue_));

    int iMinValueTextWidth = fm.boundingRect(szMinValue).width();
    int iMaxValueTextWidth = fm.boundingRect(szMaxValue).width();

    barRect = elementRect.toRect();
    scalRect = elementRect.toRect();
    textRect = elementRect.toRect();

    int iContHeight = static_cast<int>(elementRect.height());
    int iContWidth = static_cast<int>(elementRect.width());

    QBrush brush(backgroundColor_);
    painter->fillRect(elementRect, brush);

    QString szScaleDir = getDirString(scaleDir_);
    QString szScalePos = getPosString(scalePos_);

    if ( szScaleDir == QString("LeftToRight") || szScaleDir == QString("RightToLeft") ) {
        /////////////////////////////绘制水平的标尺///////////////////////////////
        bool bTextVisible = (iContWidth > (iMinValueTextWidth + iMaxValueTextWidth));

        // 计算矩形区域(水平显示)
        if ( showScale_ && (showRuler_ && bTextVisible) ) {
            // 标尺和文本都可见
            if ( iContHeight >= (iTextHeight + BIT_SCALE_LENTH + MIN_BAR_LENTH) ) {
                if(szScalePos == QString("LeftTop")) {
                    textRect.setBottom(textRect.top() + iTextHeight - 1);
                    scalRect.setTop(textRect.bottom() + 1);
                    scalRect.setBottom(scalRect.top() + BIT_SCALE_LENTH - 1);
                    barRect.setTop(scalRect.bottom() + 1);
                } else {
                    textRect.setTop(textRect.bottom() - iTextHeight + 1);
                    scalRect.setBottom(textRect.top() - 1);
                    scalRect.setTop(scalRect.bottom() - BIT_SCALE_LENTH + 1);
                    barRect.setBottom(scalRect.top() - 1);
                }
            } else if ( iContHeight >= (BIT_SCALE_LENTH + MIN_BAR_LENTH) ) {
                textRect.setTop(textRect.bottom());
                if(szScalePos == QString("LeftTop")) {
                    scalRect.setBottom(scalRect.top() + BIT_SCALE_LENTH - 1);
                    barRect.setTop(scalRect.bottom() + 1);
                } else {
                    scalRect.setTop(scalRect.bottom() - BIT_SCALE_LENTH + 1);
                    barRect.setBottom(scalRect.top() - 1);
                }
            } else {
                textRect.setTop(textRect.bottom());
                scalRect.setTop(scalRect.bottom());
            }
        } else if(showScale_) {
            // 只标尺可见
            textRect.setTop(textRect.bottom());
            if ( iContHeight >= (BIT_SCALE_LENTH + MIN_BAR_LENTH) )
            {
                if(szScalePos == QString("LeftTop")) {
                    scalRect.setBottom(scalRect.top() + BIT_SCALE_LENTH - 1);
                    barRect.setTop(scalRect.bottom() + 1);
                } else {
                    scalRect.setTop(scalRect.bottom() - BIT_SCALE_LENTH + 1);
                    barRect.setBottom(scalRect.top() - 1);
                }
            } else {
                scalRect.setTop(scalRect.bottom());
            }
        } else {
            textRect.setTop(textRect.bottom());
            scalRect.setTop(scalRect.bottom());
        }

        ////////////////////////////////////////////////////////////////////////////////
        // 绘制各个矩形(水平显示)
        // 绘制棒条
        PubTool::Draw3DFrame(painter, barRect, color3DShadow, color3DHiLight, backgroundColor_);
        if ( szScaleDir == QString("LeftToRight") ) {
            barRect.setRight(barRect.left() + (barRect.right() - barRect.left()) / 2);
        } else {
            barRect.setLeft(barRect.right() - (barRect.right() - barRect.left()) / 2);
        }

        barRect = PubTool::DeflateRect(barRect, 1);
        PubTool::FillColorRect(painter, barRect, foregroundColor_);

        // 绘制标尺
        drawScalarStick(painter, elementRect.toRect(), textRect, scalRect, minValue_, maxValue_,
                        scaleNum_, backgroundColor_, showScale_,
                        scaleColor_, szScaleDir, szScalePos);

    } else {
        /////////////////////////////绘制垂直的标尺//////////////////////////////
        bool bTextVisible = (iContHeight > (2 * iTextHeight));

        // 计算矩形区域(垂直显示)
        if ( showScale_ && (showRuler_ && bTextVisible) ) {
            // 标尺和文本都可见
            int iTextRectWidth = std::max(iMinValueTextWidth, iMaxValueTextWidth);
            // 获取粗略刻度文本长度
            int iTempMaxWidthLen = iTextRectWidth;

            if ( iContWidth >= (iTempMaxWidthLen - 2 + BIT_SCALE_LENTH + MIN_BAR_LENTH) ) {
                iTextRectWidth = iTempMaxWidthLen - 2;
            }

            if ( iContWidth >= (iTextRectWidth + BIT_SCALE_LENTH + MIN_BAR_LENTH) ) {
                if( szScalePos == QString("LeftTop") ) {
                    textRect.setRight(textRect.left() + iTextRectWidth - 1);
                    scalRect.setLeft(textRect.right() + 1);
                    scalRect.setRight(scalRect.left() + BIT_SCALE_LENTH - 1);
                    barRect.setLeft(scalRect.right() + 1);
                } else {
                    textRect.setLeft(textRect.right() - iTextRectWidth + 1);
                    scalRect.setRight(textRect.left() - 1);
                    scalRect.setLeft(scalRect.right() - BIT_SCALE_LENTH + 1);
                    barRect.setRight(scalRect.left() - 1);
                }
            } else if ( iContWidth >= (BIT_SCALE_LENTH + MIN_BAR_LENTH) ) {
                textRect.setLeft(textRect.right());
                if(szScalePos == QString("LeftTop")) {
                    scalRect.setRight(scalRect.left() + BIT_SCALE_LENTH - 1);
                    barRect.setLeft(scalRect.right() + 1);
                } else {
                    scalRect.setLeft(scalRect.right() - BIT_SCALE_LENTH + 1);
                    barRect.setRight(scalRect.left() - 1);
                }
            } else {
                textRect.setLeft(textRect.right());
                scalRect.setLeft(scalRect.right());
            }
        } else if ( showScale_ ) {
            // 只标尺可见
            textRect.setLeft(textRect.right());
            if(iContHeight >= (BIT_SCALE_LENTH + MIN_BAR_LENTH)) {
                if(szScalePos == QString("LeftTop")) {
                    scalRect.setRight(scalRect.left() + BIT_SCALE_LENTH - 1);
                    barRect.setLeft(scalRect.right() + 1);
                } else {
                    scalRect.setLeft(scalRect.right() - BIT_SCALE_LENTH + 1);
                    barRect.setRight(scalRect.left() - 1);
                }
            } else {
                scalRect.setLeft(scalRect.right());
            }
        } else {
            textRect.setLeft(textRect.right());
            scalRect.setLeft(scalRect.right());
        }

        // 绘制各个矩形(垂直显示)
        // 绘制棒条
        PubTool::Draw3DFrame(painter, barRect, color3DShadow, color3DHiLight, backgroundColor_);
        if ( szScaleDir == QString("TopToBottom") ) {
            barRect.setBottom(barRect.top() + (barRect.bottom() - barRect.top()) / 2);
        } else {
            barRect.setTop(barRect.bottom() - (barRect.bottom() - barRect.top()) / 2);
        }

        barRect = PubTool::DeflateRect(barRect, 1);
        PubTool::FillColorRect(painter, barRect, foregroundColor_);

        //绘制标尺
        drawScalarStick(painter, elementRect.toRect(), textRect, scalRect, minValue_, maxValue_,
                        scaleNum_, backgroundColor_, showScale_,
                        scaleColor_, szScaleDir, szScalePos);

    }
}


/**
 * @brief ElementValueStick::mouseDoubleClickEvent
 * @details 数值棒图控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementValueStick::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setSelectedTag(szTagSelected_);
    dlg.setMaxValue(maxValue_);
    dlg.setMinValue(minValue_);
    dlg.setScaleNum(scaleNum_);
    dlg.setBackgroundColor(backgroundColor_);
    dlg.setForegroundColor(foregroundColor_);
    dlg.setScaleColor(scaleColor_);
    dlg.setScaleDir(scaleDir_);
    dlg.setScalePos(scalePos_);
    dlg.setTextColor(textColor);
    if(dlg.exec() == QDialog::Accepted) {
        szTagSelected_ = dlg.selectedTag();
        maxValue_ = dlg.maxValue();
        minValue_ = dlg.minValue();
        scaleNum_ = dlg.scaleNum();
        backgroundColor_ = dlg.backgroundColor();
        foregroundColor_ = dlg.foregroundColor();
        scaleColor_ = dlg.scaleColor();
        scaleDir_ = dlg.scaleDir();
        scalePos_ = dlg.scalePos();
        textColor = dlg.textColor();

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


/**
 * @brief ElementValueStick::getDirString
 * @param szDir 标尺方向
 * @return 标尺方向描述
 */
QString ElementValueStick::getDirString(const QString& szDir) const
{
    if(szDir == tr("从左到右")) {
        return QString("LeftToRight");
    } else if(szDir == tr("从右到左")) {
        return QString("RightToLeft");
    } else if(szDir == tr("从上到下")) {
        return QString("TopToBottom");
    } else if(szDir == tr("从下到上")) {
        return QString("BottomToTop");
    }
    return QString("");
}


/**
 * @brief ElementValueStick::setDirString
 * @details 设置标尺方向
 * @param szDir 标尺方向
 * @param szDirSet 待设置标尺方向
 */
void ElementValueStick::setDirString(const QString& szDir, QString& szDirSet)
{
    if(szDir == QString("LeftToRight")) {
        szDirSet = tr("从左到右");
    } else if(szDir == QString("RightToLeft")) {
        szDirSet = tr("从右到左");
    } else if(szDir == QString("TopToBottom")) {
        szDirSet = tr("从上到下");
    } else if(szDir == QString("BottomToTop")) {
        szDirSet = tr("从下到上");
    }
}

/**
 * @brief ElementValueStick::getPosString
 * @param szDir 标尺位置
 * @return 标尺位置描述
 */
QString ElementValueStick::getPosString(const QString& szPos) const
{
    if(szPos == tr("右下方")) {
        return QString("RightBottom");
    } else if(szPos == tr("左上方")) {
        return QString("LeftTop");
    }
    return QString("");
}


/**
 * @brief ElementValueStick::setPosString
 * @details 设置标尺位置
 * @param szDir 标尺位置
 * @param szDirSet 待设标尺位置
 */
void ElementValueStick::setPosString(const QString& szPos, QString& szPosSet)
{
    if(szPos == QString("RightBottom")) {
        szPosSet = tr("右下方");
    } else if(szPos == QString("LeftTop")) {
        szPosSet = tr("左上方");
    }
}


void ElementValueStick::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
	writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("maxValue", QString::number(maxValue_));
    writer.writeAttribute("minValue", QString::number(minValue_));
    writer.writeAttribute("scaleNum", QString::number(scaleNum_));
    writer.writeAttribute("backgroundColor", backgroundColor_.name());
    writer.writeAttribute("foregroundColor", foregroundColor_.name());
    writer.writeAttribute("scaleColor", scaleColor_.name());
    writer.writeAttribute("scaleDir", getDirString(scaleDir_));
    writer.writeAttribute("scalePos", getPosString(scalePos_));
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("showRuler", showRuler_?"true":"false");
    writer.writeAttribute("showScale", showScale_?"true":"false");
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeEndElement();
}

void ElementValueStick::readFromXml(const QXmlStreamAttributes &attributes)
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

    if (attributes.hasAttribute("maxValue")) {
        maxValue_ = attributes.value("maxValue").toDouble();
    }

    if (attributes.hasAttribute("minValue")) {
        minValue_ = attributes.value("minValue").toDouble();
    }

    if (attributes.hasAttribute("scaleNum")) {
        scaleNum_ = attributes.value("scaleNum").toInt();
    }

    if (attributes.hasAttribute("backgroundColor")) {
        backgroundColor_ = QColor(attributes.value("backgroundColor").toString());
    }

    if (attributes.hasAttribute("foregroundColor")) {
        foregroundColor_ = QColor(attributes.value("foregroundColor").toString());
    }

    if (attributes.hasAttribute("scaleColor")) {
        scaleColor_ = QColor(attributes.value("scaleColor").toString());
    }

    if (attributes.hasAttribute("scaleDir")) {
        QString szDir = attributes.value("scaleDir").toString();
        this->setDirString(szDir, scaleDir_);
    }

    if (attributes.hasAttribute("scalePos")) {
        QString szPos = attributes.value("scalePos").toString();
        this->setPosString(szPos, scalePos_);
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("showRuler")) {
        QString value = attributes.value("showRuler").toString();
        showRuler_ = false;
        if(value == "true") {
            showRuler_ = true;
        }
    }

    if (attributes.hasAttribute("showScale")) {
        QString value = attributes.value("showScale").toString();
        showScale_ = false;
        if(value == "true") {
            showScale_ = true;
        }
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

void ElementValueStick::writeData(QDataStream &out)
{
    out << this->elementId
		<< this->szTagSelected_
        << this->maxValue_
        << this->minValue_
        << this->scaleNum_
        << this->backgroundColor_
        << this->foregroundColor_
        << this->scaleColor_
        << this->getDirString(scaleDir_)
        << this->getPosString(scalePos_)
        << this->font_.toString()
        << this->textColor
        << this->showRuler_
        << this->showScale_
        << this->showOnInitial_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight;
}

void ElementValueStick::readData(QDataStream &in)
{
    QString id;
	QString szTagSelected;
    double maxValue;
    double minValue;
    int scaleNum;
    QColor backgroundColor;
    QColor foregroundColor;
    QColor scaleColor;
    QString scaleDir;
    QString scalePos;
    QString font;
    QColor textColor;
    bool showRuler;
    bool showScale;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
	   >> szTagSelected
       >> maxValue
       >> minValue
       >> scaleNum
       >> backgroundColor
       >> foregroundColor
       >> scaleColor
       >> scaleDir
       >> scalePos
       >> font
       >> textColor
       >> showRuler
       >> showScale
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
    this->maxValue_ = maxValue;
    this->minValue_ = minValue;
    this->scaleNum_ = scaleNum;
    this->backgroundColor_ = backgroundColor;
    this->foregroundColor_ = foregroundColor;
    this->scaleColor_ = scaleColor;
    this->setDirString(scaleDir, scaleDir_);
    this->setPosString(scalePos, scalePos_);
    this->font_ = font;
    this->textColor = textColor;
    this->showRuler_ = showRuler;
    this->showScale_ = showScale;
    this->showOnInitial_ = showOnInitial;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->updateBoundingElement();
    this->updatePropertyModel();
}

QDataStream &operator<<(QDataStream &out,const ElementValueStick &ele)
{
    out << ele.elementId
        << ele.szTagSelected_
        << ele.maxValue_
        << ele.minValue_
        << ele.scaleNum_
        << ele.backgroundColor_
        << ele.foregroundColor_
        << ele.scaleColor_
        << ele.getDirString(ele.scaleDir_)
        << ele.getPosString(ele.scalePos_)
        << ele.font_
        << ele.textColor
        << ele.showRuler_
        << ele.showScale_
        << ele.showOnInitial_
        << ele.x()
        << ele.y()
        << ele.zValue()
        << ele.elementWidth
        << ele.elementHeight;
    return out;
}

QDataStream &operator>>(QDataStream &in, ElementValueStick &ele)
{
    QString id;
	QString szTagSelected;
    double maxValue;
    double minValue;
    int scaleNum;
    QColor backgroundColor;
    QColor foregroundColor;
    QColor scaleColor;
    QString scaleDir;
    QString scalePos;
    QString font;
    QColor textColor;
    bool showRuler;
    bool showScale;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
	   >> szTagSelected
       >> maxValue
       >> minValue
       >> scaleNum
       >> backgroundColor
       >> foregroundColor
       >> scaleColor
       >> scaleDir
       >> scalePos
       >> font
       >> textColor
       >> showRuler
       >> showScale
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
    ele.maxValue_ = maxValue;
    ele.minValue_ = minValue;
    ele.scaleNum_ = scaleNum;
    ele.backgroundColor_ = backgroundColor;
    ele.foregroundColor_ = foregroundColor;
    ele.scaleColor_ = scaleColor;
    ele.setDirString(scaleDir, ele.scaleDir_);
    ele.setPosString(scalePos, ele.scalePos_);
    ele.font_ = font;
    ele.textColor = textColor;
    ele.showRuler_ = showRuler;
    ele.showScale_ = showScale;
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

