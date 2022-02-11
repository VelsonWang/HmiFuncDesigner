#include "ElementRect.h"
#include "../../Public/PubTool.h"
#include "../../Public/RealTimeDB.h"
#include <QDebug>

ElementRect::ElementRect()
{
    elementId = tr("Rect");
    internalElementType = tr("Rect");
    fillColor_ = Qt::white;
    isFill_ = true;
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    init();
}

QRectF ElementRect::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementRect::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}


void ElementRect::setClickPosition(QPointF position)
{
    elementXPos = position.x();
    elementYPos = position.y();
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementRect::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementRect::paint(QPainter *painter)
{
    if(!showOnInitial_ || !bShow_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setPen(QPen(borderColor_, borderWidth_));
    if(isFill_) {
        if(szTagSelected_ != "" && bEnable_) {
            QString szTagValue = "";
            if(tagColorList_.size()) {
                QString szTagID = pRtdbObj_->getIdByTagName(szTagSelected_);
                if(szTagID != "") {
                    szTagValue = pRtdbObj_->GetDataString(szTagID);
                } else {
                    szTagValue = "#";
                }
            }
            foreach (QString szValueColor, tagColorList_) {
                QStringList listValueColor = szValueColor.split(':');
                if(listValueColor.size() == 2) {
                    QString szValue = listValueColor.at(0);
                    QString szColor = listValueColor.at(1);
                    if(szValue == szTagValue) {
                        lastFillColor_ = QColor(szColor);
                    }
                }
            }
            painter->setBrush(QBrush(lastFillColor_));
        } else {
            painter->setBrush(QBrush(fillColor_));
        }
    } else {
        painter->setBrush(Qt::NoBrush);
    }
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);
    painter->drawRect(elementRect);
    painter->restore();
}

void ElementRect::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementRect::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementRect::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementRect::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        setElementId(attributes.value("elementId").toString());
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

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("tagColorList")) {
        QString listString = attributes.value("tagColorList").toString();
        tagColorList_ = listString.split('|');
    }

    if (attributes.hasAttribute("fillColor")) {
        fillColor_ = QColor(attributes.value("fillColor").toString());
        lastFillColor_ = fillColor_;
    }

    if (attributes.hasAttribute("isFill")) {
        QString value = attributes.value("isFill").toString();
        isFill_ = false;
        if(value == "true") {
            isFill_ = true;
        }
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
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


void ElementRect::readData(QDataStream &in)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString szTagSelected;
    QStringList tagColorList;
    QString fillColor;
    bool isFill;
    int borderWidth;
    QColor borderColor;
    qreal angle;
    bool showOnInitial;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> szTagSelected
       >> tagColorList
       >> fillColor
       >> isFill
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->szTagSelected_ = szTagSelected;
    this->tagColorList_ = tagColorList;
    this->fillColor_ = fillColor;
    this->isFill_ = isFill;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
}


QDataStream &operator>>(QDataStream &in, ElementRect &rect)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString szTagSelected;
    QStringList tagColorList;
    QString fillColor;
    bool isFill;
    int borderWidth;
    QColor borderColor;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> szTagSelected
       >> tagColorList
       >> fillColor
       >> isFill
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    rect.setElementId(id);
    rect.setElementXPos(xpos);
    rect.setElementYPos(ypos);
    rect.setElementZValue(zvalue);
    rect.setElementWidth(width);
    rect.setElementHeight(height);
    rect.szTagSelected_ = szTagSelected;
    rect.tagColorList_ = tagColorList;
    rect.fillColor_ = fillColor;
    rect.isFill_ = isFill;
    rect.borderWidth_ = borderWidth;
    rect.borderColor_ = borderColor;
    rect.showOnInitial_ = showOnInitial;
    rect.setAngle(angle);
    rect.updateBoundingElement();

    return in;
}
