#include "ElementAnalogClock.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "HmiRunTime.h"
#include <QDateTime>
#include <QDate>
#include <QDebug>


ElementAnalogClock::ElementAnalogClock()
    : Element()
{
    elementId = tr("Clock");
    internalElementType = tr("Clock");
    init();
    iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();
}

QRectF ElementAnalogClock::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementAnalogClock::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementAnalogClock::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementAnalogClock::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementAnalogClock::paint(QPainter *painter)
{
    if(period_ != 0.0) {
        if(period_ < 0.5) {
            period_ = 0.5;
        }
        if((QDateTime::currentMSecsSinceEpoch() - iLastMSecs_) > period_ * 1000) {
            iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();
            // 处理"时钟到来"功能
            HmiRunTime::execScriptFunction(funcs_, tr("TimeOut"));
        }
    }

    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect_, brush);
    }

    // 绘制时钟
    drawAnalogClock(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    painter->restore();
}



void ElementAnalogClock::drawAnalogClock(QPainter *painter)
{
    painter->save();

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);
    QColor secondColor(127, 127, 0, 191);

    int iMin = qMin(elementRect_.width(), elementRect_.height());
    int iMax = qMax(elementRect_.width(), elementRect_.height());
    QTime time = QTime::currentTime();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(elementRect_.width() / 2, elementRect_.height() / 2);
    //painter->scale(1.0 * iMin / iMax, 1.0 * iMin / iMax);

    // 绘制时针
    painter->setPen(Qt::NoPen);
    painter->setBrush(hourColor);
    painter->save();
    painter->rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    QPointF hourHand[3] = {
        QPointF(7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(-7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(0, -40.0 / 100.0 * iMin / 2)
    };

    painter->drawConvexPolygon(hourHand, 3);
    painter->restore();
    painter->setPen(hourColor);

    for (int i = 0; i < 12; ++i) {
        painter->drawLine(iMin / 2 - 12.0 / 100.0 * iMin / 2, 0, iMin / 2 - 4.0 / 100.0 * iMin / 2, 0);
        painter->rotate(30.0);
    }

    // 绘制分针
    painter->setPen(Qt::NoPen);
    painter->setBrush(minuteColor);
    painter->save();
    painter->rotate(6.0 * (time.minute() + time.second() / 60.0));
    QPointF minuteHand[3] = {
        QPointF(7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(-7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(0, -70.0 / 100.0 * iMin / 2)
    };

    painter->drawConvexPolygon(minuteHand, 3);
    painter->restore();
    painter->setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter->drawLine(iMin / 2 - 8.0 / 100.0 * iMin / 2, 0, iMin / 2 - 4.0 / 100.0 * iMin / 2, 0);
        painter->rotate(6.0);
    }

    // 绘制秒针
    painter->setPen(Qt::NoPen);
    painter->setBrush(secondColor);
    painter->save();
    painter->rotate(6.0 * time.second());
    QPointF secondHand[3] = {
        QPointF(7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(-7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(0, -100.0 / 100.0 * iMin / 2)
    };

    painter->drawConvexPolygon(secondHand, 3);
    painter->restore();
    painter->setPen(secondColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter->drawLine(iMin / 2 - 8.0 / 100.0 * iMin / 2, 0, iMin / 2 - 4.0 / 100.0 * iMin / 2, 0);
        painter->rotate(6.0);
    }

    painter->restore();
}


void ElementAnalogClock::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementAnalogClock::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementAnalogClock::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementAnalogClock::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
    }

    if (attributes.hasAttribute("period")) {
        period_ = attributes.value("period").toString().toDouble();
    }

    if (attributes.hasAttribute("functions")) {
        QString listString = attributes.value("functions").toString();
        funcs_ = listString.split('|');
    }

    if (attributes.hasAttribute("backgroundColor")) {
        backgroundColor_ = QColor(attributes.value("backgroundColor").toString());
    }

    if (attributes.hasAttribute("transparentBackground")) {
        QString value = attributes.value("transparentBackground").toString();
        transparentBackground_ = false;
        if(value == "true") {
            transparentBackground_ = true;
        }
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
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


void ElementAnalogClock::readData(QDataStream &in)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> period
       >> funcs
       >> backgroundColor
       >> transparentBackground
       >> borderColor
       >> borderWidth
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    this->setElementId(id);
    this->period_ = period;
    this->funcs_ = funcs;
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->borderColor_ = borderColor;
    this->borderWidth_ = borderWidth;
    this->showOnInitial_ = showOnInitial;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in,ElementAnalogClock &clock)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> period
       >> funcs
       >> backgroundColor
       >> transparentBackground
       >> borderColor
       >> borderWidth
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    clock.setElementId(id);
    clock.period_ = period;
    clock.funcs_ = funcs;
    clock.backgroundColor_ = backgroundColor;
    clock.transparentBackground_ = transparentBackground;
    clock.borderColor_ = borderColor;
    clock.borderWidth_ = borderWidth;
    clock.showOnInitial_ = showOnInitial;
    clock.setElementXPos(static_cast<int>(xpos));
    clock.setElementYPos(static_cast<int>(ypos));
    clock.setElementZValue(static_cast<int>(zvalue));
    clock.setElementWidth(width);
    clock.setElementHeight(height);
    clock.updateBoundingElement();

    return in;
}

