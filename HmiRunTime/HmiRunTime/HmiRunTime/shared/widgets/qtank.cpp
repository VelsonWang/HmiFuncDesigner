#include <qwidget.h>
#include <qpainter.h>
#include <QPaintEvent>
#include "qtank.h"
#include "../qprojectcore.h"
#include "../pubtool.h"

QTank::QTank(QWidget *parent) : QWidget(parent)
{
    min = 0.0;
    threshold = 70.0;
    max = 100.0;
    precision = 2;
    numTicks = 4;
    borderStyle = Flat;
    indicatorColor = Qt::blue;
    resize(minimumSizeHint());
    tagSelected = "";
    m_tag = NULL;
}

QTank::~QTank()
{
}

void QTank::setMinValue(double newMinValue)
{
    min = newMinValue;
    if(threshold < min) {
        threshold = min;
    }
    update();
}

QString QTank::getTagSelected() const
{
    return tagSelected;
}

void QTank::setTagSelected(const QString &value)
{
    if(value != tagSelected) {
        tagSelected = value;
        m_tag = RealTimeDB::instance()->tag(tagId(value));
        this->update();
    }
}

void QTank::setMaxValue(double newMaxValue)
{
    if(newMaxValue > min) {
        max = newMaxValue;
        if(threshold > max) {
            threshold = max;
        }
        update();
    }
}

void QTank::setBorderStyle(BorderStyle newBorderStyle)
{
    if(borderStyle != newBorderStyle) {
        borderStyle = newBorderStyle;
        update();
    }
}

void QTank::setPrecision(int newPrecision)
{
    if(newPrecision >= 0 && newPrecision <= 6) {
        precision = newPrecision;
        update();
    }
}

void QTank::setNumTicks(int newNumTicks)
{
    if(newNumTicks > 0) {
        numTicks = newNumTicks;
        update();
    }
}

void QTank::setThreshold(double newThreshold)
{
    if(newThreshold >= min && newThreshold <= max) {
        threshold = newThreshold;
        update();
    }
}

