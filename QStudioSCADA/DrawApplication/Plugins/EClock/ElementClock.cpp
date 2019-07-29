#include "ElementClock.h"
#include "Helper.h"
#include "XMLObject.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDate>

int ElementClock::iLastIndex_ = 1;

ElementClock::ElementClock(const QString &szProjPath, const QString &szProjName)
    : Element(szProjPath, szProjName)
{
    elementId = QString(tr("Clock_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("Clock");
    elementIcon = QIcon(":/images/Clock.png");
    szHAlign_ = tr("居中对齐");
    szVAlign_ = tr("居中对齐");
    backgroundColor_ = Qt::white;
    transparentBackground_ = true;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    period_ = 0.5;
    font_.setPixelSize(18);
    showDate_ = false;
    showWeek_ = false;
    showOnInitial_ = true;

    TagManager::setProjectPath(szProjectPath_);
    DrawListUtils::setProjectPath(szProjectPath_);
    ElementIDHelper::setProjectPath(szProjectPath_);

    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementClock::regenerateElementId()
{
    elementId = QString(tr("Clock_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementClock::release
 * @details 释放占用的资源
 */
void ElementClock::release()
{

}


QRectF ElementClock::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementClock::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    if (isSelected()) {
        path.addRect(QRectF(elementRect_.topLeft() - QPointF(3,3), elementRect_.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect_.bottomRight() - QPointF(3,3), elementRect_.bottomRight() + QPointF(3,3)));
    }
    return path;
}

void ElementClock::createPropertyList()
{
    // ID
    idProperty_ = new TextProperty(tr("ID"));
    idProperty_->setId(EL_ID);
    idProperty_->setReadOnly(true);
    propList.insert(propList.end(), idProperty_);

    // 标题
    titleProperty_ = new EmptyProperty(tr("标题"));
    propList.insert(propList.end(), titleProperty_);

    // 时钟间隔
    periodProperty_ = new DoubleProperty(tr("时钟间隔"));
    periodProperty_->setId(EL_PERIOD);
    periodProperty_->setSettings(0, 6000, 5);
    periodProperty_->setValue(period_);
    propList.insert(propList.end(), periodProperty_);

    // 选择功能
    QStringList listEvents;
    getSupportEvents(listEvents);
    funcProperty_ = new FunctionProperty(tr("功能操作"));
    funcProperty_->setId(EL_FUNCTION);
    funcProperty_->setSupportEvents(listEvents);
    propList.insert(propList.end(), funcProperty_);

    // 背景颜色
    backgroundColorProperty_ = new ColorProperty(tr("背景颜色"));
    backgroundColorProperty_->setId(EL_BACKGROUND);
    backgroundColorProperty_->setValue(backgroundColor_);
    propList.insert(propList.end(), backgroundColorProperty_);

    // 透明背景颜色
    transparentBackgroundProperty_ = new BoolProperty(tr("透明背景颜色"));
    transparentBackgroundProperty_->setId(EL_TRANSPARENT_BACKGROUND);
    transparentBackgroundProperty_->setTrueText(tr("透明"));
    transparentBackgroundProperty_->setFalseText(tr("不透明"));
    transparentBackgroundProperty_->setValue(transparentBackground_);
    propList.insert(propList.end(), transparentBackgroundProperty_);

    // 字体
    fontProperty_ = new FontProperty(tr("字体"));
    fontProperty_->setId(EL_FONT);
    fontProperty_->setValue(QFont("Arial Black", 12));
    propList.insert(propList.end(), fontProperty_);

    // 文本颜色
    textColorProperty = new ColorProperty(tr("文本颜色"));
    textColorProperty->setId(EL_FONT_COLOR);
    propList.insert(propList.end(),textColorProperty);

    // 水平对齐
    hAlignProperty_ = new ListProperty(tr("水平对齐"));
    hAlignProperty_->setId(EL_H_ALIGN);
    QStringList hAlignList;
    hAlignList << tr("左对齐") << tr("居中对齐") << tr("右对齐");
    hAlignProperty_->setList(hAlignList);
    propList.insert(propList.end(), hAlignProperty_);

    // 垂直对齐
    vAlignProperty_ = new ListProperty(tr("水平对齐"));
    vAlignProperty_->setId(EL_V_ALIGN);
    QStringList vAlignList;
    vAlignList << tr("上对齐") << tr("居中对齐") << tr("下对齐");
    vAlignProperty_->setList(vAlignList);
    propList.insert(propList.end(), vAlignProperty_);

    // 边框宽度
    borderWidthProperty_ = new IntegerProperty(tr("边框宽度"));
    borderWidthProperty_->setId(EL_BORDER_WIDTH);
    borderWidthProperty_->setSettings(0, 1000);
    borderWidthProperty_->setValue(borderWidth_);
    propList.insert(propList.end(), borderWidthProperty_);

    // 边框颜色
    borderColorProperty_ = new ColorProperty(tr("边框颜色"));
    borderColorProperty_->setId(EL_BORDER_COLOR);
    borderColorProperty_->setValue(borderColor_);
    propList.insert(propList.end(), borderColorProperty_);

    // 显示日期
    showDateProperty_ = new BoolProperty(tr("显示日期"));
    showDateProperty_->setId(EL_SHOW_DATE);
    showDateProperty_->setTrueText(tr("是"));
    showDateProperty_->setFalseText(tr("否"));
    showDateProperty_->setValue(showDate_);
    propList.insert(propList.end(), showDateProperty_);

    // 显示星期
    showWeekProperty_ = new BoolProperty(tr("显示星期"));
    showWeekProperty_->setId(EL_SHOW_WEEK);
    showWeekProperty_->setTrueText(tr("是"));
    showWeekProperty_->setFalseText(tr("否"));
    showWeekProperty_->setValue(showWeek_);
    propList.insert(propList.end(), showWeekProperty_);

    // 初始可见性
    showOnInitialProperty_ = new BoolProperty(tr("初始可见性"));
    showOnInitialProperty_->setId(EL_SHOW_ON_INITIAL);
    showOnInitialProperty_->setTrueText(tr("显示"));
    showOnInitialProperty_->setFalseText(tr("不显示"));
    showOnInitialProperty_->setValue(showOnInitial_);
    propList.insert(propList.end(), showOnInitialProperty_);

    xCoordProperty_ = new IntegerProperty(tr("坐标 X"));
    xCoordProperty_->setSettings(0, 5000);
    xCoordProperty_->setId(EL_X);
    propList.insert(propList.end(), xCoordProperty_);

    yCoordProperty_ = new IntegerProperty(tr("坐标 Y"));
    yCoordProperty_->setId(EL_Y);
    yCoordProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), yCoordProperty_);

    zValueProperty_ = new IntegerProperty(tr("Z 值"));
    zValueProperty_->setId(EL_Z_VALUE);
    zValueProperty_->setSettings(-1000, 1000);
    propList.insert(propList.end(), zValueProperty_);

    widthProperty_ = new IntegerProperty(tr("宽度"));
    widthProperty_->setId(EL_WIDTH);
    widthProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), widthProperty_);

    heightProperty_ = new IntegerProperty(tr("高度"));
    heightProperty_->setId(EL_HEIGHT);
    heightProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), heightProperty_);
}

