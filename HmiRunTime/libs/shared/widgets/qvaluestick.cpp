#include "qvaluestick.h"
#include "../qprojectcore.h"
#include "../pubtool.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QDebug>

#define SCALE_LENTH         4
#define BIT_SCALE_LENTH     6
#define MIN_BAR_LENTH       5

QValueStick::QValueStick(QWidget *parent) : QWidget(parent)
{
    scaleDir = tr("从左到右");
    scalePos = tr("左上方");
    font = QFont("Arial Black", 10);
    backgroundColor = QColor(0xf0, 0xf0, 0xf0);
    foregroundColor = QColor(0xff, 0x00, 0x00);
    scaleColor = QColor(0x00, 0x00, 0xff);
    showRuler = true;
    showScale = true;
    showOnInitial = true;
    scaleNum = 5;
    maxValue = 100;
    minValue = 0;
    setPropertyInner();
}

void QValueStick::fromObject(XMLObject* xml)
{
    if(xml != NULL) {
        if(xml->getTagName() == "Object") {
            QList<XMLObject*> properties = xml->getChildren();
            foreach(XMLObject* pObj, properties) {
                if(pObj->getTagName() == PROPERTY_TITLE) {
                    QString propertyName = pObj->getProperty("name");
                    if(propertyName == "tag") {
                        this->setProperty("tag", pObj->getProperty("value"));
                    } else if(propertyName == "objectName") {
                        this->setProperty("objectName", pObj->getProperty("value"));
                    } else if(propertyName == "geometry") {
                        int x, y, width, height;
                        QList<XMLObject*> tmpProps = pObj->getChildren();
                        foreach(XMLObject* propObj, tmpProps) {
                            QString propertyName = propObj->getProperty("name");
                            if(propertyName == "x") {
                                x = propObj->getProperty("value").toInt();
                            } else if(propertyName == "y") {
                                y = propObj->getProperty("value").toInt();
                            } else if(propertyName == "Width") {
                                width = propObj->getProperty("value").toInt();
                            } else if(propertyName == "Height") {
                                height = propObj->getProperty("value").toInt();
                            }
                        }
                        this->setProperty("geometry", QRect(x, y, width, height));
                    } else if(propertyName == "maxValue") {
                        this->setProperty("maxValue", pObj->getProperty("value"));
                    } else if(propertyName == "minValue") {
                        this->setProperty("minValue", pObj->getProperty("value"));
                    } else if(propertyName == "scaleNum") {
                        this->setProperty("scaleNum", pObj->getProperty("value"));
                    } else if(propertyName == "textColor") {
                        this->setProperty("textColor", pObj->getProperty("value"));
                    } else if(propertyName == "backgroundColor") {
                        this->setProperty("backgroundColor", pObj->getProperty("value"));
                    } else if(propertyName == "foregroundColor") {
                        this->setProperty("foregroundColor", pObj->getProperty("value"));
                    } else if(propertyName == "scaleColor") {
                        this->setProperty("scaleColor", pObj->getProperty("value"));
                    } else if(propertyName == "scaleDir") {
                        this->setProperty("scaleDir", pObj->getProperty("value"));
                    } else if(propertyName == "scalePos") {
                        this->setProperty("scalePos", pObj->getProperty("value"));
                    } else if(propertyName == "font") {
                        this->setProperty("font", pObj->getProperty("value"));
                    } else if(propertyName == "showRuler") {
                        this->setProperty("showRuler", pObj->getProperty("value"));
                    } else if(propertyName == "showScale") {
                        this->setProperty("showScale", pObj->getProperty("value"));
                    } else if(propertyName == "showOnInitial") {
                        this->setProperty("showOnInitial", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}

void QValueStick::setPropertyInner()
{

}

void QValueStick::drawScalarStick(QPainter *painter,
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
    if(!scaleVisible) {
        return;
    }

    QFontMetrics fm(font);
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
            if(scalePosition == QString("LeftTop")) {
                iFlags = Qt::AlignRight | Qt::AlignVCenter;
            }
            aRect.setBottom(aRect.top() + iTextHeight - 1);
            painter->drawText(aRect, iFlags, szMaxValue);
            upRect = aRect;
            aRect.setBottom(textRect.bottom());
            aRect.setTop(aRect.bottom() - iTextHeight + 1);
            painter->drawText(aRect, iFlags, szMinValue);
            downRect = aRect;
        } else if ( scaleDirect ==  QString("TopToBottom") ) {
            iFlags = Qt::AlignLeft | Qt::AlignVCenter;
            if(scalePosition == QString("LeftTop")) {
                iFlags = Qt::AlignRight | Qt::AlignVCenter;
            }
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

                QRect aRectTemp = aRect;
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
                if(aRect.bottom() >= aRect.top()) {
                    PubTool::FillColorRect(painter, aRect, fillColor);
                }
            }
        }
    }
}


void QValueStick::drawValueStick(QPainter *painter)
{
    QColor color3DShadow = QColor(0x0F, 0x0F, 0x0F);
    QColor color3DHiLight = QColor(0xF0, 0xF0, 0xF0);

    // 绘制边框
    painter->setPen(QPen(QColor(61, 123, 173), 1));
    painter->setBrush(Qt::NoBrush);
    QRect elementRect = QRect(0, 0, width(), height());
    painter->drawRect(elementRect);

    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font);

    QRect barRect, scalRect, textRect;
    QFontMetrics fm(font);
    int iTextHeight = fm.height();
    QString szMinValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", minValue));
    QString szMaxValue = PubTool::DeleteEndZeroOfDecimal(QString::asprintf("%lf", maxValue));

    int iMinValueTextWidth = fm.boundingRect(szMinValue).width();
    int iMaxValueTextWidth = fm.boundingRect(szMaxValue).width();

    barRect = elementRect;
    scalRect = elementRect;
    textRect = elementRect;

    int iContHeight = static_cast<int>(elementRect.height());
    int iContWidth = static_cast<int>(elementRect.width());

    QBrush brush(backgroundColor);
    painter->fillRect(elementRect, brush);

    QString szScaleDir = dirString(scaleDir);
    QString szScalePos = posString(scalePos);

    if ( szScaleDir == QString("LeftToRight") || szScaleDir == QString("RightToLeft") ) {
        /////////////////////////////绘制水平的标尺///////////////////////////////
        bool bTextVisible = (iContWidth > (iMinValueTextWidth + iMaxValueTextWidth));

        // 计算矩形区域(水平显示)
        if ( showScale && (showRuler && bTextVisible) ) {
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
        } else if(showScale) {
            // 只标尺可见
            textRect.setTop(textRect.bottom());
            if ( iContHeight >= (BIT_SCALE_LENTH + MIN_BAR_LENTH) ) {
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
        PubTool::Draw3DFrame(painter, barRect, color3DShadow, color3DHiLight, backgroundColor);
        if ( szScaleDir == QString("LeftToRight") ) {
            barRect.setRight(barRect.left() + (barRect.right() - barRect.left()) / 2);
        } else {
            barRect.setLeft(barRect.right() - (barRect.right() - barRect.left()) / 2);
        }

        barRect = PubTool::DeflateRect(barRect, 1);
        PubTool::FillColorRect(painter, barRect, foregroundColor);

        // 绘制标尺
        drawScalarStick(painter, elementRect, textRect, scalRect, minValue, maxValue,
                        scaleNum, backgroundColor, showScale,
                        scaleColor, szScaleDir, szScalePos);

    } else {
        /////////////////////////////绘制垂直的标尺//////////////////////////////
        bool bTextVisible = (iContHeight > (2 * iTextHeight));

        // 计算矩形区域(垂直显示)
        if ( showScale && (showRuler && bTextVisible) ) {
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
        } else if (showScale) {
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
        PubTool::Draw3DFrame(painter, barRect, color3DShadow, color3DHiLight, backgroundColor);
        if ( szScaleDir == QString("TopToBottom") ) {
            barRect.setBottom(barRect.top() + (barRect.bottom() - barRect.top()) / 2);
        } else {
            barRect.setTop(barRect.bottom() - (barRect.bottom() - barRect.top()) / 2);
        }

        barRect = PubTool::DeflateRect(barRect, 1);
        PubTool::FillColorRect(painter, barRect, foregroundColor);

        //绘制标尺
        drawScalarStick(painter, elementRect, textRect, scalRect, minValue, maxValue,
                        scaleNum, backgroundColor, showScale,
                        scaleColor, szScaleDir, szScalePos);
    }
}

/**
 * @brief QValueStick::dirString
 * @details 无论属性是哪种语言都转为英文保证数据每种语言都能正确打开
 * @param szDir 标尺方向
 * @return 标尺方向描述
 */
QString QValueStick::dirString(const QString& szDir) const
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
    return szDir;
}

/**
 * @brief QValueStick::posString
 * @param szDir 标尺位置
 * @return 标尺位置描述
 */
QString QValueStick::posString(const QString& szPos) const
{
    if(szPos == tr("右下方")) {
        return QString("RightBottom");
    } else if(szPos == tr("左上方")) {
        return QString("LeftTop");
    }
    return szPos;
}

void QValueStick::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    // 绘制数值棒图
    drawValueStick(&painter);
    QWidget::paintEvent(event);
}

void QValueStick::resizeEvent(QResizeEvent *event)
{
    setPropertyInner();
    QWidget::resizeEvent(event);
}

QColor QValueStick::getTextColor() const
{
    return textColor;
}

void QValueStick::setTextColor(const QColor &value)
{
    if(value != textColor) {
        textColor = value;
        this->update();
    }
}

bool QValueStick::getShowOnInitial() const
{
    return showOnInitial;
}

void QValueStick::setShowOnInitial(bool value)
{
    if(value != showOnInitial) {
        showOnInitial = value;
        this->update();
    }
}

bool QValueStick::getShowScale() const
{
    return showScale;
}

void QValueStick::setShowScale(bool value)
{
    if(value != showScale) {
        showScale = value;
        this->update();
    }
}

bool QValueStick::getShowRuler() const
{
    return showRuler;
}

void QValueStick::setShowRuler(bool value)
{
    if(value != showRuler) {
        showRuler = value;
        this->update();
    }
}

QFont QValueStick::getFont() const
{
    return font;
}

void QValueStick::setFont(const QFont &value)
{
    if(value != font) {
        font = value;
        this->update();
    }
}

QString QValueStick::getScalePos() const
{
    return scalePos;
}

void QValueStick::setScalePos(const QString &value)
{
    if(value != scalePos) {
        scalePos = value;
        this->update();
    }
}

QString QValueStick::getScaleDir() const
{
    return scaleDir;
}

void QValueStick::setScaleDir(const QString &value)
{
    if(value != scaleDir) {
        scaleDir = value;
        this->update();
    }
}

QColor QValueStick::getScaleColor() const
{
    return scaleColor;
}

void QValueStick::setScaleColor(const QColor &value)
{
    if(value != scaleColor) {
        scaleColor = value;
        this->update();
    }
}

QColor QValueStick::getForegroundColor() const
{
    return foregroundColor;
}

void QValueStick::setForegroundColor(const QColor &value)
{
    if(value != foregroundColor) {
        foregroundColor = value;
        this->update();
    }
}

QColor QValueStick::getBackgroundColor() const
{
    return backgroundColor;
}

void QValueStick::setBackgroundColor(const QColor &value)
{
    if(value != backgroundColor) {
        backgroundColor = value;
        this->update();
    }
}

int QValueStick::getScaleNum() const
{
    return scaleNum;
}

void QValueStick::setScaleNum(int value)
{
    if(value != scaleNum) {
        scaleNum = value;
        this->update();
    }
}

double QValueStick::getMinValue() const
{
    return minValue;
}

void QValueStick::setMinValue(double value)
{
    if(value != minValue) {
        minValue = value;
        this->update();
    }
}

double QValueStick::getMaxValue() const
{
    return maxValue;
}

void QValueStick::setMaxValue(double value)
{
    if(value != maxValue) {
        maxValue = value;
        this->update();
    }
}

QString QValueStick::getTagSelected() const
{
    return tagSelected;
}

void QValueStick::setTagSelected(const QString &value)
{
    if(value != tagSelected) {
        tagSelected = value;
        this->update();
    }
}


