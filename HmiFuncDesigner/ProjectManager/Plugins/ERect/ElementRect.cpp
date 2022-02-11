﻿#include "ElementRect.h"
#include "ProjectData.h"
#include "variantmanager.h"
#include "editbasicpropertydialog.h"

int ElementRect::iLastIndex_ = 1;

ElementRect::ElementRect(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
{
    elementId = QString(tr("Rect_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("Rect");
    elementIcon = QIcon(":/images/rectitem.png");
    fillColor_ = Qt::white;
    isFill_ = true;
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementRect::regenerateElementId()
{
    elementId = QString(tr("Rect_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementRect::release
 * @details 释放占用的资源
 */
void ElementRect::release()
{

}

QRectF ElementRect::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}


void ElementRect::createPropertyList()
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

    // 填充颜色列表
    property = variantPropertyManager_->addProperty(VariantManager::tagColorListTypeId(), tr("填充颜色列表"));
    addProperty(property, QLatin1String("tagColorList"));

    // 填充颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("填充颜色"));
    addProperty(property, QLatin1String("fillColor"));

    // 是否填充颜色
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("填充"));
    addProperty(property, QLatin1String("isFill"));

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

void ElementRect::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];
    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("tagColorList")) {
        QString szTagColor = value.toString();
        tagColorList_ = szTagColor.split('|');
    } else if (id == QLatin1String("fillColor")) {
        fillColor_ = value.value<QColor>();
    } else if (id == QLatin1String("isFill")) {
        isFill_ = value.toBool();
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

void ElementRect::updatePropertyModel()
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

    property = idToProperty_[QLatin1String("tagColorList")];
    if(property != Q_NULLPTR) {
        property->setValue(tagColorList_.join('|'));
    }

    property = idToProperty_[QLatin1String("fillColor")];
    if(property != Q_NULLPTR) {
        property->setValue(fillColor_);
    }

    property = idToProperty_[QLatin1String("isFill")];
    if(property != Q_NULLPTR) {
        property->setValue(isFill_);
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

void ElementRect::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementRect::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementRect::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter->setPen(QPen(borderColor_, borderWidth_));
    if(isFill_) {
        painter->setBrush(QBrush(fillColor_));
    } else {
        painter->setBrush(Qt::NoBrush);
    }

    painter->drawRect(elementRect);
    // 绘制选中状态
    paintSelected(painter, borderWidth_);
}


/**
 * @brief ElementRect::mouseDoubleClickEvent
 * @details 矩形控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setSelectedTag(szTagSelected_);
    dlg.setTagColorList(tagColorList_);
    dlg.setFillColor(fillColor_);
    dlg.setIsFill(isFill_);
    dlg.setBorderWidth(borderWidth_);
    dlg.setBorderColor(borderColor_);
    if(dlg.exec() == QDialog::Accepted) {
        szTagSelected_ = dlg.selectedTag();
        tagColorList_ = dlg.tagColorList();
        fillColor_ = dlg.fillColor();
        isFill_ = dlg.isFill();
        borderWidth_ = dlg.borderWidth();
        borderColor_ = dlg.borderColor();

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

bool ElementRect::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("tag", szTagSelected_);
    pObj->setProperty("x", QString::number(x()));
    pObj->setProperty("y", QString::number(y()));
    pObj->setProperty("z", QString::number(zValue()));
    pObj->setProperty("width", QString::number(elementWidth));
    pObj->setProperty("height", QString::number(elementHeight));
    pObj->setProperty("tagColorList", tagColorList_.join("|"));
    pObj->setProperty("fillColor", fillColor_.name());
    pObj->setProperty("isFill", isFill_?"true":"false");
    pObj->setProperty("borderWidth", QString::number(borderWidth_));
    pObj->setProperty("borderColor", borderColor_.name());
    pObj->setProperty("showOnInitial", showOnInitial_?"true":"false");
    pObj->setProperty("elemAngle", QString::number(elemAngle));

    return true;
}


bool ElementRect::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    szTagSelected_ = pObj->getProperty("tag");
    setElementXPos(pObj->getProperty("x").toInt());
    setElementYPos(pObj->getProperty("y").toInt());
    setZValue(pObj->getProperty("z").toInt());
    setElementWidth(pObj->getProperty("width").toInt());
    setElementHeight(pObj->getProperty("height").toInt());
    elementText = pObj->getProperty("elementtext");

    QString listString = pObj->getProperty("tagColorList");
    tagColorList_ = listString.split('|');

    fillColor_ = QColor(pObj->getProperty("fillColor"));

    QString value = pObj->getProperty("isFill");
    isFill_ = false;
    if(value == "true") isFill_ = true;

    borderWidth_ = pObj->getProperty("borderWidth").toInt();
    borderColor_ = QColor(pObj->getProperty("borderColor"));

    value = pObj->getProperty("showOnInitial");
    showOnInitial_ = false;
    if(value == "true") showOnInitial_ = true;

    setAngle(pObj->getProperty("elemAngle").toInt());

    updateBoundingElement();
    updatePropertyModel();

    return true;
}




void ElementRect::writeData(QDataStream &out)
{
    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->szTagSelected_
        << this->tagColorList_
        << this->fillColor_
        << this->isFill_
        << this->borderWidth_
        << this->borderColor_
        << this->showOnInitial_
        << this->elemAngle;
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
    QColor fillColor;
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
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
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
    this->updatePropertyModel();
}