void ElementClock::updateElementProperty(uint id, const QVariant &value)
{
    switch (id) {
    case EL_ID:
        elementId = value.toString();
        break;
    case EL_PERIOD:
        period_ = value.toDouble();
		break;
    case EL_FUNCTION:
        funcs_ = value.toStringList();
        break;
    case EL_BACKGROUND:
        backgroundColor_ = value.value<QColor>();
        break;
    case EL_TRANSPARENT_BACKGROUND:
        transparentBackground_ = value.toBool();
        break;
    case EL_FONT:
        font_ = value.value<QFont>();
        break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_H_ALIGN:
        szHAlign_ = value.toString();
        break;
    case EL_V_ALIGN:
        szVAlign_ = value.toString();
        break;
    case EL_BORDER_WIDTH:
        borderWidth_ = value.toInt();
        break;
    case EL_BORDER_COLOR:
        borderColor_ = value.value<QColor>();
        break;
    case EL_SHOW_DATE:
        showDate_ = value.toBool();
        break;
    case EL_SHOW_WEEK:
        showWeek_ = value.toBool();
        break;
    case EL_SHOW_ON_INITIAL:
        showOnInitial_ = value.toBool();
        break;
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
    }

    update();
    scene()->update();
}

void ElementClock::updatePropertyModel()
{
    idProperty_->setValue(elementId);
    periodProperty_->setValue(period_);
    funcProperty_->setValue(funcs_);
    backgroundColorProperty_->setValue(backgroundColor_);
    transparentBackgroundProperty_->setValue(transparentBackground_);
    fontProperty_->setValue(font_);
    textColorProperty->setValue(textColor);
    hAlignProperty_->setValue(szHAlign_);
    vAlignProperty_->setValue(szVAlign_);
    borderWidthProperty_->setValue(borderWidth_);
    borderColorProperty_->setValue(borderColor_);
    showDateProperty_->setValue(showDate_);
    showWeekProperty_->setValue(showWeek_);
    showOnInitialProperty_->setValue(showOnInitial_);
    xCoordProperty_->setValue(elementXPos);
    yCoordProperty_->setValue(elementYPos);
    zValueProperty_->setValue(elementZValue);
    widthProperty_->setValue(elementWidth);
    heightProperty_->setValue(elementHeight);
}

