#include "ElementTagTextList.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "SCADARunTime.h"
#include <QDateTime>
#include <QDate>
#include <QFontMetrics>
#include <QDebug>


ElementTagTextList::ElementTagTextList()
    : Element()
{
    elementId = tr("TagTextList");
    internalElementType = tr("TagTextList");
    init();
}

QRectF ElementTagTextList::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementTagTextList::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementTagTextList::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementTagTextList::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementTagTextList::paint(QPainter *painter)
{
    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    // 绘制变量文本
    drawTagTextList(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    painter->restore();
}


void ElementTagTextList::drawTagTextList(QPainter *painter)
{
    QString szText = "";
    QColor getBackgroundColor;
    QColor getTextColor;

    // 未关联变量, 使用属性：移动文本
    szText = elementText;
    getBackgroundColor = backgroundColor_;
    getTextColor = textColor;

    // 已关联变量, 使用属性：变量文本列表
    if(szTagSelected_ != "") {
        QString szTagValue = "#";
        QString szTagID = RealTimeDB::getIdByTagName(szTagSelected_);
        if (szTagID != "") {
            szTagValue = RealTimeDB::GetDataString(szTagID);
        } else {
            szTagValue = "#";
        }

        for(int i=0; i<tagTextList_.size(); i++) {
            QString szValueColor = tagTextList_.at(i);
            QStringList listValueColor = szValueColor.split(":");
            if(listValueColor.size() == 4) {
                if(szTagValue != "#") {
                    if(szTagValue == listValueColor.at(0)) {
                        szText = listValueColor.at(1);
                        getBackgroundColor = QColor(listValueColor.at(2));
                        getTextColor = QColor(listValueColor.at(3));
                    }
                }
            }
        }
    }

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(getBackgroundColor);
        painter->fillRect(elementRect_, brush);
    }

    painter->setPen(getTextColor);
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


void ElementTagTextList::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementTagTextList::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementTagTextList::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementTagTextList::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("tagTextList")) {
        QString listString = attributes.value("tagTextList").toString();
        tagTextList_ = listString.split('|');
    }

    if (attributes.hasAttribute("elementText")) {
        elementText = attributes.value("elementText").toString();
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->szHAlign_ = align;
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->szVAlign_ = align;
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


void ElementTagTextList::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> szTagSelected
       >> tagTextList
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderColor
       >> borderWidth
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    this->setElementId(id);
    this->szTagSelected_ = szTagSelected;
    this->tagTextList_ = tagTextList;
    this->elementText = text;
    this->szHAlign_ = hAlign;
    this->szVAlign_ = vAlign;
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->textColor = textColor;
    this->font_ = font;
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

QDataStream &operator>>(QDataStream &in, ElementTagTextList &ele)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> szTagSelected
       >> tagTextList
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderColor
       >> borderWidth
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    ele.setElementId(id);
    ele.szTagSelected_ = szTagSelected;
    ele.tagTextList_ = tagTextList;
    ele.elementText = text;
    ele.szHAlign_ = hAlign;
    ele.szVAlign_ = vAlign;
    ele.backgroundColor_ = backgroundColor;
    ele.transparentBackground_ = transparentBackground;
    ele.textColor = textColor;
    ele.font_ = font;
    ele.borderColor_ = borderColor;
    ele.borderWidth_ = borderWidth;
    ele.showOnInitial_ = showOnInitial;
    ele.setElementXPos(static_cast<int>(xpos));
    ele.setElementYPos(static_cast<int>(ypos));
    ele.setElementZValue(static_cast<int>(zvalue));
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.updateBoundingElement();

    return in;
}


