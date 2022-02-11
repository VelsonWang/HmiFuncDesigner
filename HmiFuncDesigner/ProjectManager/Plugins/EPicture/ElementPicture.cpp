﻿#include "ElementPicture.h"
#include "ProjectData.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "variantmanager.h"
#include "editbasicpropertydialog.h"

int ElementPicture::iLastIndex_ = 1;

ElementPicture::ElementPicture(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
{
    elementId = QString(tr("Picture_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("Picture");
    elementIcon = QIcon(":/images/Picture.png");
    filePicture_ = "";
    showNoScale_ = false;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    init();
    elementWidth = 80;
    elementHeight = 80;
    createPropertyList();
    updatePropertyModel();
}

void ElementPicture::regenerateElementId()
{
    elementId = QString(tr("Picture_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementPicture::release
 * @details 释放占用的资源
 */
void ElementPicture::release()
{
    if(filePicture_ != "") {
        PictureResourceManager &picResMgr_ = m_pProjDataObj->pictureResourceMgr_;
        picResMgr_.del(filePicture_);
    }
}

QRectF ElementPicture::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

void ElementPicture::createPropertyList()
{
    propList.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;

    // ID
    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    // 图片
    property = variantPropertyManager_->addProperty(VariantManager::filePathTypeId(), tr("选择图片"));
    property->setAttribute(QLatin1String("filter"), "image files (*.png *.jpg *.jpeg *.bmp)");
    addProperty(property, QLatin1String("filePicture"));

    // 原尺寸显示
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("原尺寸显示"));
    addProperty(property, QLatin1String("showNoScale"));

    // 边框宽度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("边框宽度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("borderWidth"));

    // 边框颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("边框颜色"));
    addProperty(property, QLatin1String("borderColor"));

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

    // 旋转角度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("角度"));
    property->setAttribute(QLatin1String("minimum"), -360);
    property->setAttribute(QLatin1String("maximum"), 360);
    addProperty(property, QLatin1String("angle"));
}

void ElementPicture::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("filePicture")) {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            PictureResourceManager &picResMgr_ = m_pProjDataObj->pictureResourceMgr_;
            if(filePicture_ != "" && filePicture_ != infoSrc.fileName()) {
                picResMgr_.del(filePicture_);
            }
            filePicture_ = infoSrc.fileName();
            picResMgr_.add(szTmpName);
            updatePropertyModel();
        }
    } else if (id == QLatin1String("showNoScale")) {
        showNoScale_ = value.toBool();
    } else if (id == QLatin1String("borderWidth")) {
        borderWidth_ = value.toInt();
    } else if (id == QLatin1String("borderColor")) {
        borderColor_ = value.value<QColor>();
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
    } else if (id == QLatin1String("angle")) {
        elemAngle = value.toInt();
        setAngle(elemAngle);
    }

    scene()->update();
    update();
}

void ElementPicture::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(elementId);
    }

    property = idToProperty_[QLatin1String("filePicture")];
    if(property != Q_NULLPTR) {
        property->setValue(filePicture_);
    }

    property = idToProperty_[QLatin1String("showNoScale")];
    if(property != Q_NULLPTR) {
        property->setValue(showNoScale_);
    }

    property = idToProperty_[QLatin1String("borderWidth")];
    if(property != Q_NULLPTR) {
        property->setValue(borderWidth_);
    }

    property = idToProperty_[QLatin1String("borderColor")];
    if(property != Q_NULLPTR) {
        property->setValue(borderColor_);
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

    property = idToProperty_[QLatin1String("angle")];
    if(property != Q_NULLPTR) {
        property->setValue(elemAngle);
    }
}

void ElementPicture::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementPicture::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPicture::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(filePicture_ != QString()) {
        PictureResourceManager &picResMgr_ = m_pProjDataObj->pictureResourceMgr_;
        QImage image = picResMgr_.getPicture(filePicture_);
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

    if(borderWidth_ < 1) {
        borderColor = Qt::gray;
        painter->setPen(QPen(borderColor, 1, Qt::DashDotLine));
    } else {
        painter->setPen(QPen(borderColor_, borderWidth_, Qt::SolidLine));
    }
    painter->drawRect(elementRect);

    // 绘制选中状态
    paintSelected(painter, borderWidth_);
}


/**
 * @brief ElementPicture::mouseDoubleClickEvent
 * @details 图片控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementPicture::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setFilePicture(filePicture_);
    dlg.setShowNoScale(showNoScale_);
    dlg.setBorderWidth(borderWidth_);
    if(dlg.exec() == QDialog::Accepted) {
        filePicture_ = dlg.filePicture();
        showNoScale_ = dlg.showNoScale();
        borderWidth_ = dlg.borderWidth();

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



bool ElementPicture::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    setElementXPos(pObj->getProperty("x").toInt());
    setElementYPos(pObj->getProperty("y").toInt());
    setZValue(pObj->getProperty("z").toInt());
    setElementWidth(pObj->getProperty("width").toInt());
    setElementHeight(pObj->getProperty("height").toInt());
    filePicture_ = pObj->getProperty("picture");
    showNoScale_ = pObj->getProperty("showNoScale") == "true";
    borderWidth_ = pObj->getProperty("borderWidth").toInt();
    borderColor_ = QColor(pObj->getProperty("borderColor"));
    showOnInitial_ = pObj->getProperty("showOnInitial") == "true";
    setAngle(pObj->getProperty("elemAngle").toInt());

    updateBoundingElement();
    updatePropertyModel();

    return true;
}


bool ElementPicture::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("x", QString::number(x()));
    pObj->setProperty("y", QString::number(y()));
    pObj->setProperty("z", QString::number(zValue()));
    pObj->setProperty("width", QString::number(elementWidth));
    pObj->setProperty("height", QString::number(elementHeight));
    pObj->setProperty("picture", filePicture_);
    pObj->setProperty("showNoScale", showNoScale_?"true":"false");
    pObj->setProperty("borderWidth", QString::number(borderWidth_));
    pObj->setProperty("borderColor", borderColor_.name());
    pObj->setProperty("showOnInitial", showOnInitial_?"true":"false");
    pObj->setProperty("elemAngle", QString::number(elemAngle));
    return true;
}


void ElementPicture::writeData(QDataStream &out)
{
    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->filePicture_
        << this->showNoScale_
        << this->borderWidth_
        << this->borderColor_
        << this->showOnInitial_
        << this->elemAngle;
}

void ElementPicture::readData(QDataStream &in)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString pic;
    bool showNoScale;
    int borderWidth;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> pic
       >> showNoScale
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->filePicture_ = pic;
    this->showNoScale_ = showNoScale;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
    this->updatePropertyModel();
}