void ElementClock::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect_.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementClock::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementClock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect_, brush);
    }

    // 绘制时钟
    drawClock(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    if (isSelected()) {
        painter->setPen(QPen(borderColor));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect_.topLeft() - QPointF(3,3), elementRect_.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect_.bottomRight() - QPointF(3,3), elementRect_.bottomRight() + QPointF(3,3)));
    }
}

void ElementClock::drawClock(QPainter *painter)
{
    QString szWeeks[7] = { tr("星期一"), tr("星期二"), tr("星期三"),
                           tr("星期四"), tr("星期五"), tr("星期六"), tr("星期日") };
    QString szText = QDateTime::currentDateTime().toString("hh:mm:ss");
    if(showDate_) {
        szText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
    if(showWeek_) {
        int iWeek = QDate::currentDate().dayOfWeek();
        szText += szWeeks[iWeek - 1];
    }

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

    QRectF rect(elementRect_.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, hFlags|vFlags, szText);
}

void ElementClock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();

    if (resizing) {
        setCursor(Qt::SizeFDiagCursor);

        switch (rd) {
        case RdBottomRight:
            elementRect_.setBottomRight(mousePoint);
            elementWidth = static_cast<int>(qAbs(elementRect_.topLeft().x() - elementRect_.bottomRight().x()));
            elementHeight = static_cast<int>(qAbs(elementRect_.topLeft().y() - elementRect_.bottomRight().y()));
            break;
        case RdTopLeft:
            elementRect_.setTopLeft(mousePoint);
            setElementXPos(static_cast<int>(mapToScene(elementRect_.topLeft()).x()));
            setElementYPos(static_cast<int>(mapToScene(elementRect_.topLeft()).y()));
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementRect_.topLeft()).x() - mapToScene(elementRect_.bottomRight()).x())));
            setElementHeight(static_cast<int>(qAbs(mapToScene(elementRect_.topLeft()).y() - mapToScene(elementRect_.bottomRight()).y())));
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

void ElementClock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect_.topLeft();
    QPointF bottomRight = elementRect_.bottomRight();

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

void ElementClock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void ElementClock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect_.topLeft();
    QPointF bottomRight = elementRect_.bottomRight();

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

void ElementClock::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("period", QString::number(period_));
    writer.writeAttribute("functions", funcs_.join("|"));
    writer.writeAttribute("backgroundColor", backgroundColor_.name());
    writer.writeAttribute("transparentBackground", transparentBackground_?"true":"false");
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("halign", getHAlignString(szHAlign_));
    writer.writeAttribute("valign", getVAlignString(szVAlign_));
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("borderWidth", QString::number(borderWidth_));
    writer.writeAttribute("showDate", showDate_?"true":"false");
    writer.writeAttribute("showWeek", showWeek_?"true":"false");
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeEndElement();
}

void ElementClock::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
    }

    if (attributes.hasAttribute("period")) {
        period_ = attributes.value("period").toString().toDouble();
    }

    if (attributes.hasAttribute("functions")) {
        QString listString = attributes.value("functions").toString();
        funcs_ = listString.split('|');
    }

    if (attributes.hasAttribute("backgroundColor")) {
        backgroundColor_ = QColor(attributes.value("backgroundColor").toString());
    }

    if (attributes.hasAttribute("transparentBackground")) {
        QString value = attributes.value("transparentBackground").toString();
        transparentBackground_ = false;
        if(value == "true") {
            transparentBackground_ = true;
        }
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->setHAlignString(align, szHAlign_);
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->setVAlignString(align, szVAlign_);
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("showDate")) {
        QString value = attributes.value("showDate").toString();
        showDate_ = false;
        if(value == "true") {
            showDate_ = true;
        }
    }

    if (attributes.hasAttribute("showWeek")) {
        QString value = attributes.value("showWeek").toString();
        showWeek_ = false;
        if(value == "true") {
            showWeek_ = true;
        }
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
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

    updateBoundingElement();
    updatePropertyModel();
}

