#include "ElementIndicationLamp.h"
#include "ProjectData.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "variantmanager.h"
#include "editbasicpropertydialog.h"
#include <QDebug>

int ElementIndicationLamp::iLastIndex_ = 1;

ElementIndicationLamp::ElementIndicationLamp(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
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
    init();
    elementWidth = 80;
    elementHeight = 80;
    createPropertyList();
    updatePropertyModel();
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
        picResMgr_.del(resetFileIndicationLamp_);
    }
    if(setFileIndicationLamp_ != "") {
        PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
        picResMgr_.del(setFileIndicationLamp_);
    }
}

QRectF ElementIndicationLamp::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}


void ElementIndicationLamp::createPropertyList()
{
    propList.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;

    // ID
    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    // 选择变量
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("选择变量"));
    tagNames_.clear();
    m_pProjDataObj->getAllTagName(tagNames_);
    if(tagNames_.size() > 0) szTagSelected_ = tagNames_.at(0);
    property->setAttribute(QLatin1String("enumNames"), tagNames_);
    addProperty(property, QLatin1String("tag"));

    // 初始状态
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始状态"));
    addProperty(property, QLatin1String("stateOnInitial"));

    // 复位图片
    property = variantPropertyManager_->addProperty(VariantManager::filePathTypeId(), tr("选择复位图片"));
    property->setAttribute(QLatin1String("filter"), "image files (*.png *.jpg *.jpeg *.bmp)");
    addProperty(property, QLatin1String("resetPicture"));

    // 置位图片
    property = variantPropertyManager_->addProperty(VariantManager::filePathTypeId(), tr("选择置位图片"));
    property->setAttribute(QLatin1String("filter"), "image files (*.png *.jpg *.jpeg *.bmp)");
    addProperty(property, QLatin1String("setPicture"));

    // 原尺寸显示
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("原尺寸显示"));
    addProperty(property, QLatin1String("showNoScale"));

    // 初始可见性
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始可见性"));
    addProperty(property, QLatin1String("showOnInitial"));

    // 坐标 X
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("坐标 X"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("xCoord"));

    // 坐标 Y
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("坐标 Y"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("yCoord"));

    // Z 值
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("Z 值"));
    property->setAttribute(QLatin1String("minimum"), -1000);
    property->setAttribute(QLatin1String("maximum"), 1000);
    addProperty(property, QLatin1String("zValue"));

    // 宽度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("宽度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("width"));

    // 高度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("高度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("height"));
}

void ElementIndicationLamp::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("stateOnInitial")) {
        stateOnInitial_ = value.toBool();
    } else if (id == QLatin1String("resetPicture")) {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
            if(resetFileIndicationLamp_ != "" && resetFileIndicationLamp_ != infoSrc.fileName()) {
                picResMgr_.del(resetFileIndicationLamp_);
            }
            resetFileIndicationLamp_ = infoSrc.fileName();
            picResMgr_.add(szTmpName);
            updatePropertyModel();
        }
    } else if (id == QLatin1String("setPicture")) {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
            if(setFileIndicationLamp_ != "" && setFileIndicationLamp_ != infoSrc.fileName()) {
                picResMgr_.del(setFileIndicationLamp_);
            }
            setFileIndicationLamp_ = infoSrc.fileName();
            picResMgr_.add(szTmpName);
            updatePropertyModel();
        }
    } else if (id == QLatin1String("showNoScale")) {
        showNoScale_ = value.toBool();
    } else if (id == QLatin1String("showOnInitial")) {
        showOnInitial_ = value.toBool();
    } else if (id == QLatin1String("xCoord")) {
        elementXPos = value.toInt();
        setElementXPos(elementXPos);
    } else if (id == QLatin1String("yCoord")) {
        elementYPos = value.toInt();
        setElementYPos(elementYPos);
    } else if (id == QLatin1String("zValue")) {
        elementZValue = value.toInt();
        setZValue(elementZValue);
    } else if (id == QLatin1String("width")) {
        elementWidth = value.toInt();
        updateBoundingElement();
    } else if (id == QLatin1String("height")) {
        elementHeight = value.toInt();
        updateBoundingElement();
    }

    scene()->update();
    update();
}