void QTank::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 变量当前值
    double dTagValue = 0.0;
    QString szTagValue = "#";

    if(m_tag) {
        szTagValue = m_tag->toString();
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(tagSelected));
        szTagValue = "#";
    }

    if (szTagValue != "#") {
        bool ok;
        double dVal = szTagValue.toDouble(&ok);
        if (ok) {
            dTagValue = dVal;
        }
    }

    int strLen, strHeight;

    painter.save();
    int w = this->width();
    int h = this->height();

    switch(borderStyle) {
        case Flat:
            break;
        case Frame: //to be modified
            painter.setPen(Qt::white);
            painter.drawLine(0, 0, w - 1, 0);
            painter.drawLine(0, 0, 0, h - 1);
            painter.drawLine(2, h - 2, w - 2, h - 2);
            painter.drawLine(w - 2, h - 2, w - 2, 1);
            painter.setPen(Qt::darkGray);
            painter.drawLine(2, 2, w - 4, 2);
            painter.drawLine(2, 2, 2, h - 4);
            painter.drawLine(2, h - 1, w - 2, h - 1);
            painter.drawLine(w - 1, h - 1, w - 1, 0);
            break;
        case Raised:
            painter.setPen(Qt::darkGray);
            painter.drawRect(0, 0, w - 1, h - 1);
            painter.setPen(QColor(Qt::darkGray).lighter(50)) ;
            painter.drawLine(0, 0, w - 1, 0);
            painter.drawLine(0, 0, 0, h - 1) ;
            break;
        case Sunken:
            painter.setPen(QColor(Qt::darkGray).lighter(50));
            painter.drawRect(0, 0, w - 1, h - 1);
            painter.setPen(Qt::darkGray);
            painter.drawLine(0, 0, w - 1, 0);
            painter.drawLine(0, 0, 0, h - 1);
            break;
        default:
            break;
    }

    int deltaPixel = h * 3 / 5 - 1;
    //罐体上部分
    painter.setPen(Qt::darkGray);
    painter.drawLine(w / 2 + w / 4, h / 10, w / 2 + w / 4, h * 3 / 5 + h / 10);
    painter.drawArc(w / 2 - w / 4, h / 10 - w / 16, w / 2, w / 8, 0 * 16, 45 * 16);

    painter.setPen(QColor(Qt::darkGray).lighter(50));
    painter.drawArc(w / 2 - w / 4, h / 10 - w / 16, w / 2, w / 8, 45 * 16, 135 * 16);
    painter.drawLine(w / 2 - w / 4, h / 10, w / 2 - w / 4, h * 3 / 5 + h / 10);

    //罐体底部分
    QBrush brush = QBrush(indicatorColor, Qt::SolidPattern);
    painter.setBrush(brush);
    painter.setPen(indicatorColor);
    painter.drawPie(w / 2 - w / 4 + 1, h / 10 + (h * 3 / 5) - w / 16, w / 2 - 1, w / 8, 180 * 16, 180 * 16);

    //indicator
    int valPixel = (int)( (double)deltaPixel / (max - min) * (dTagValue - min) );
    if(valPixel > h * 3 / 5 + h / 10) {
        valPixel = h * 3 / 5 + h / 10;
    }
    painter.fillRect(w / 2 - (w / 4) + 1, h * 3 / 5 + h / 10 - valPixel, w / 2 - 1, valPixel, brush);

    //ticks
    //top value
    QFontMetrics fm( this->font() );
    painter.setPen(Qt::white);
    painter.drawLine(w / 2 - (w / 4) - (w / 20), h / 10 - 1, w / 2 - (w / 4) - 1, h / 10 - 1) ;
    painter.setPen(Qt::black);
    painter.drawLine(w / 2 - (w / 4) - (w / 20), h / 10, w / 2 - (w / 4) - 1, h / 10);
    QString Max = QString("%1") .arg(max, 0, 'f', precision);
    strLen = fm.size(Qt::TextSingleLine, Max).width();
    painter.drawText(w / 2 - (w / 4) - (w / 20) - strLen, h / 10, Max);


    //for each tick in numTicks
    int tick;
    double step;
    QString val;

    int stepPixel = deltaPixel / numTicks;

    for(tick = 1; tick < numTicks; tick++) {
        step = (max - min) / numTicks;
        painter.setPen(Qt::white);
        painter.drawLine(w / 2 - (w / 4) - (w / 20), h * 3 / 5 + h / 10 - 1 - (stepPixel * tick) - 1, w / 2 - (w / 4) - 1, h * 3 / 5 + h / 10 - 1 - (stepPixel * tick) - 1);
        painter.setPen(Qt::black);
        painter.drawLine(w / 2 - (w / 4) - (w / 20), h * 3 / 5 + h / 10 - 1 - (stepPixel * tick), w / 2 - (w / 4) - 1, h * 3 / 5 + h / 10 - 1 - (stepPixel * tick) );
        val.setNum((min + (step)*tick), 'f', precision);
        strLen = fm.size(Qt::TextSingleLine, val).width();
        painter.drawText(w / 2 - (w / 4) - (w / 20) - strLen, h * 3 / 5 + h / 10 - 1 - (stepPixel * tick), val);
    }

    //bottom value
    painter.drawLine(w / 2 - (w / 4) - (w / 20), h * 3 / 5 + h / 10 - 1, w / 2 - (w / 4) - 1, h * 3 / 5 + h / 10 - 1);
    painter.setPen(Qt::white);
    painter.drawLine(w / 2 - (w / 4) - (w / 20), h * 3 / 5 + h / 10 - 2, w / 2 - (w / 4) - 1, h * 3 / 5 + h / 10 - 2);
    painter.setPen(Qt::black);
    QString Min = QString("%1") .arg(min, 0, 'f', precision);
    strLen = fm.size(Qt::TextSingleLine, Min).width();
    painter.drawText(w / 2 - (w / 4) - (w / 20) - strLen, h * 3 / 5 + h / 10 - 1, Min);

    // scale
    painter.setPen(Qt::white);
    painter.drawLine(w / 2 - (w / 4) - (w / 40) - 1, h / 10, w / 2 - (w / 4) - (w / 40) - 1, h * 3 / 5 + h / 10 - 1);
    painter.setPen(Qt::black);
    painter.drawLine(w / 2 - (w / 4) - (w / 40), h / 10, w / 2 - (w / 4) - (w / 40), h * 3 / 5 + h / 10 - 1); //shadow

    // to be modified threshold
    painter.setPen(Qt::red);
    int thresholdPixel = (int)( (double)deltaPixel / (max - min) * (threshold - min) );
    if(thresholdPixel > h * 3 / 5 + h / 10) {
        thresholdPixel = h * 3 / 5 + h / 10;
    }
    painter.drawLine(w / 2 + (w / 4) + 1, h * 3 / 5 + h / 10 - thresholdPixel, w / 2 + (w / 4) + (w / 20), h * 3 / 5 + h / 10 - thresholdPixel);
    QString strThreshold = QString("%1") .arg(threshold, 0, 'f', precision);
    strLen = fm.size(Qt::TextSingleLine, strThreshold).width();
    painter.drawText(w / 2 + (w / 4) + (w / 20), h * 3 / 5 + h / 10 - thresholdPixel, strThreshold);

    // numeric
    QString strValue = QString("%1") .arg(dTagValue, 0, 'f', precision);
    strLen = fm.size(Qt::TextSingleLine, strValue).width();
    strHeight = fm.size(Qt::TextSingleLine, strValue).height();

    //draw a double rectangle with shadow
    painter.setPen(Qt::darkGray);
    painter.drawLine(w / 2 - w / 4, h / 10 * 9 - strHeight / 2 - 3, w / 2 + w / 4, h / 10 * 9 - strHeight / 2 - 3);
    painter.drawLine(w / 2 - w / 4, h / 10 * 9 - strHeight / 2 - 3, w / 2 - w / 4, h / 10 * 9 + strHeight / 2 + 3);

    painter.setPen(Qt::white);
    painter.drawLine(w / 2 - w / 4, h / 10 * 9 + strHeight / 2 + 3, w / 2 + w / 4, h / 10 * 9 + strHeight / 2 + 3);
    painter.drawLine(w / 2 + w / 4, h / 10 * 9 - strHeight / 2 - 3, w / 2 + w / 4, h / 10 * 9 + strHeight / 2 + 3);

    // write value
    painter.setPen(Qt::black);
    painter.drawText(w / 2 - strLen / 2, h / 10 * 9 + strHeight / 2 - 2, strValue);

    painter.restore();

}

