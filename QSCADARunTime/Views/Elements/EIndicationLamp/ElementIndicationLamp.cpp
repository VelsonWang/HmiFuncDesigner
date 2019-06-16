#include "ElementIndicationLamp.h"
#include "RealTimeDB.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

ElementIndicationLamp::ElementIndicationLamp()
{
    elementId = trUtf8("指示灯");
    internalElementType = trUtf8("IndicationLamp");
    resetFileIndicationLamp_ = "";
    setFileIndicationLamp_ = "";
    stateOnInitial_ = false;
    showNoScale_ = false;
    showOnInitial_ = true;
    init();
}

QRectF ElementIndicationLamp::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementIndicationLamp::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}


void ElementIndicationLamp::setClickPosition(QPointF position)
{
    elementXPos = position.x();
    elementYPos = position.y();
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementIndicationLamp::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementIndicationLamp::paint(QPainter *painter)
{
    if(!showOnInitial_ || !bShow_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));

    QString szFileIndicationLamp_ = QString();
    qint32 id = -1;
    if (szTagSelected_ != "") {
        id = RealTimeDB::getIdByTagName(szTagSelected_);
    }
    bool bVal = false;
    if(id != -1) {
        bVal = RealTimeDB::GetDataString(id).toInt() > 0 ? true : false;
        szFileIndicationLamp_ = bVal ? setFileIndicationLamp_ : resetFileIndicationLamp_;
    }

    if(szFileIndicationLamp_ != QString()) {
        QString picture = getProjectPath() + "/Pictures/" + szFileIndicationLamp_;
        if(QFile::exists(picture)) {
            QImage image(getProjectPath() + "/Pictures/" + szFileIndicationLamp_);
            QImage scaleImage;
            if(showNoScale_) {
                scaleImage = image;
            } else {
                scaleImage = image.scaled((int)elementRect.width(), (int)elementRect.height(), Qt::IgnoreAspectRatio);
            }
            painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
            painter->drawImage(elementRect, scaleImage);
        }
    }

    painter->setPen(QPen(Qt::gray, 1, Qt::DashDotLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(elementRect);

    painter->restore();

}

void ElementIndicationLamp::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementIndicationLamp::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementIndicationLamp::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementIndicationLamp::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("stateOnInitial")) {
        QString value = attributes.value("stateOnInitial").toString();
        stateOnInitial_ = false;
        if(value == "true") {
            stateOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("resetPicture")) {
        resetFileIndicationLamp_ = attributes.value("resetPicture").toString();
    }

    if (attributes.hasAttribute("setPicture")) {
        setFileIndicationLamp_ = attributes.value("setPicture").toString();
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

    if (attributes.hasAttribute("showNoScale")) {
        QString value = attributes.value("showNoScale").toString();
        showNoScale_ = false;
        if(value == "true") {
            showNoScale_ = true;
        }
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
    }

    updateBoundingElement();

    qint32 iTagId = -1;
    if (szTagSelected_ != "") {
        iTagId = RealTimeDB::getIdByTagName(szTagSelected_);
        if (iTagId != -1) {
            if(stateOnInitial_) {
                RealTimeDB::SetDataString(iTagId, "1");
            } else {
                RealTimeDB::SetDataString(iTagId, "0");
            }
        }
    }
}

void ElementIndicationLamp::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    bool stateOnInitial;
    QString resetPic;
    QString setPic;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    bool showNoScale;
    bool showOnInitial;

    in >> id
       >> szTagSelected
       >> stateOnInitial
       >> resetPic
       >> setPic
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> showNoScale
       >> showOnInitial;

    this->setElementId(id);
    this->szTagSelected_ = szTagSelected;
    this->stateOnInitial_ = stateOnInitial;
    this->resetFileIndicationLamp_ = resetPic;
    this->setFileIndicationLamp_ = setPic;
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->showNoScale_ = showNoScale;
    this->showOnInitial_ = showOnInitial;
    this->updateBoundingElement();

    qint32 iTagId = -1;
    if (szTagSelected_ != "") {
        iTagId = RealTimeDB::getIdByTagName(szTagSelected_);
        if (iTagId != -1) {
            if(stateOnInitial_) {
                RealTimeDB::SetDataString(iTagId, "1");
            } else {
                RealTimeDB::SetDataString(iTagId, "0");
            }
        }
    }
}


QDataStream &operator>>(QDataStream &in,ElementIndicationLamp &lamp)
{
    QString id;
    QString szTagSelected;
    bool stateOnInitial;
    QString resetPic;
    QString setPic;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    bool showNoScale;
    bool showOnInitial;

    in >> id
       >> szTagSelected
       >> stateOnInitial
       >> resetPic
       >> setPic
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> showNoScale
       >> showOnInitial;

    lamp.setElementId(id);
    lamp.szTagSelected_ = szTagSelected;
    lamp.stateOnInitial_ = stateOnInitial;
    lamp.resetFileIndicationLamp_ = resetPic;
    lamp.setFileIndicationLamp_ = setPic;
    lamp.setElementXPos(xpos);
    lamp.setElementYPos(ypos);
    lamp.setElementZValue(zvalue);
    lamp.setElementWidth(width);
    lamp.setElementHeight(height);
    lamp.showNoScale_ = showNoScale;
    lamp.showOnInitial_ = showOnInitial;
    lamp.updateBoundingElement();

    qint32 iTagId = -1;
    if (lamp.szTagSelected_ != "") {
        iTagId = RealTimeDB::getIdByTagName(lamp.szTagSelected_);
        if (iTagId != -1) {
            if(lamp.stateOnInitial_) {
                RealTimeDB::SetDataString(iTagId, "1");
            } else {
                RealTimeDB::SetDataString(iTagId, "0");
            }
        }
    }
    return in;
}
