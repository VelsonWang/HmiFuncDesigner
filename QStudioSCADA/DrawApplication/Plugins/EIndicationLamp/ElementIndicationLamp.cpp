#include "ElementIndicationLamp.h"
#include "ProjectData.h"
#include "TagManager.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>

int ElementIndicationLamp::iLastIndex_ = 1;

ElementIndicationLamp::ElementIndicationLamp(const QString &szProjPath, const QString &szProjName) :
    Element(szProjPath, szProjName)
{
    elementId = QString(tr("IndicationLamp_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("IndicationLamp");
    elementIcon = QIcon(":/images/IndicationLamp.png");
    resetFileIndicationLamp_ = "";
    setFileIndicationLamp_ = "";
    stateOnInitial_ = false;
    showNoScale_ = false;
    showOnInitial_ = true;
    TagManager::setProjectPath(szProjectPath_);
    init();
    elementWidth = 80;
    elementHeight = 80;
    createPropertyList();
    updatePropertyModel();

    if(ProjectData::getInstance()->getDBPath() == "")
        ProjectData::getInstance()->createOrOpenProjectData(szProjectPath_, szProjectName_);
}

void ElementIndicationLamp::regenerateElementId()
{
    elementId = QString(tr("IndicationLamp_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementIndicationLamp::release
 * @details 释放占用的资源
 */
void ElementIndicationLamp::release()
{
    if(resetFileIndicationLamp_ != "") {
        PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
        picResMgr_.del(ProjectData::getInstance()->dbData_, resetFileIndicationLamp_);
    }
    if(setFileIndicationLamp_ != "") {
        PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
        picResMgr_.del(ProjectData::getInstance()->dbData_, setFileIndicationLamp_);
    }
}

QRectF ElementIndicationLamp::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementIndicationLamp::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3, 3), elementRect.topLeft() + QPointF(3, 3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3, 3), elementRect.bottomRight() + QPointF(3, 3)));
    }

    return path;
}

void ElementIndicationLamp::createPropertyList()
{
    idProperty = new TextProperty(tr("ID"));
    idProperty->setId(EL_ID);
    idProperty->setReadOnly(true);
    propList.insert(propList.end(), idProperty);

    titleProperty = new EmptyProperty(tr("标题"));
    propList.insert(propList.end(), titleProperty);

    // 选择变量
    tagSelectProperty_ = new ListProperty(tr("选择变量"));
    tagSelectProperty_->setId(EL_TAG);
    QStringList varList;
    TagManager::getAllTagName(TagManager::getProjectPath(), varList);
    tagSelectProperty_->setList(varList);
    propList.insert(propList.end(), tagSelectProperty_);

    // 初始状态
    stateOnInitialProperty_ = new BoolProperty(tr("初始状态"));
    stateOnInitialProperty_->setId(EL_STATE_ON_INITIAL);
    stateOnInitialProperty_->setTrueText(tr("置位状态"));
    stateOnInitialProperty_->setFalseText(tr("复位状态"));
    stateOnInitialProperty_->setValue(stateOnInitial_);
    propList.insert(propList.end(), stateOnInitialProperty_);

    // 复位图片
    resetFileProperty_ = new FileProperty(tr("选择复位图片"));
    resetFileProperty_->setId(EL_PICTURE1);
    propList.insert(propList.end(), resetFileProperty_);

    // 置位图片
    setFileProperty_ = new FileProperty(tr("选择置位图片"));
    setFileProperty_->setId(EL_PICTURE2);
    propList.insert(propList.end(), setFileProperty_);

    // 原尺寸显示
    showNoScaleProperty_ = new BoolProperty(tr("原尺寸显示"));
    showNoScaleProperty_->setId(EL_SHOW_SCALE);
    showNoScaleProperty_->setTrueText(tr("是"));
    showNoScaleProperty_->setFalseText(tr("否"));
    showNoScaleProperty_->setValue(showNoScale_);
    propList.insert(propList.end(), showNoScaleProperty_);

    // 初始可见性
    showOnInitialProperty_ = new BoolProperty(tr("初始可见性"));
    showOnInitialProperty_->setId(EL_SHOW_ON_INITIAL);
    showOnInitialProperty_->setTrueText(tr("显示"));
    showOnInitialProperty_->setFalseText(tr("不显示"));
    showOnInitialProperty_->setValue(showOnInitial_);
    propList.insert(propList.end(), showOnInitialProperty_);

    xCoordProperty = new IntegerProperty(tr("坐标 X"));
    xCoordProperty->setSettings(0, 5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(), xCoordProperty);

    yCoordProperty = new IntegerProperty(tr("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0, 5000);
    propList.insert(propList.end(), yCoordProperty);

    zValueProperty = new IntegerProperty(tr("Z 值"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000, 1000);
    propList.insert(propList.end(), zValueProperty);

    // 宽度
    widthProperty_ = new IntegerProperty(tr("宽度"));
    widthProperty_->setId(EL_WIDTH);
    widthProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), widthProperty_);

    // 高度
    heightProperty_ = new IntegerProperty(tr("高度"));
    heightProperty_->setId(EL_HEIGHT);
    heightProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), heightProperty_);

}

