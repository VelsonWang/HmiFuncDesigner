﻿#include "ElementMovingText.h"
#include "Helper.h"
#include "XMLNodeObject.h"
#include "ProjectData.h"
#include "variantmanager.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include "variantmanager.h"
#include "editbasicpropertydialog.h"

int ElementMovingText::iLastIndex_ = 1;

ElementMovingText::ElementMovingText(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
{
    elementId = QString(tr("MovingText_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("MovingText");
    elementIcon = QIcon(":/images/MovingText.png");
    backgroundColor_ = Qt::white;
    transparentBackground_ = true;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    period_ = 1.0;
    showOnInitial_ = true;
    szMoveDir_ = tr("从右到左");
    iMoveCharNum_ = 1;
    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementMovingText::regenerateElementId()
{
    elementId = QString(tr("MovingText_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementMovingText::release
 * @details 释放占用的资源
 */
void ElementMovingText::release()
{
}


QRectF ElementMovingText::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

void ElementMovingText::createPropertyList()
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

    // 变量文本列表
    property = variantPropertyManager_->addProperty(VariantManager::TagTextListTypeId(), tr("变量文本列表"));
    addProperty(property, QLatin1String("tagTextList"));

    // 文本
    property = variantPropertyManager_->addProperty(QVariant::String, tr("文本"));
    addProperty(property, QLatin1String("text"));

    // 移动方向
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("移动方向"));
    moveDirList_.clear();
    moveDirList_ << tr("从左到右") << tr("从右到左");
    property->setAttribute(QLatin1String("enumNames"), moveDirList_);
    addProperty(property, QLatin1String("moveDir"));

    // 移动字符数
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("移动字符数"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("moveCharNum"));

    // 移动间隔
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("移动间隔"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 6000);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("period"));

    // 背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("背景颜色"));
    addProperty(property, QLatin1String("background"));

    // 透明背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("透明背景颜色"));
    addProperty(property, QLatin1String("transparent"));

    // 字体
    property = variantPropertyManager_->addProperty(QVariant::Font, tr("字体"));
    addProperty(property, QLatin1String("font"));

    // 文本颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("文本颜色"));
    addProperty(property, QLatin1String("textColor"));

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
}

void ElementMovingText::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("tagTextList")) {
        QString szTagText = value.toString();
        tagTextList_ = szTagText.split('|');
    } else if (id == QLatin1String("text")) {
        elementText = value.toString();
    } else if (id == QLatin1String("moveDir")) {
        szMoveDir_ = moveDirList_.at(value.toInt());
    } else if (id == QLatin1String("moveCharNum")) {
        iMoveCharNum_ = value.toInt();
    } else if (id == QLatin1String("period")) {
        period_ = value.toDouble();
    } else if (id == QLatin1String("background")) {
        backgroundColor_ = value.value<QColor>();
    } else if (id == QLatin1String("transparent")) {
        transparentBackground_ = value.toBool();
    } else if (id == QLatin1String("font")) {
        font_ = value.value<QFont>();
    } else if (id == QLatin1String("textColor")) {
        textColor = value.value<QColor>();
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
    }

    scene()->update();
    update();
}

void ElementMovingText::updatePropertyModel()
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

    property = idToProperty_[QLatin1String("tagTextList")];
    if(property != Q_NULLPTR) {
        property->setValue(tagTextList_.join('|'));
    }

    property = idToProperty_[QLatin1String("text")];
    if(property != Q_NULLPTR) {
        property->setValue(elementText);
    }

    property = idToProperty_[QLatin1String("moveDir")];
    if(property != Q_NULLPTR) {
        property->setValue(moveDirList_.indexOf(szMoveDir_));
    }

    property = idToProperty_[QLatin1String("moveCharNum")];
    if(property != Q_NULLPTR) {
        property->setValue(iMoveCharNum_);
    }

    property = idToProperty_[QLatin1String("period")];
    if(property != Q_NULLPTR) {
        property->setValue(period_);
    }

    property = idToProperty_[QLatin1String("background")];
    if(property != Q_NULLPTR) {
        property->setValue(backgroundColor_);
    }

    property = idToProperty_[QLatin1String("transparent")];
    if(property != Q_NULLPTR) {
        property->setValue(transparentBackground_);
    }

    property = idToProperty_[QLatin1String("font")];
    if(property != Q_NULLPTR) {
        property->setValue(font_);
    }

    property = idToProperty_[QLatin1String("textColor")];
    if(property != Q_NULLPTR) {
        property->setValue(textColor);
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
}

void ElementMovingText::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementMovingText::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementMovingText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect, brush);
    }

    // 绘制文本
    drawMovingText(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect);

    // 绘制选中状态
    paintSelected(painter, borderWidth_);
}

void ElementMovingText::drawMovingText(QPainter *painter)
{
    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    QRectF rect(elementRect.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, Qt::AlignCenter, elementText);
}


