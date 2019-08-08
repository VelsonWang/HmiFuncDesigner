#include "ElementValueStick.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "SCADARunTime.h"
#include <QDateTime>
#include <QDate>
#include <QFontMetrics>
#include <cfloat>
#include <algorithm>
#include <QDebug>



#define SCALE_LENTH         4
#define BIT_SCALE_LENTH     6
#define MIN_BAR_LENTH       5


ElementValueStick::ElementValueStick()
    : Element()
{
    elementId = tr("ValueStick");
    internalElementType = tr("ValueStick");
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
}

QRectF ElementValueStick::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementValueStick::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementValueStick::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementValueStick::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementValueStick::paint(QPainter *painter)
{
    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    // 绘制数值棒图
    drawValueStick(painter);

    painter->restore();
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

    QColor color3DShadow = QColor(0x0F, 0x0F, 0x0F);
    QColor color3DHiLight = QColor(0xF0, 0xF0, 0xF0);

    // 绘制边框
    painter->setPen(QPen(QColor(61, 123, 173), 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(elementRect_);

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

    barRect = elementRect_.toRect();
    scalRect = elementRect_.toRect();
    textRect = elementRect_.toRect();

    int iContHeight = static_cast<int>(elementRect_.height());
    int iContWidth = static_cast<int>(elementRect_.width());

    QBrush brush(backgroundColor_);
    painter->fillRect(elementRect_, brush);

    QString szScaleDir = scaleDir_;
    QString szScalePos = scalePos_;

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
        int iBarLength = barRect.right() - barRect.left();
        int iTagVal = static_cast<int>(dTagValue * iBarLength / (maxValue_ - minValue_));
        int iBarVal = (iTagVal > iBarLength) ? iBarLength : iTagVal;
        if ( szScaleDir == QString("LeftToRight") ) {
            barRect.setRight(barRect.left() + iBarVal);
        } else {
            barRect.setLeft(barRect.right() - iBarVal);
        }

        barRect = PubTool::DeflateRect(barRect, 1);
        PubTool::FillColorRect(painter, barRect, foregroundColor_);

        // 绘制标尺
        drawScalarStick(painter, elementRect_.toRect(), textRect, scalRect, minValue_, maxValue_,
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
        int iBarLength = barRect.bottom() - barRect.top();
        int iTagVal = static_cast<int>(dTagValue * iBarLength / (maxValue_ - minValue_));
        int iBarVal = (iTagVal > iBarLength) ? iBarLength : iTagVal;
        if ( szScaleDir == QString("TopToBottom") ) {
            barRect.setBottom(barRect.top() + iBarVal);
        } else {
            barRect.setTop(barRect.bottom() - iBarVal);
        }

        barRect = PubTool::DeflateRect(barRect, 1);
        PubTool::FillColorRect(painter, barRect, foregroundColor_);

        //绘制标尺
        drawScalarStick(painter, elementRect_.toRect(), textRect, scalRect, minValue_, maxValue_,
                        scaleNum_, backgroundColor_, showScale_,
                        scaleColor_, szScaleDir, szScalePos);

    }
}


void ElementValueStick::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementValueStick::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementValueStick::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementValueStick::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
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
        this->scaleDir_ = szDir;
    }

    if (attributes.hasAttribute("scalePos")) {
        QString szPos = attributes.value("scalePos").toString();
        this->scalePos_ = szPos;
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
        this->setElementZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    updateBoundingElement();
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
    this->szTagSelected_ = szTagSelected;
    this->maxValue_ = maxValue;
    this->minValue_ = minValue;
    this->scaleNum_ = scaleNum;
    this->backgroundColor_ = backgroundColor;
    this->foregroundColor_ = foregroundColor;
    this->scaleColor_ = scaleColor;
    this->scaleDir_ = scaleDir;
    this->scalePos_ = scalePos;
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
    ele.szTagSelected_ = szTagSelected;
    ele.maxValue_ = maxValue;
    ele.minValue_ = minValue;
    ele.scaleNum_ = scaleNum;
    ele.backgroundColor_ = backgroundColor;
    ele.foregroundColor_ = foregroundColor;
    ele.scaleColor_ = scaleColor;
    ele.scaleDir_ = scaleDir;
    ele.scalePos_ = scalePos;
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

    return in;
}



