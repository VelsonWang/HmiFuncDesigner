#include "ElementEllipse.h"
#include "RealTimeDB.h"
#include <QDebug>

ElementEllipse::ElementEllipse() {
    elementId = tr("Ellipse");
    internalElementType = tr("Ellipse");
    fillColor_ = Qt::white;
    isFill_ = true;
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    init();
}

QRectF ElementEllipse::boundingRect() const {
    qreal extra = 5;
    QRectF rect = elementRect.toRect();
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementEllipse::shape() const {
    QPainterPath path;
    path.addEllipse(elementRect);
    return path;
}

void ElementEllipse::setClickPosition(QPointF position) {
    elementXPos = position.x();
    elementYPos = position.y();
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementEllipse::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementEllipse::paint(QPainter *painter) {
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
                QString szTagID = RealTimeDB::instance()->getIdByTagName(szTagSelected_);
                if(szTagID != "") {
                    szTagValue = RealTimeDB::instance()->GetDataString(szTagID);
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
    painter->drawPath(shape());
    painter->restore();
}

void ElementEllipse::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementEllipse::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementEllipse::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementEllipse::readFromXml(const QXmlStreamAttributes &attributes) {
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

void ElementEllipse::readData(QDataStream &in) {
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

QDataStream &operator>>(QDataStream &in,ElementEllipse &ellipse) {
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

    ellipse.setElementId(id);
    ellipse.setElementXPos(xpos);
    ellipse.setElementYPos(ypos);
    ellipse.setElementZValue(zvalue);
    ellipse.setElementWidth(width);
    ellipse.setElementHeight(height);
    ellipse.szTagSelected_ = szTagSelected;
    ellipse.tagColorList_ = tagColorList;
    ellipse.fillColor_ = fillColor;
    ellipse.isFill_ = isFill;
    ellipse.borderWidth_ = borderWidth;
    ellipse.borderColor_ = borderColor;
    ellipse.showOnInitial_ = showOnInitial;
    ellipse.setAngle(angle);
    ellipse.updateBoundingElement();

    return in;
}