/**
 * @brief ElementMovingText::mouseDoubleClickEvent
 * @details 移动文本控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementMovingText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setSelectedTag(szTagSelected_);
    dlg.setTagTextList(tagTextList_);
    dlg.setElementText(elementText);
    dlg.setMoveDir(szMoveDir_);
    dlg.setMoveCharNum(iMoveCharNum_);
    dlg.setPeriod(period_);

    if(dlg.exec() == QDialog::Accepted) {
        szTagSelected_ = dlg.selectedTag();
        tagTextList_ = dlg.tagTextList();
        elementText = dlg.elementText();
        szMoveDir_ = dlg.moveDir();
        iMoveCharNum_ = dlg.moveCharNum();
        period_ = dlg.period();

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


/**
 * @brief ElementMovingText::getMoveDirString
 * @param szAlign 文本移动方向
 * @return 文本移动方向
 */
QString ElementMovingText::getMoveDirString(const QString& szAlign) const
{
    if(szAlign == tr("从左到右")) {
        return QString("LeftToRight");
    } else if(szAlign == tr("从右到左")) {
        return QString("RightToLeft");
    }
    return QString("");
}


/**
 * @brief ElementMovingText::setMoveDirString
 * @details 设置文本移动方向
 * @param szAlign 文本移动方向
 * @param szAlignSet 待设置文本移动方向
 */
void ElementMovingText::setMoveDirString(const QString& szAlign, QString& szAlignSet)
{
    if(szAlign == QString("LeftToRight")) {
        szAlignSet = tr("从左到右");
    } else if(szAlign == QString("RightToLeft")) {
        szAlignSet = tr("从右到左");
    }
}


bool ElementMovingText::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    szTagSelected_ = pObj->getProperty("tag");
    tagTextList_ = pObj->getProperty("tagTextList").split('|');
    elementText = pObj->getProperty("elementText");
    this->setMoveDirString(pObj->getProperty("moveDir"), szMoveDir_);
    iMoveCharNum_ = pObj->getProperty("moveCharNum").toInt();
    period_ = pObj->getProperty("period").toDouble();
    backgroundColor_ = QColor(pObj->getProperty("backgroundColor"));
    transparentBackground_ = (pObj->getProperty("transparentBackground") == "true");
    textColor = QColor(pObj->getProperty("textcolor"));
    font_.fromString(pObj->getProperty("font"));
    borderColor_ = QColor(pObj->getProperty("borderColor"));
    borderWidth_ = pObj->getProperty("borderWidth").toInt();
    showOnInitial_ = (pObj->getProperty("showOnInitial") == "true");
    setElementXPos(pObj->getProperty("x").toInt());
    setElementYPos(pObj->getProperty("y").toInt());
    setZValue(pObj->getProperty("z").toInt());
    setElementWidth(pObj->getProperty("width").toInt());
    setElementHeight(pObj->getProperty("height").toInt());
    updateBoundingElement();
    updatePropertyModel();
    return true;
}


bool ElementMovingText::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("tag", szTagSelected_);
    pObj->setProperty("tagTextList", tagTextList_.join("|"));
    pObj->setProperty("elementText", elementText);
    pObj->setProperty("moveDir", getMoveDirString(szMoveDir_));
    pObj->setProperty("moveCharNum", QString::number(iMoveCharNum_));
    pObj->setProperty("period", QString::number(period_));
    pObj->setProperty("backgroundColor", backgroundColor_.name());
    pObj->setProperty("transparentBackground", transparentBackground_?"true":"false");
    pObj->setProperty("textcolor", textColor.name());
    pObj->setProperty("font", font_.toString());
    pObj->setProperty("borderColor", borderColor_.name());
    pObj->setProperty("borderWidth", QString::number(borderWidth_));
    pObj->setProperty("showOnInitial", showOnInitial_?"true":"false");
    pObj->setProperty("x", QString::number(x()));
    pObj->setProperty("y", QString::number(y()));
    pObj->setProperty("z", QString::number(zValue()));
    pObj->setProperty("width", QString::number(elementWidth));
    pObj->setProperty("height", QString::number(elementHeight));
    return true;
}


void ElementMovingText::writeData(QDataStream &out)
{
    out << this->elementId
        << this->szTagSelected_
        << this->tagTextList_
        << this->elementText
        << this->getMoveDirString(szMoveDir_)
        << this->iMoveCharNum_
        << this->period_
        << this->backgroundColor_
        << this->transparentBackground_
        << this->textColor
        << this->font_.toString()
        << this->borderColor_
        << this->borderWidth_
        << this->showOnInitial_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight;
}

void ElementMovingText::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString szMoveDir;
    int iMoveCharNum;
    double period;
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
       >> szMoveDir
       >> iMoveCharNum
       >> period
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
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->szTagSelected_ = szTagSelected;
    this->tagTextList_ = tagTextList;
    this->elementText = text;
    this->setMoveDirString(szMoveDir, szMoveDir_);
    this->iMoveCharNum_ = iMoveCharNum;

    this->period_ = period;
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
    this->updatePropertyModel();
}


