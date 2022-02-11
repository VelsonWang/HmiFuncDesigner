#include "ElementInputEdit.h"
#include "ProjectData.h"
#include "variantmanager.h"
#include "editbasicpropertydialog.h"

int ElementInputEdit::iLastIndex_ = 1;

ElementInputEdit::ElementInputEdit(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
{
    elementId = QString(tr("InputEdit_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("InputEdit");
    elementIcon = QIcon(":/images/InputEdit.png");
    szHAlign_ = tr("左对齐");
    szVAlign_ = tr("居中对齐");
    backgroundColor_ = Qt::white;
    transparentBackground_ = false;
    borderWidth_ = 2;
    borderColor_ = QColor(61, 123, 173);
    enableEdit_ = true;
    enableOnInitial_ = true;
    inputPassword_ = false;
    showOnInitial_ = true;
    init();
    elementWidth = 80;
    elementHeight = 26;
    elementText = tr("输入编辑框");
    createPropertyList();
    updatePropertyModel();
}

void ElementInputEdit::regenerateElementId()
{
    elementId = QString(tr("InputEdit_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementInputEdit::release
 * @details 释放占用的资源
 */
void ElementInputEdit::release()
{
}

QRectF ElementInputEdit::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}


void ElementInputEdit::createPropertyList()
{
    propList.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;

    // ID
    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    // 允许编辑输入
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("允许编辑输入"));
    addProperty(property, QLatin1String("enableEdit"));

    // 选择变量
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("选择变量"));
    tagNames_.clear();
    m_pProjDataObj->getAllTagName(tagNames_);
    if(tagNames_.size() > 0) szTagSelected_ = tagNames_.at(0);
    property->setAttribute(QLatin1String("enumNames"), tagNames_);
    addProperty(property, QLatin1String("tag"));

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

    // 密码输入
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("密码输入"));
    addProperty(property, QLatin1String("inputPassword"));

    // 初始有效性
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始有效性"));
    addProperty(property, QLatin1String("enableOnInitial"));

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

void ElementInputEdit::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("enableEdit")) {
        enableEdit_ = value.toBool();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
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
    } else if (id == QLatin1String("inputPassword")) {
        inputPassword_ = value.toBool();
    } else if (id == QLatin1String("enableOnInitial")) {
        enableOnInitial_ = value.toBool();
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

void ElementInputEdit::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(elementId);
    }

    property = idToProperty_[QLatin1String("enableEdit")];
    if(property != Q_NULLPTR) {
        property->setValue(enableEdit_);
    }

    property = idToProperty_[QLatin1String("tag")];
    if(property != Q_NULLPTR) {
        property->setValue(tagNames_.indexOf(szTagSelected_));
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

    property = idToProperty_[QLatin1String("inputPassword")];
    if(property != Q_NULLPTR) {
        property->setValue(inputPassword_);
    }

    property = idToProperty_[QLatin1String("enableOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(enableOnInitial_);
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

void ElementInputEdit::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementInputEdit::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementInputEdit::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    drawInputEdit(painter);
    // 绘制选中状态
    paintSelected(painter, borderWidth_);
}

void ElementInputEdit::drawInputEdit(QPainter *painter)
{
    // 绘制边框
    if(borderWidth_ > 0) {
        painter->setPen(QPen(borderColor_, borderWidth_, Qt::SolidLine));
    } else {
        painter->setPen(QPen(borderColor_, 1, Qt::DashLine));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(elementRect);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QRectF rect(elementRect.toRect());
        QRectF fillRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
        QBrush brush(backgroundColor_);
        painter->fillRect(fillRect, brush);
    }

    // 绘制文本
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

    QRectF rect1(elementRect.toRect());
    QRectF textRect = rect1.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
    QString szDrawText = elementText;
    if(inputPassword_) {
        int iLen = elementText.length();
        szDrawText = "";
        for(int i=0; i<iLen; i++) {
            szDrawText.append(QChar('*'));
        }
    }
    painter->drawText(textRect, hFlags|vFlags, szDrawText);
}


/**
 * @brief ElementInputEdit::mouseDoubleClickEvent
 * @details 文本编辑控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementInputEdit::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    dlg.setText(elementText);
    dlg.setSelectedTag(szTagSelected_);
    if(dlg.exec() == QDialog::Accepted) {
        elementText = dlg.text();
        szTagSelected_ = dlg.selectedTag();

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



bool ElementInputEdit::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    enableEdit_ = pObj->getProperty("enableEdit") == "true";
    szTagSelected_ = pObj->getProperty("tag");
    setElementXPos(pObj->getProperty("x").toInt());
    setElementYPos(pObj->getProperty("y").toInt());
    setZValue(pObj->getProperty("z").toInt());
    setElementWidth(pObj->getProperty("width").toInt());
    setElementHeight(pObj->getProperty("height").toInt());
    elementText = pObj->getProperty("elementtext");
    this->setHAlignString(pObj->getProperty("halign"), szHAlign_);
    this->setVAlignString(pObj->getProperty("valign"), szVAlign_);
    backgroundColor_ = QColor(pObj->getProperty("backgroundColor"));
    transparentBackground_ = pObj->getProperty("transparentBackground") == "true";
    textColor = QColor(pObj->getProperty("textcolor"));
    font_.fromString(pObj->getProperty("font"));
    borderWidth_ = pObj->getProperty("borderWidth").toInt();
    borderColor_ = QColor(pObj->getProperty("borderColor"));
    inputPassword_ = pObj->getProperty("inputPassword") == "true";
    enableOnInitial_ = pObj->getProperty("enableOnInitial") == "true";
    showOnInitial_ = pObj->getProperty("showOnInitial") == "true";
    setAngle(pObj->getProperty("elemAngle").toInt());

    updateBoundingElement();
    updatePropertyModel();

    return true;
}


bool ElementInputEdit::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("enableEdit", enableEdit_?"true":"false");
    pObj->setProperty("tag", szTagSelected_);
    pObj->setProperty("x",QString::number(x()));
    pObj->setProperty("y",QString::number(y()));
    pObj->setProperty("z",QString::number(zValue()));
    pObj->setProperty("width",QString::number(elementWidth));
    pObj->setProperty("height",QString::number(elementHeight));
    pObj->setProperty("elementtext",elementText);
    pObj->setProperty("halign", getHAlignString(szHAlign_));
    pObj->setProperty("valign", getVAlignString(szVAlign_));
    pObj->setProperty("backgroundColor", backgroundColor_.name());
    pObj->setProperty("transparentBackground", transparentBackground_?"true":"false");
    pObj->setProperty("textcolor", textColor.name());
    pObj->setProperty("font", font_.toString());
    pObj->setProperty("borderWidth", QString::number(borderWidth_));
    pObj->setProperty("borderColor", borderColor_.name());
    pObj->setProperty("inputPassword", inputPassword_?"true":"false");
    pObj->setProperty("enableOnInitial", enableOnInitial_?"true":"false");
    pObj->setProperty("showOnInitial", showOnInitial_?"true":"false");
    pObj->setProperty("elemAngle", QString::number(elemAngle));
    return true;
}


void ElementInputEdit::writeData(QDataStream &out)
{
    out << this->elementId
        << this->enableEdit_
        << this->szTagSelected_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->elementText
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
        << this->backgroundColor_
        << this->transparentBackground_
        << this->textColor
        << this->font_.toString()
        << this->borderWidth_
        << this->borderColor_
        << this->inputPassword_
        << this->enableOnInitial_
        << this->showOnInitial_
        << this->elemAngle;
}

void ElementInputEdit::readData(QDataStream &in)
{
    QString id;
    bool enableEdit;
    QString szTagSelected;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    int borderWidth;
    QColor borderColor;
    bool inputPassword;
    bool enableOnInitial;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> enableEdit
       >> szTagSelected
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderWidth
       >> borderColor
       >> inputPassword
       >> enableOnInitial
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->enableEdit_ = enableEdit;
    this->szTagSelected_ = szTagSelected;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->elementText = text;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->textColor = textColor;
    this->font_ = font;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->inputPassword_ = inputPassword;
    this->enableOnInitial_ = enableOnInitial;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
    this->updatePropertyModel();
}