void ElementIndicationLamp::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(elementId);
    }

    property = idToProperty_[QLatin1String("tag")];
    if(property != Q_NULLPTR) {
        property->setValue(tagNames_.indexOf(szTagSelected_));
    }

    property = idToProperty_[QLatin1String("stateOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(stateOnInitial_);
    }

    property = idToProperty_[QLatin1String("resetPicture")];
    if(property != Q_NULLPTR) {
        property->setValue(resetFileIndicationLamp_);
    }

    property = idToProperty_[QLatin1String("setPicture")];
    if(property != Q_NULLPTR) {
        property->setValue(setFileIndicationLamp_);
    }

    property = idToProperty_[QLatin1String("showNoScale")];
    if(property != Q_NULLPTR) {
        property->setValue(showNoScale_);
    }

    property = idToProperty_[QLatin1String("showOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(showOnInitial_);
    }

    property = idToProperty_[QLatin1String("xCoord")];
    if(property != Q_NULLPTR) {
        property->setValue(elementXPos);
    }

    property = idToProperty_[QLatin1String("yCoord")];
    if(property != Q_NULLPTR) {
        property->setValue(elementYPos);
    }

    property = idToProperty_[QLatin1String("zValue")];
    if(property != Q_NULLPTR) {
        property->setValue(elementZValue);
    }

    property = idToProperty_[QLatin1String("width")];
    if(property != Q_NULLPTR) {
        property->setValue(elementWidth);
    }

    property = idToProperty_[QLatin1String("height")];
    if(property != Q_NULLPTR) {
        property->setValue(elementHeight);
    }
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
        PictureResourceManager &picResMgr_ = m_pProjDataObj->pictureResourceMgr_;
        QImage image = picResMgr_.getPicture(szFileIndicationLamp_);
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
    } else {
        painter->save();
        qreal fHalfWidth = elementRect.width()/2;
        qreal fHalfHeight = elementRect.height()/2;
        qreal fRadius = (fHalfWidth < fHalfHeight) ? fHalfWidth : fHalfHeight;
        QRadialGradient radialGradient(fHalfWidth, fHalfHeight, fRadius, fHalfWidth, fHalfHeight);
        // 创建了一个QRadialGradient对象实例，参数分别为中心坐标，半径长度和焦点坐标,
        // 如果需要对称那么中心坐标和焦点坐标要一致
        if(stateOnInitial_){
            radialGradient.setColorAt(0, Qt::yellow);
            radialGradient.setColorAt(0.8, Qt::blue); // 设置50%处的半径为蓝色
        }else{
            radialGradient.setColorAt(0, Qt::black);
            radialGradient.setColorAt(0.8, Qt::white); // 设置50%处的半径为蓝色
        }

        radialGradient.setColorAt(1, Qt::darkGray);
        painter->setBrush(QBrush(radialGradient));
        QRectF tmpRect;
        tmpRect.setX(elementRect.x()+elementRect.width()/2-fRadius);
        tmpRect.setY(elementRect.y()+elementRect.height()/2-fRadius);
        tmpRect.setWidth(fRadius*2);
        tmpRect.setHeight(fRadius*2);
        painter->drawEllipse(tmpRect);
        painter->restore();
    }

    painter->setPen(QPen(Qt::gray, 1, Qt::DashDotLine));
    painter->drawRect(elementRect);

    // 绘制选中状态
    paintSelected(painter, 1);
}


/**
 * @brief ElementIndicationLamp::mouseDoubleClickEvent
 * @details 指示灯控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementIndicationLamp::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setSelectedTag(szTagSelected_);
    dlg.setStateOnInitial(stateOnInitial_);
    dlg.setResetFileIndicationLamp(resetFileIndicationLamp_);
    dlg.setSetFileIndicationLamp(setFileIndicationLamp_);
    if(dlg.exec() == QDialog::Accepted) {
        szTagSelected_ = dlg.selectedTag();
        stateOnInitial_ = dlg.stateOnInitial();
        resetFileIndicationLamp_ = dlg.resetFileIndicationLamp();
        setFileIndicationLamp_ = dlg.setFileIndicationLamp();

        // 更新属性表
        VariantManager *pVariantManager = dynamic_cast<VariantManager *>(variantPropertyManager_);
        if(pVariantManager != Q_NULLPTR) {
            QtTreePropertyBrowser *pPropertyEditor = pVariantManager->getPropertyEditor();
            if(pPropertyEditor != Q_NULLPTR) {
                pPropertyEditor->clear();
                this->updatePropertyModel();
                QListIterator<QtProperty*> iter(this->getPropertyList());
                while (iter.hasNext()) {
                    pPropertyEditor->addProperty(iter.next());
                }
            }
        }

        scene()->update();
        update();
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}


bool ElementIndicationLamp::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    szTagSelected_ = pObj->getProperty("tag");
    stateOnInitial_ = pObj->getProperty("stateOnInitial") == "true";
    resetFileIndicationLamp_ = pObj->getProperty("resetPicture");
    setFileIndicationLamp_ = pObj->getProperty("setPicture");
    setElementXPos(pObj->getProperty("x").toInt());
    setElementYPos(pObj->getProperty("y").toInt());
    setZValue(pObj->getProperty("z").toInt());
    setElementWidth(pObj->getProperty("width").toInt());
    setElementHeight(pObj->getProperty("height").toInt());
    showNoScale_ = pObj->getProperty("showNoScale") == "true";
    showOnInitial_ = pObj->getProperty("showOnInitial") == "true";

    updateBoundingElement();
    updatePropertyModel();

    return true;
}


bool ElementIndicationLamp::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("tag", szTagSelected_);
    pObj->setProperty("stateOnInitial", stateOnInitial_?"true":"false");
    pObj->setProperty("resetPicture", resetFileIndicationLamp_);
    pObj->setProperty("setPicture", setFileIndicationLamp_);
    pObj->setProperty("x", QString::number(x()));
    pObj->setProperty("y", QString::number(y()));
    pObj->setProperty("z", QString::number(zValue()));
    pObj->setProperty("width", QString::number(elementWidth));
    pObj->setProperty("height", QString::number(elementHeight));
    pObj->setProperty("showNoScale", showNoScale_?"true":"false");
    pObj->setProperty("showOnInitial", showOnInitial_?"true":"false");
    return true;
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



