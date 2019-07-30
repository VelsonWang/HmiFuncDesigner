#include "ElementClock.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "SCADARunTime.h"
#include <QDateTime>
#include <QDate>
#include <QDebug>


ElementClock::ElementClock()
    : Element()
{
    elementId = tr("Clock");
    internalElementType = tr("Clock");
    init();
    iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();
}

QRectF ElementClock::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementClock::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementClock::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementClock::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementClock::paint(QPainter *painter)
{
    if(period_ != 0.0) {
        if(period_ < 0.5) {
            period_ = 0.5;
        }
        if((QDateTime::currentMSecsSinceEpoch() - iLastMSecs_) > period_ * 1000) {
            iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();
            // 处理"时钟到来"功能
            SCADARunTime::execScriptFunction(funcs_, tr("时钟到来"));
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
    drawClock(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    painter->restore();
}


void ElementClock::drawClock(QPainter *painter)
{
    QString szWeeks[7] = { tr("星期一"), tr("星期二"), tr("星期三"),
                           tr("星期四"), tr("星期五"), tr("星期六"), tr("星期日") };
    QString szText = QDateTime::currentDateTime().toString("hh:mm:ss");
    if(showDate_) {
        szText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
    if(showWeek_) {
        int iWeek = QDate::currentDate().dayOfWeek();
        szText += szWeeks[iWeek - 1];
    }

    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    int hFlags = Qt::AlignLeft;
    if(szHAlign_ == QString("left")) {
        hFlags = Qt::AlignLeft;
    } else if(szHAlign_ == QString("center")) {
        hFlags = Qt::AlignHCenter;
    } else if(szHAlign_ == QString("right")) {
        hFlags = Qt::AlignRight;
    }

    int vFlags = Qt::AlignVCenter;
    if(szVAlign_ == QString("top")) {
        vFlags = Qt::AlignTop;
    } else if(szVAlign_ == QString("center")) {
        vFlags = Qt::AlignVCenter;
    } else if(szVAlign_ == QString("bottom")) {
        vFlags = Qt::AlignBottom;
    }

    QRectF rect(elementRect_.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, hFlags|vFlags, szText);
}


void ElementClock::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementClock::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementClock::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementClock::readFromXml(const QXmlStreamAttributes &attributes)
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

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->szHAlign_ = align;
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->szVAlign_ = align;
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("showDate")) {
        QString value = attributes.value("showDate").toString();
        showDate_ = false;
        if(value == "true") {
            showDate_ = true;
        }
    }

    if (attributes.hasAttribute("showWeek")) {
        QString value = attributes.value("showWeek").toString();
        showWeek_ = false;
        if(value == "true") {
            showWeek_ = true;
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


void ElementClock::readData(QDataStream &in)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QString hAlign;
    QString vAlign;
    QColor borderColor;
    int borderWidth;
    bool showDate;
    bool showWeek;
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
       >> textColor
       >> font
       >> hAlign
       >> vAlign
       >> borderColor
       >> borderWidth
       >> showDate
       >> showWeek
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
    this->textColor = textColor;
    this->font_ = font;
    this->szHAlign_ = hAlign;
    this->szVAlign_ = vAlign;
    this->borderColor_ = borderColor;
    this->borderWidth_ = borderWidth;
    this->showDate_ = showDate;
    this->showWeek_ = showWeek;
    this->showOnInitial_ = showOnInitial;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in,ElementClock &clock)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QString hAlign;
    QString vAlign;
    QColor borderColor;
    int borderWidth;
    bool showDate;
    bool showWeek;
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
       >> textColor
       >> font
       >> hAlign
       >> vAlign
       >> borderColor
       >> borderWidth
       >> showDate
       >> showWeek
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
    clock.textColor = textColor;
    clock.font_ = font;
    clock.szHAlign_ = hAlign;
    clock.szVAlign_ = vAlign;
    clock.borderColor_ = borderColor;
    clock.borderWidth_ = borderWidth;
    clock.showDate_ = showDate;
    clock.showWeek_ = showWeek;
    clock.showOnInitial_ = showOnInitial;
    clock.setElementXPos(static_cast<int>(xpos));
    clock.setElementYPos(static_cast<int>(ypos));
    clock.setElementZValue(static_cast<int>(zvalue));
    clock.setElementWidth(width);
    clock.setElementHeight(height);
    clock.updateBoundingElement();

    return in;
}

