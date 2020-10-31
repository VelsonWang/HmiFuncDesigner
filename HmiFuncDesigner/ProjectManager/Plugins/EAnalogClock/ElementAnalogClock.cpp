#include "ElementAnalogClock.h"
#include "Helper.h"
#include "XMLObject.h"
#include "variantmanager.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include <QFile>

int ElementAnalogClock::iLastIndex_ = 1;

ElementAnalogClock::ElementAnalogClock(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
    : Element(pProjDataObj, propertyMgr)
{
    elementId = QString(tr("AnalogClock_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("AnalogClock");
    elementIcon = QIcon(":/images/AnalogClock.png");
    backgroundColor_ = Qt::white;
    transparentBackground_ = true;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    period_ = 0.5;
    showOnInitial_ = true;
    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementAnalogClock::regenerateElementId()
{
    elementId = QString(tr("AnalogClock_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementAnalogClock::release
 * @details 释放占用的资源
 */
void ElementAnalogClock::release()
{
}


QRectF ElementAnalogClock::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}


void ElementAnalogClock::createPropertyList()
{
    propList.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;

    // ID
    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    // 时钟间隔
    property = variantPropertyManager_->addProperty(QVariant::Double, tr("时钟间隔"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 6000);
    property->setAttribute(QLatin1String("singleStep"), 0.1);
    property->setAttribute(QLatin1String("decimals"), 1);
    addProperty(property, QLatin1String("period"));

    // 选择功能
    property = variantPropertyManager_->addProperty(VariantManager::functionTypeId(), tr("功能操作"));
    QStringList listEvents;
    getSupportEvents(listEvents);
    property->setAttribute(QLatin1String("supportevents"), listEvents.join("|"));
    addProperty(property, QLatin1String("functions"));

    // 背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("背景颜色"));
    addProperty(property, QLatin1String("background"));

    // 透明背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("透明背景颜色"));
    addProperty(property, QLatin1String("transparent"));

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

void ElementAnalogClock::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("period")) {
        period_ = value.toDouble();
    } else if (id == QLatin1String("functions")) {
        QString szFuncs = value.toString();
        funcs_ = szFuncs.split('|');
    } else if (id == QLatin1String("background")) {
        backgroundColor_ = value.value<QColor>();
    } else if (id == QLatin1String("transparent")) {
        transparentBackground_ = value.toBool();
    }  else if (id == QLatin1String("borderWidth")) {
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

void ElementAnalogClock::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(elementId);
    }

    property = idToProperty_[QLatin1String("period")];
    if(property != Q_NULLPTR) {
        property->setValue(period_);
    }

    property = idToProperty_[QLatin1String("functions")];
    if(property != Q_NULLPTR) {
        property->setValue(funcs_.join('|'));
    }

    property = idToProperty_[QLatin1String("background")];
    if(property != Q_NULLPTR) {
        property->setValue(backgroundColor_);
    }

    property = idToProperty_[QLatin1String("transparent")];
    if(property != Q_NULLPTR) {
        property->setValue(transparentBackground_);
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

void ElementAnalogClock::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementAnalogClock::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementAnalogClock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect, brush);
    }

    // 绘制时钟
    drawAnalogClock(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect);

    // 绘制选中状态
    paintSelected(painter, borderWidth_);
}


void ElementAnalogClock::drawAnalogClock(QPainter *painter)
{
    painter->save();

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);
    QColor secondColor(127, 127, 0, 191);

    int iMin = qMin(elementRect.width(), elementRect.height());
    int iMax = qMax(elementRect.width(), elementRect.height());
    QTime time = QTime::currentTime();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(elementRect.width() / 2, elementRect.height() / 2);
    //painter->scale(1.0 * iMin / iMax, 1.0 * iMin / iMax);

    // 绘制时针
    painter->setPen(Qt::NoPen);
    painter->setBrush(hourColor);
    painter->save();
    painter->rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    QPointF hourHand[3] = {
        QPointF(7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(-7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(0, -40.0 / 100.0 * iMin / 2)
    };

    painter->drawConvexPolygon(hourHand, 3);
    painter->restore();
    painter->setPen(hourColor);

    for (int i = 0; i < 12; ++i) {
        painter->drawLine(iMin / 2 - 12.0 / 100.0 * iMin / 2, 0, iMin / 2 - 4.0 / 100.0 * iMin / 2, 0);
        painter->rotate(30.0);
    }

    // 绘制分针
    painter->setPen(Qt::NoPen);
    painter->setBrush(minuteColor);
    painter->save();
    painter->rotate(6.0 * (time.minute() + time.second() / 60.0));
    QPointF minuteHand[3] = {
        QPointF(7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(-7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(0, -70.0 / 100.0 * iMin / 2)
    };

    painter->drawConvexPolygon(minuteHand, 3);
    painter->restore();
    painter->setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter->drawLine(iMin / 2 - 8.0 / 100.0 * iMin / 2, 0, iMin / 2 - 4.0 / 100.0 * iMin / 2, 0);
        painter->rotate(6.0);
    }

    // 绘制秒针
    painter->setPen(Qt::NoPen);
    painter->setBrush(secondColor);
    painter->save();
    painter->rotate(6.0 * time.second());
    QPointF secondHand[3] = {
        QPointF(7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(-7.0 / 100.0 * iMin / 2, 8.0 / 100.0 * iMin / 2),
        QPointF(0, -100.0 / 100.0 * iMin / 2)
    };

    painter->drawConvexPolygon(secondHand, 3);
    painter->restore();
    painter->setPen(secondColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter->drawLine(iMin / 2 - 8.0 / 100.0 * iMin / 2, 0, iMin / 2 - 4.0 / 100.0 * iMin / 2, 0);
        painter->rotate(6.0);
    }

    painter->restore();
}


bool ElementAnalogClock::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pObj = pXmlObj;

    QString szID = pObj->getProperty("id");
    setElementId(szID);
    int index = getIndexFromIDString(szID);
    if(iLastIndex_ < index) iLastIndex_ = index;

    period_ = pObj->getProperty("period").toDouble();

    QString listString = pObj->getProperty("functions");
    funcs_ = listString.split('|');

    backgroundColor_ = QColor(pObj->getProperty("backgroundColor"));

    QString value = pObj->getProperty("transparentBackground");
    transparentBackground_ = false;
    if(value == "true") transparentBackground_ = true;

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


bool ElementAnalogClock::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pObj = new XMLObject(pXmlObj);
    pObj->setTagName("element");
    pObj->setProperty("internalType", internalElementType);
    pObj->setProperty("id", elementId);
    pObj->setProperty("period", QString::number(period_));
    pObj->setProperty("functions", funcs_.join("|"));
    pObj->setProperty("backgroundColor", backgroundColor_.name());
    pObj->setProperty("transparentBackground", transparentBackground_?"true":"false");
    pObj->setProperty("borderWidth", QString::number(borderWidth_));
    pObj->setProperty("borderColor", borderColor_.name());
    pObj->setProperty("showOnInitial", showOnInitial_?"true":"false");
    pObj->setProperty("x", QString::number(x()));
    pObj->setProperty("y", QString::number(y()));
    pObj->setProperty("z", QString::number(zValue()));
    pObj->setProperty("width", QString::number(elementWidth));
    pObj->setProperty("height", QString::number(elementHeight));
    return true;
}


void ElementAnalogClock::writeData(QDataStream &out)
{
    out << this->elementId
        << this->period_
        << this->funcs_
        << this->backgroundColor_
        << this->transparentBackground_
        << this->borderColor_
        << this->borderWidth_
        << this->showOnInitial_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight;
}

void ElementAnalogClock::readData(QDataStream &in)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> period
       >> funcs
       >> backgroundColor
       >> transparentBackground
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
    this->period_ = period;
    this->funcs_ = funcs;
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
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


void ElementAnalogClock::getSupportEvents(QStringList &listValue)
{
    QString xmlFileName = Helper::AppDir() + "/Config/ElementSupportEvents.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists()) {
        QMessageBox::critical(Q_NULLPTR, tr("提示"), tr("事件配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, Q_NULLPTR)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* eventGroup, childrenFuncSupport) {
        QString szEventGroupName = eventGroup->getProperty("name");
        if(szEventGroupName == "Clock") {

            QList<XMLObject*> childrenGroup = eventGroup->getChildren();
            if(childrenGroup.size() < 1)
                continue;

            foreach(XMLObject* event, childrenGroup) {
                QString eventName = event->getProperty("name");
                QString eventShowName = event->getProperty("ShowName");
                listValue << QString("%1-%2").arg(eventName).arg(eventShowName);

                QList<XMLObject*> funcDesc = event->getChildren();
                if(funcDesc.size() < 1)
                    continue;
                QString strDesc = event->getCurrentChild("desc")->getText();
            }
        }
    }
}


