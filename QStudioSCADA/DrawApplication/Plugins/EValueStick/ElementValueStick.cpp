#include "ElementValueStick.h"
#include "TagManager.h"
#include "PubTool.h"
#include <cfloat>
#include <QFontMetrics>
#include <algorithm>

#define SCALE_LENTH         4
#define BIT_SCALE_LENTH     6
#define MIN_BAR_LENTH       5


int ElementValueStick::iLastIndex_ = 1;

ElementValueStick::ElementValueStick(const QString &szProjPath, const QString &szProjName)
    : Element(szProjPath, szProjName)
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
    TagManager::setProjectPath(szProjectPath_);
    init();
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

}


QRectF ElementValueStick::boundingRect() const
{
    qreal extra = 5;
    QRectF QRect(elementRect.toRect());
    return QRect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementValueStick::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
    return path;
}

void ElementValueStick::createPropertyList()
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

    // 刻度最大值
    maxValueProperty_ = new DoubleProperty(tr("最大值"));
    maxValueProperty_->setSettings(DBL_MIN, DBL_MAX, 5);
    maxValueProperty_->setId(EL_SCALE_VALUE_MAX);
    maxValueProperty_->setValue(maxValue_);
    propList.insert(propList.end(), maxValueProperty_);

    // 刻度最小值
    minValueProperty_ = new DoubleProperty(tr("最小值"));
    minValueProperty_->setSettings(DBL_MIN, DBL_MAX, 5);
    minValueProperty_->setId(EL_SCALE_VALUE_MIN);
    minValueProperty_->setValue(minValue_);
    propList.insert(propList.end(), minValueProperty_);

    // 刻度个数
    scaleNumProperty_ = new IntegerProperty(tr("刻度个数"));
    scaleNumProperty_->setSettings(0, 5000);
    scaleNumProperty_->setId(EL_SCALE_NUM);
    scaleNumProperty_->setValue(scaleNum_);
    propList.insert(propList.end(), scaleNumProperty_);

    // 背景颜色
    backgroundColorProperty_ = new ColorProperty(tr("背景颜色"));
    backgroundColorProperty_->setId(EL_BACKGROUND);
    backgroundColorProperty_->setValue(backgroundColor_);
    propList.insert(propList.end(), backgroundColorProperty_);

    // 前景颜色
    foregroundColorProperty_ = new ColorProperty(tr("前景颜色"));
    foregroundColorProperty_->setId(EL_FOREGROUND);
    foregroundColorProperty_->setValue(foregroundColor_);
    propList.insert(propList.end(), foregroundColorProperty_);

    // 标尺颜色
    scaleColorProperty_ = new ColorProperty(tr("标尺颜色"));
    scaleColorProperty_->setId(EL_SCALECOLOR);
    scaleColorProperty_->setValue(scaleColor_);
    propList.insert(propList.end(), scaleColorProperty_);

    // 标尺方向
    scaleDirProperty_ = new ListProperty(tr("标尺方向"));
    scaleDirProperty_->setId(EL_SCALE_DIR);
    QStringList scaleDirList;
    scaleDirList << tr("从左到右") << tr("从右到左") << tr("从上到下") << tr("从下到上");
    scaleDirProperty_->setList(scaleDirList);
    scaleDirProperty_->setValue(scaleDir_);
    propList.insert(propList.end(), scaleDirProperty_);

    // 标尺位置
    scalePosProperty_ = new ListProperty(tr("标尺位置"));
    scalePosProperty_->setId(EL_SCALE_POS);
    QStringList scalePosList;
    scalePosList << tr("右下方") << tr("左上方");
    scalePosProperty_->setList(scalePosList);
    scalePosProperty_->setValue(scalePos_);
    propList.insert(propList.end(), scalePosProperty_);

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

    // 显示标尺
    showRulerProperty_ = new BoolProperty(tr("显示标尺"));
    showRulerProperty_->setId(EL_SHOW_RULER);
    showRulerProperty_->setTrueText(tr("是"));
    showRulerProperty_->setFalseText(tr("否"));
    showRulerProperty_->setValue(showRuler_);
    propList.insert(propList.end(), showRulerProperty_);


    // 显示刻度
    showScaleProperty_ = new BoolProperty(tr("显示刻度"));
    showScaleProperty_->setId(EL_SHOW_SCALE);
    showScaleProperty_->setTrueText(tr("是"));
    showScaleProperty_->setFalseText(tr("否"));
    showScaleProperty_->setValue(showScale_);
    propList.insert(propList.end(), showScaleProperty_);

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

void ElementValueStick::updateElementProperty(uint id, const QVariant &value)
{
    switch (id) {
    case EL_ID:
        elementId = value.toString();
        break;
	case EL_TAG:
		szTagSelected_ = value.toString();
		break;
    case EL_SCALE_VALUE_MAX:
        maxValue_ = value.toDouble();
        break;
    case EL_SCALE_VALUE_MIN:
        minValue_ = value.toDouble();
        break;
    case EL_SCALE_NUM:
        scaleNum_ = value.toInt();
        break;
    case EL_BACKGROUND:
        backgroundColor_ = value.value<QColor>();
        break;
    case EL_FOREGROUND:
        foregroundColor_ = value.value<QColor>();
        break;
    case EL_SCALECOLOR:
        scaleColor_ = value.value<QColor>();
        break;
    case EL_SCALE_DIR:
        scaleDir_ = value.toString();
        break;
    case EL_SCALE_POS:
        scalePos_ = value.toString();
        break;
    case EL_FONT:
        font_ = value.value<QFont>();
        break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_SHOW_RULER:
        showRuler_ = value.toBool();
        break;
    case EL_SHOW_SCALE:
        showScale_ = value.toBool();
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

void ElementValueStick::updatePropertyModel()
{
    idProperty_->setValue(elementId);
	tagSelectProperty_->setValue(szTagSelected_);
    maxValueProperty_->setValue(maxValue_);
    minValueProperty_->setValue(minValue_);
    scaleNumProperty_->setValue(scaleNum_);
    backgroundColorProperty_->setValue(backgroundColor_);
    foregroundColorProperty_->setValue(foregroundColor_);
    scaleColorProperty_->setValue(scaleColor_);
    scaleDirProperty_->setValue(scaleDir_);
    scalePosProperty_->setValue(scalePos_);
    fontProperty_->setValue(font_);
    textColorProperty_->setValue(textColor);
    showRulerProperty_->setValue(showRuler_);
    showScaleProperty_->setValue(showScale_);
    showOnInitialProperty_->setValue(showOnInitial_);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
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

void ElementValueStick::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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


void ElementValueStick::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void ElementValueStick::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void ElementValueStick::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

