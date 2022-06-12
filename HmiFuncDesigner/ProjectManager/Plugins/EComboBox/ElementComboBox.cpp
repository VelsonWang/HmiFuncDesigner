#include "ElementComboBox.h"
#include "Helper.h"
#include "XMLNodeObject.h"
#include "ProjectData.h"
#include "variantmanager.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include "editbasicpropertydialog.h"

int ElementComboBox::iLastIndex_ = 1;

ElementComboBox::ElementComboBox(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
{
    elementId = QString(tr("ComboBox_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("ComboBox");
    elementIcon = QIcon(":/images/ComboBox.png");
    szHAlign_ = tr("左对齐");
    szVAlign_ = tr("居中对齐");
    backgroundColor_ = Qt::white;
    transparentBackground_ = true;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementComboBox::regenerateElementId()
{
    elementId = QString(tr("ComboBox_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementComboBox::release
 * @details 释放占用的资源
 */
void ElementComboBox::release()
{
}


QRectF ElementComboBox::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}


void ElementComboBox::createPropertyList()
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
    addProperty(property, QLatin1String("ComboBox"));

    // 文本
    property = variantPropertyManager_->addProperty(QVariant::String, tr("文本"));
    addProperty(property, QLatin1String("text"));

    // 水平对齐
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("水平对齐"));
    hAlignList_.clear();
    hAlignList_ << tr("左对齐") << tr("居中对齐") << tr("右对齐");
    property->setAttribute(QLatin1String("enumNames"), hAlignList_);
    addProperty(property, QLatin1String("hAlign"));

    // 垂直对齐
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("垂直对齐"));
    vAlignList_.clear();
    vAlignList_ << tr("上对齐") << tr("居中对齐") << tr("下对齐");
    property->setAttribute(QLatin1String("enumNames"), vAlignList_);
    addProperty(property, QLatin1String("vAlign"));

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

void ElementComboBox::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("ComboBox")) {
        QString szTagText = value.toString();
        ComboBox_ = szTagText.split('|');
    } else if (id == QLatin1String("text")) {
        elementText = value.toString();
    } else if (id == QLatin1String("hAlign")) {
        szHAlign_ = hAlignList_.at(value.toInt());
    } else if (id == QLatin1String("vAlign")) {
        szVAlign_ = vAlignList_.at(value.toInt());
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

void ElementComboBox::updatePropertyModel()
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

    property = idToProperty_[QLatin1String("ComboBox")];
    if(property != Q_NULLPTR) {
        property->setValue(ComboBox_.join('|'));
    }

    property = idToProperty_[QLatin1String("text")];
    if(property != Q_NULLPTR) {
        property->setValue(elementText);
    }

    property = idToProperty_[QLatin1String("hAlign")];
    if(property != Q_NULLPTR) {
        property->setValue(hAlignList_.indexOf(szHAlign_));
    }

    property = idToProperty_[QLatin1String("vAlign")];
    if(property != Q_NULLPTR) {
        property->setValue(vAlignList_.indexOf(szVAlign_));
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
        QStringList fontlist=font_.family().split(",");
        if(fontlist.length()>6)
        {//lxw:复制粘贴时，字体和大小一次
          font_.setFamily(QString(fontlist.at(0)));
          font_.setPointSize(QString(fontlist.at(1)).toInt());
          //font_.setBold(QString(fontlist.at(4)).toInt());
          font_.setItalic(QString(fontlist.at(5)).toInt());
          font_.setUnderline(QString(fontlist.at(6)).toInt());
          font_.setStrikeOut(QString(fontlist.at(7)).toInt());
          //font_.setKerning(QString(fontlist.at(1)).toInt());
        }
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

void ElementComboBox::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementComboBox::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementComboBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    drawComboBox(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect);

    // 绘制选中状态
    paintSelected(painter, borderWidth_);
}

void ElementComboBox::drawComboBox(QPainter *painter)
{
    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    int hFlags = Qt::AlignLeft;
    if(szHAlign_ == tr("左对齐")) {
        hFlags = Qt::AlignLeft;
    } else if(szHAlign_ == tr("居中对齐")) {
        hFlags = Qt::AlignHCenter;
    } else if(szHAlign_ == tr("右对齐")) {
        hFlags = Qt::AlignRight;
    }

    int vFlags = Qt::AlignVCenter;
    if(szVAlign_ == tr("上对齐")) {
        vFlags = Qt::AlignTop;
    } else if(szVAlign_ == tr("居中对齐")) {
        vFlags = Qt::AlignVCenter;
    } else if(szVAlign_ == tr("下对齐")) {
        vFlags = Qt::AlignBottom;
    }

    QRectF rect(elementRect.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, hFlags|vFlags, elementText);
    int harrow=elementRect.width()/3;
    harrow=harrow>150?150:harrow;
    harrow=harrow<50?50:harrow;
    painter->setBrush(Qt::gray);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawRect(QRectF(elementRect.topRight().x()-harrow,elementRect.topRight().y(),harrow,elementRect.height()));

    if (isSelected())
    {
        painter->drawText(QRectF(elementRect.topRight().x()-harrow/3-font_.pointSize()*1.5,elementRect.topRight().y(),harrow,elementRect.height()),"▲");
        painter->drawText(QRectF(elementRect.topRight().x()-harrow/3-font_.pointSize()*1.5,elementRect.bottomRight().y()-font_.pointSize()*1.5,harrow,elementRect.height()),"▼");
    }
    else
    {
        painter->drawText(QRectF(elementRect.topRight().x()-harrow/3-font_.pointSize()*1.5,elementRect.bottomRight().y()-font_.pointSize()-elementRect.height()/2,harrow,elementRect.height()),"▼");
    }
}


/**
 * @brief ElementComboBox::mouseDoubleClickEvent
 * @details 变量文本列表控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementComboBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    EditBasicPropertyDialog dlg;
    dlg.setSelectedTag(szTagSelected_);
    dlg.setTagTextList(ComboBox_);
    dlg.setTransparentBackground(transparentBackground_);
    dlg.setElementText(elementText);
    dlg.setTextColor(textColor);
    dlg.setBackgroundColor(backgroundColor_);
    dlg.setBorderWidth(borderWidth_);
    dlg.setBorderColor(borderColor_);
    dlg.setHAlign(szHAlign_);
    dlg.setVAlign(szVAlign_);
    if(dlg.exec() == QDialog::Accepted) {
        szTagSelected_ = dlg.selectedTag();
        ComboBox_ = dlg.tagTextList();
        transparentBackground_ = dlg.transparentBackground();
        elementText = dlg.elementText();
        textColor = dlg.textColor();
        backgroundColor_ = dlg.backgroundColor();
        borderWidth_ = dlg.borderWidth();
        borderColor_ = dlg.borderColor();
        szHAlign_ = dlg.hAlign();
        szVAlign_ = dlg.vAlign();

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


bool ElementComboBox::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("tag", szTagSelected_);
    pObj->setProperty("ComboBox", ComboBox_.join("|"));
    pObj->setProperty("elementText", elementText);
    pObj->setProperty("halign", getHAlignString(szHAlign_));
    pObj->setProperty("valign", getVAlignString(szVAlign_));
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


bool ElementComboBox::openFromXml(XMLObject *pXmlObj) {
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    szTagSelected_ = pObj->getProperty("tag");

    QString listString = pObj->getProperty("ComboBox");
    ComboBox_ = listString.split('|');

    elementText = pObj->getProperty("elementText");

    QString align = pObj->getProperty("halign");
    this->setHAlignString(align, szHAlign_);

    align = pObj->getProperty("valign");
    this->setVAlignString(align, szVAlign_);

    backgroundColor_ = QColor(pObj->getProperty("backgroundColor"));

    QString value = pObj->getProperty("transparentBackground");
    transparentBackground_ = false;
    if(value == "true") transparentBackground_ = true;

    textColor = QColor(pObj->getProperty("textcolor"));

    QString szFont = pObj->getProperty("font");
    font_.fromString(szFont);

    borderColor_ = QColor(pObj->getProperty("borderColor"));
    borderWidth_ = pObj->getProperty("borderWidth").toInt();

    value = pObj->getProperty("showOnInitial");
    showOnInitial_ = false;
    if(value == "true") showOnInitial_ = true;

    setElementXPos(pObj->getProperty("x").toInt());
    setElementYPos(pObj->getProperty("y").toInt());
    setZValue(pObj->getProperty("z").toInt());
    setElementWidth(pObj->getProperty("width").toInt());
    setElementHeight(pObj->getProperty("height").toInt());

    updateBoundingElement();
    updatePropertyModel();

    return true;
}


void ElementComboBox::writeData(QDataStream &out)
{
    out << this->elementId
        << this->szTagSelected_
        << this->ComboBox_
        << this->elementText
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
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

void ElementComboBox::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QStringList ComboBox;
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
       >> ComboBox
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
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->szTagSelected_ = szTagSelected;
    this->ComboBox_ = ComboBox;
    this->elementText = text;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
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