QSize QTank::sizeHint() const
{
    return minimumSizeHint();
}


QSize QTank::minimumSizeHint() const
{
    return QSize(180, 180);
}

void QTank::setColor(QColor newColor)
{
    indicatorColor = newColor ;
    update();
}

double QTank::getMinValue() const
{
    return min;
}

double QTank::getMaxValue() const
{
    return max;
}

int QTank::getPrecision() const
{
    return precision;
}

int QTank::getNumTicks() const
{
    return numTicks;
}

QColor QTank::getColor() const
{
    return indicatorColor;
}

double QTank::getThreshold() const
{
    return threshold;
}

QTank::BorderStyle QTank::getBorderStyle() const
{
    return borderStyle;
}

void QTank::fromObject(XMLObject* xml)
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
                    } else if(propertyName == "max") {
                        this->setProperty("max", pObj->getProperty("value"));
                    } else if(propertyName == "min") {
                        this->setProperty("min", pObj->getProperty("value"));
                    } else if(propertyName == "precision") {
                        this->setProperty("precision", pObj->getProperty("value"));
                    } else if(propertyName == "numTicks") {
                        this->setProperty("numTicks", pObj->getProperty("value"));
                    } else if(propertyName == "threshold") {
                        this->setProperty("threshold", pObj->getProperty("value"));
                    } else if(propertyName == "borderStyle") {
                        this->setProperty("borderStyle", pObj->getProperty("value"));
                    } else if(propertyName == "tankColor") {
                        this->setProperty("tankColor", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}