void ElementIndicationLamp::updateElementProperty(uint id, const QVariant &value)
{
    switch (id) {
    case EL_ID:
        elementId = value.toString();
        break;
    case EL_TAG:
        szTagSelected_ = value.toString();
        break;
    case EL_STATE_ON_INITIAL:
        stateOnInitial_ = value.toBool();
        break;
    case EL_PICTURE1:
    {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            QString picturePath = getProjectPath() + "/Pictures";
            QDir dir(picturePath);
            if(!dir.exists())
                dir.mkpath(picturePath);
            QString fileDes = picturePath + "/" + infoSrc.fileName();
            QFileInfo infoDes(fileDes);
            PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
            if(resetFileIndicationLamp_ != "" && resetFileIndicationLamp_ != infoSrc.fileName()) {
                picResMgr_.del(ProjectData::getInstance()->dbData_, resetFileIndicationLamp_);
            }
            if(!infoDes.exists()) {
                QFile::copy(szTmpName, fileDes);
            }
            resetFileIndicationLamp_ = infoSrc.fileName();
            picResMgr_.add(ProjectData::getInstance()->dbData_, resetFileIndicationLamp_);
            updatePropertyModel();
        }
    }break;
    case EL_PICTURE2:
    {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            QString picturePath = getProjectPath() + "/Pictures";
            QDir dir(picturePath);
            if(!dir.exists())
                dir.mkpath(picturePath);
            QString fileDes = picturePath + "/" + infoSrc.fileName();
            QFileInfo infoDes(fileDes);
            PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
            if(setFileIndicationLamp_ != "" && setFileIndicationLamp_ != infoSrc.fileName()) {
                picResMgr_.del(ProjectData::getInstance()->dbData_, setFileIndicationLamp_);
            }
            if(!infoDes.exists()) {
                QFile::copy(szTmpName, fileDes);
            }
            setFileIndicationLamp_ = infoSrc.fileName();
            picResMgr_.add(ProjectData::getInstance()->dbData_, setFileIndicationLamp_);
            updatePropertyModel();
        }
    }break;
    case EL_X:
        elementXPos = value.toInt();
        setElementXPos(elementXPos);
        break;
    case EL_Y:
        elementYPos = value.toInt();
        setElementYPos(elementYPos);
        break;
    case EL_Z_VALUE:
        elementZValue = value.toInt();
        setZValue(elementZValue);
        break;
    case EL_WIDTH:
        elementWidth = value.toInt();
        updateBoundingElement();
        break;
    case EL_HEIGHT:
        elementHeight = value.toInt();
        updateBoundingElement();
        break;
    case EL_SHOW_SCALE:
        showNoScale_ = value.toBool();
        break;
    case EL_SHOW_ON_INITIAL:
        showOnInitial_ = value.toBool();
        break;
    }

    scene()->update();
    update();
}

void ElementIndicationLamp::updatePropertyModel()
{
    idProperty->setValue(elementId);
    tagSelectProperty_->setValue(szTagSelected_);
    stateOnInitialProperty_->setValue(stateOnInitial_);
    resetFileProperty_->setValue(resetFileIndicationLamp_);
    setFileProperty_->setValue(setFileIndicationLamp_);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty_->setValue(elementWidth);
    heightProperty_->setValue(elementHeight);
    showNoScaleProperty_->setValue(showNoScale_);
    showOnInitialProperty_->setValue(showOnInitial_);
}

void ElementIndicationLamp::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementIndicationLamp::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementIndicationLamp::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QString szFileIndicationLamp_ = QString();
    if(stateOnInitial_) {
        szFileIndicationLamp_ = setFileIndicationLamp_;
    } else {
        szFileIndicationLamp_ = resetFileIndicationLamp_;
    }

    if(szFileIndicationLamp_ != QString()) {
        QString picture = getProjectPath() + "/Pictures/" + szFileIndicationLamp_;
        if(QFile::exists(picture)) {
            QImage image(getProjectPath() + "/Pictures/" + szFileIndicationLamp_);
            QImage scaleImage;
            if(showNoScale_) {
                scaleImage = image;
            } else {
                scaleImage = image.scaled(static_cast<int>(elementRect.width()),
                                          static_cast<int>(elementRect.height()),
                                          Qt::IgnoreAspectRatio);
            }
            painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
            painter->drawImage(elementRect, scaleImage);
        }
    }

    painter->setPen(QPen(Qt::gray, 1, Qt::DashDotLine));
    painter->drawRect(elementRect);

    if (isSelected()) {
        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3, 3), elementRect.topLeft() + QPointF(3, 3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3, 3), elementRect.bottomRight() + QPointF(3, 3)));
    }
}