void ElementClock::writeData(QDataStream &out)
{
    out << this->elementId
        << this->period_
        << this->funcs_
        << this->backgroundColor_
        << this->transparentBackground_
        << this->textColor
        << this->font_.toString()
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
        << this->borderColor_
        << this->borderWidth_
        << this->showDate_
        << this->showWeek_
        << this->showOnInitial_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight;
}

void ElementClock::readData(QDataStream &in)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QString hAlign;
    QString vAlign;
    QColor borderColor;
    int borderWidth;
    bool showDate;
    bool showWeek;
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
       >> textColor
       >> font
       >> hAlign
       >> vAlign
       >> borderColor
       >> borderWidth
       >> showDate
       >> showWeek
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
    this->textColor = textColor;
    this->font_ = font;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
    this->borderColor_ = borderColor;
    this->borderWidth_ = borderWidth;
    this->showDate_ = showDate;
    this->showWeek_ = showWeek;
    this->showOnInitial_ = showOnInitial;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->updateBoundingElement();
    this->updatePropertyModel();
}


void ElementClock::getSupportEvents(QStringList &listValue)
{
    QString xmlFileName = Helper::AppDir() + "/Config/ElementSupportEvents.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists()) {
        QMessageBox::critical(nullptr, tr("提示"), tr("事件配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, nullptr)) {
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
                listValue << eventShowName;

                QList<XMLObject*> funcDesc = event->getChildren();
                if(funcDesc.size() < 1)
                    continue;
                QString strDesc = event->getCurrentChild("desc")->getText();
            }
        }
    }
}


QDataStream &operator<<(QDataStream &out,const ElementClock &clock)
{
    out << clock.elementId
        << clock.period_
        << clock.funcs_
        << clock.backgroundColor_
        << clock.transparentBackground_
        << clock.textColor
        << clock.font_.toString()
        << clock.getHAlignString(clock.szHAlign_)
        << clock.getVAlignString(clock.szVAlign_)
        << clock.borderColor_
        << clock.borderWidth_
        << clock.showDate_
        << clock.showWeek_
        << clock.showOnInitial_
        << clock.x()
        << clock.y()
        << clock.zValue()
        << clock.elementWidth
        << clock.elementHeight;

    return out;
}

QDataStream &operator>>(QDataStream &in, ElementClock &clock)
{
    QString id;
    double period;
    QStringList funcs;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QString hAlign;
    QString vAlign;
    QColor borderColor;
    int borderWidth;
    bool showDate;
    bool showWeek;
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
       >> textColor
       >> font
       >> hAlign
       >> vAlign
       >> borderColor
       >> borderWidth
       >> showDate
       >> showWeek
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    clock.setElementId(id);
    int index = clock.getIndexFromIDString(id);
    if(clock.iLastIndex_ < index) {
        clock.iLastIndex_ = index;
    }
    clock.period_ = period;
    clock.funcs_ = funcs;
    clock.backgroundColor_ = backgroundColor;
    clock.transparentBackground_ = transparentBackground;
    clock.textColor = textColor;
    clock.font_ = font;
    clock.setHAlignString(hAlign, clock.szHAlign_);
    clock.setVAlignString(vAlign, clock.szVAlign_);
    clock.borderColor_ = borderColor;
    clock.borderWidth_ = borderWidth;
    clock.showDate_ = showDate;
    clock.showWeek_ = showWeek;
    clock.showOnInitial_ = showOnInitial;
    clock.setElementXPos(static_cast<int>(xpos));
    clock.setElementYPos(static_cast<int>(ypos));
    clock.setElementZValue(static_cast<int>(zvalue));
    clock.setElementWidth(width);
    clock.setElementHeight(height);
    clock.updateBoundingElement();
    clock.updatePropertyModel();

    return in;
}

