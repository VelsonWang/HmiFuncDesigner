#include "ElementText.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include <QDebug>

ElementText::ElementText() :
    bHide_(false)
{
    elementId = tr("Text");
    internalElementType = tr("Text");
    init();
}

QRectF ElementText::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementText::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}

void ElementText::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementText::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementText::paint(QPainter *painter)
{
    if(!showOnInitial_ || bHide_ || !bShow_) {
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
        painter->fillRect(elementRect, brush);
    }

    // 绘制文本
    drawText(painter);

    // 绘制边框
    if(borderWidth_ > 0) {
        QRect rect(static_cast<int>(elementRect.x()),
                   static_cast<int>(elementRect.y()),
                   static_cast<int>(elementRect.width()),
                   static_cast<int>(elementRect.height()));
        for(int i=0; i<borderWidth_; i++) {
            PubTool::DrawFrameRect(painter, rect, borderColor_);
            rect.adjust(1, 1, -1, -1);
        }
    }
    painter->restore();
}

void ElementText::refreshTagValue()
{
    if (szTagSelected_ == "") {
		return;
	}

    QString szTagID = RealTimeDB::getIdByTagName(szTagSelected_);
    if (szTagID != "") {
        elementText = RealTimeDB::GetDataString(szTagID);
    } else {
		elementText = "#";
	}
}

void ElementText::drawText(QPainter *painter)
{
    refreshTagValue();
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

    QRectF rect(elementRect.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, hFlags|vFlags, elementText);
}

void ElementText::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementText::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(hideOnClick_) {
        bHide_ = true;
    }
}

void ElementText::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementText::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szTagID = attributes.value("elementId").toString();
        setElementId(szTagID);
    }

	if (attributes.hasAttribute("tag")) {
		szTagSelected_ = attributes.value("tag").toString();
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

    if (attributes.hasAttribute("elementtext")) {
        elementText = attributes.value("elementtext").toString();
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        szHAlign_ = align;
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        szVAlign_ = align;
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

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("hideOnClick")) {
        QString value = attributes.value("hideOnClick").toString();
        hideOnClick_ = false;
        if(value == "true") {
            hideOnClick_ = true;
        }
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    updateBoundingElement();
}


void ElementText::readData(QDataStream &in)
{
    QString id;
	QString szTagSelected;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    int borderWidth;
    QColor borderColor;
    bool hideOnClick;
    bool showOnInitial;
    qreal angle;

    in >> id
	   >> szTagSelected
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderWidth
       >> borderColor
       >> hideOnClick
       >> showOnInitial
       >> angle;

    this->setElementId(id);
	this->szTagSelected_ = szTagSelected;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->elementText = text;
    this->szHAlign_ = hAlign;
    this->szVAlign_ = vAlign;
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->textColor = textColor;
    this->font_ = font;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->hideOnClick_ = hideOnClick;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in,ElementText &rect)
{
    QString id;
	QString szTagSelected;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    int borderWidth;
    QColor borderColor;
    bool hideOnClick;
    bool showOnInitial;
    qreal angle;

    in >> id
	   >> szTagSelected
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderWidth
       >> borderColor
       >> hideOnClick
       >> showOnInitial
       >> angle;

    rect.setElementId(id);
	rect.szTagSelected_ = szTagSelected;
    rect.setElementXPos(static_cast<int>(xpos));
    rect.setElementYPos(static_cast<int>(ypos));
    rect.setElementZValue(static_cast<int>(zvalue));
    rect.setElementWidth(width);
    rect.setElementHeight(height);
    rect.elementText = text;
    rect.szHAlign_ = hAlign;
    rect.szVAlign_ = vAlign;
    rect.backgroundColor_ = backgroundColor;
    rect.transparentBackground_ = transparentBackground;
    rect.textColor = textColor;
    rect.font_ = font;
    rect.borderWidth_ = borderWidth;
    rect.borderColor_ = borderColor;
    rect.hideOnClick_ = hideOnClick;
    rect.showOnInitial_ = showOnInitial;
    rect.setAngle(angle);
    rect.updateBoundingElement();

    return in;
}