void ElementIndicationLamp::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    if (resizing) {
        setCursor(Qt::SizeFDiagCursor);
        switch (rd) {
        case RdBottomRight:
            elementRect.setBottomRight(mousePoint);
            elementWidth = static_cast<int>(qAbs(elementRect.topLeft().x() - elementRect.bottomRight().x()));
            elementHeight = static_cast<int>(qAbs(elementRect.topLeft().y() - elementRect.bottomRight().y()));
            break;
        case RdTopLeft:
            elementRect.setTopLeft(mousePoint);
            setElementXPos(static_cast<int>(mapToScene(elementRect.topLeft()).x()));
            setElementYPos(static_cast<int>(mapToScene(elementRect.topLeft()).y()));
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.bottomRight()).x())));
            setElementHeight(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).y() - mapToScene(elementRect.bottomRight()).y())));
            updateBoundingElement();
            break;
        case RdNone:
            QGraphicsObject::mouseMoveEvent(event);
            break;
        }

        scene()->update();
        return;
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        // 限制矩形区域
        RestrictedRectangularRegion();
    }
}

void ElementIndicationLamp::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
        rd = RdBottomRight;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else {
        resizing = false;
        rd = RdNone;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

void ElementIndicationLamp::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    elementXPos = static_cast<int>(pos().x());
    elementYPos = static_cast<int>(pos().y());
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementIndicationLamp::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}

void ElementIndicationLamp::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("stateOnInitial", stateOnInitial_?"true":"false");
    writer.writeAttribute("resetPicture", resetFileIndicationLamp_);
    writer.writeAttribute("setPicture", setFileIndicationLamp_);
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeAttribute("showNoScale", showNoScale_?"true":"false");
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeEndElement();
}

void ElementIndicationLamp::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
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
        setZValue(attributes.value("z").toString().toInt());
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
    updatePropertyModel();
}

void ElementIndicationLamp::writeData(QDataStream &out)
{
    out << this->elementId
        << this->szTagSelected_
        << this->stateOnInitial_
        << this->resetFileIndicationLamp_
        << this->setFileIndicationLamp_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->showNoScale_
        << this->showOnInitial_;
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
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->szTagSelected_ = szTagSelected;
    this->stateOnInitial_ = stateOnInitial;
    this->resetFileIndicationLamp_ = resetPic;
    this->setFileIndicationLamp_ = setPic;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->showNoScale_ = showNoScale;
    this->showOnInitial_ = showOnInitial;
    this->updateBoundingElement();
    this->updatePropertyModel();
}


QDataStream &operator<<(QDataStream &out,const ElementIndicationLamp &lamp)
{
    out << lamp.elementId
        << lamp.szTagSelected_
        << lamp.stateOnInitial_
        << lamp.resetFileIndicationLamp_
        << lamp.setFileIndicationLamp_
        << lamp.x()
        << lamp.y()
        << lamp.zValue()
        << lamp.elementWidth
        << lamp.elementHeight
        << lamp.showNoScale_
        << lamp.showOnInitial_;

    return out;
}

QDataStream &operator>>(QDataStream &in, ElementIndicationLamp &lamp)
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
    int index = lamp.getIndexFromIDString(id);
    if(lamp.iLastIndex_ < index) {
        lamp.iLastIndex_ = index;
    }
    lamp.szTagSelected_ = szTagSelected;
    lamp.stateOnInitial_ = stateOnInitial;
    lamp.resetFileIndicationLamp_ = resetPic;
    lamp.setFileIndicationLamp_ = setPic;
    lamp.setElementXPos(static_cast<int>(xpos));
    lamp.setElementYPos(static_cast<int>(ypos));
    lamp.setElementZValue(static_cast<int>(zvalue));
    lamp.setElementWidth(width);
    lamp.setElementHeight(height);
    lamp.showNoScale_ = showNoScale;
    lamp.showOnInitial_ = showOnInitial;
    lamp.updateBoundingElement();
    lamp.updatePropertyModel();

    return in;
}
