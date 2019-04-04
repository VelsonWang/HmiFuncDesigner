#include "ElementPushButton.h"
#include "PubTool.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "Helper.h"
#include "xmlobject.h"
#include <QMessageBox>
#include <QDebug>

ElementPushButton::ElementPushButton(const QString &projPath) :
    Element(projPath)
{
    elementId = trUtf8("弹出按钮");
    internalElementType = trUtf8("PushButton");
    elementIcon = QIcon(":/images/PushButton.png");

    init();

    elementWidth = 100;
    elementHeight = 40;
    backgroundColor = QColor(240, 240, 240);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    elementText = trUtf8("弹出按钮");

    TagManager::setProjectPath(projPath);
    DrawListUtils::setProjectPath(projPath);

    createPropertyList();
    updatePropertyModel();
}

QRectF ElementPushButton::boundingRect() const {

    qreal extra = 5;

    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementPushButton::shape() const {
    QPainterPath path;
    path.addRect(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }

    return path;
}

void ElementPushButton::createPropertyList() {

    idProperty = new TextProperty(trUtf8("ID"));
    idProperty->setId(EL_ID);
    propList.insert(propList.end(),idProperty);

    titleProperty = new EmptyProperty(trUtf8("标题"));
    propList.insert(propList.end(),titleProperty);

    xCoordProperty = new IntegerProperty(trUtf8("坐标 X"));
    xCoordProperty->setSettings(0,5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(),xCoordProperty);

    yCoordProperty = new IntegerProperty(trUtf8("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0,5000);
    propList.insert(propList.end(),yCoordProperty);

    zValueProperty = new IntegerProperty(trUtf8("Z 值"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000,1000);
    propList.insert(propList.end(),zValueProperty);

    widthProperty = new IntegerProperty(trUtf8("宽度"));
    widthProperty->setId(EL_WIDTH);
    widthProperty->setSettings(0,5000);
    propList.insert(propList.end(),widthProperty);

    heightProperty = new IntegerProperty(trUtf8("高度"));
    heightProperty->setId(EL_HEIGHT);
    heightProperty->setSettings(0,5000);
    propList.insert(propList.end(),heightProperty);

    elementTextProperty = new TextProperty(trUtf8("文本"));
    elementTextProperty->setId(EL_TEXT);
    propList.insert(propList.end(),elementTextProperty);

    textColorProperty = new ColorProperty(trUtf8("颜色"));
    textColorProperty->setId(EL_FONT_COLOR);
    propList.insert(propList.end(),textColorProperty);

    fontSizeProperty = new IntegerProperty(trUtf8("字体"));
    fontSizeProperty->setId(EL_FONT_SIZE);
    fontSizeProperty->setSettings(8,72);
    propList.insert(propList.end(),fontSizeProperty);

    angleProperty = new IntegerProperty(trUtf8("角度"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0,360);
    propList.insert(propList.end(),angleProperty);

    QStringList listEvents;
    getSupportEvents(listEvents);
    funcProperty = new FunctionProperty(trUtf8("功能操作"));
    funcProperty->setId(EL_FUNCTION);
    funcProperty->setSupportEvents(listEvents);
    propList.insert(propList.end(), funcProperty);
}

void ElementPushButton::updateElementProperty(uint id, const QVariant &value) {

    switch (id) {

    case EL_ID:
        elementId = value.toString();
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
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_FONT_SIZE:
        fontSize = value.toInt();
        break;
    case EL_TEXT:
        elementText = value.toString();
        break;
    case EL_ANGLE:
        elemAngle = value.toInt();
        setAngle(elemAngle);
        break;
    case EL_FUNCTION:
        funcs_ = value.toStringList();
        qDebug() << funcs_;
        break;
    }

    update();
    scene()->update();
}

void ElementPushButton::updatePropertyModel() {

    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
    textColorProperty->setValue(textColor);
    fontSizeProperty->setValue(fontSize);
    elementTextProperty->setValue(elementText);
    angleProperty->setValue(elemAngle);
    funcProperty->setValue(funcs_);
}

void ElementPushButton::setClickPosition(QPointF position) {

    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);

    elementRect.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementPushButton::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPushButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    drawPushButton(painter);

    if (isSelected()) {

        painter->setPen(QPen(borderColor));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
}

void ElementPushButton::drawPushButton(QPainter *painter) {

    QRect rect(elementRect.x(), elementRect.y(), elementRect.width(), elementRect.height());
    for(int i=0; i<borderWidth; i++) {
        PubTool::DrawFrameRect(painter, rect, borderColor);
        if(i<borderWidth/2) rect.adjust(1, 1, -1, -1);
        else rect.adjust(1, 1, 0, 0);
    }

    PubTool::DrawFrameRect(painter, rect, QColor(252, 252, 252));
    rect.adjust(1, 1, -1, -1);

    rect.adjust(-1, -1, 0, 0);
    PubTool::FillFullRect(painter, rect, backgroundColor);

    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);

    QFont font = painter->font();
    font.setFamily("Arial Black");
    font.setPointSize(fontSize);

    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, elementText);
}

void ElementPushButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();

    if (resizing) {

        setCursor(Qt::SizeFDiagCursor);

        switch (rd) {

        case RdBottomRight:
            elementRect.setBottomRight(mousePoint);
            elementWidth = qAbs(elementRect.topLeft().x() - elementRect.bottomRight().x());
            elementHeight = qAbs(elementRect.topLeft().y() - elementRect.bottomRight().y());
            break;
        case RdTopLeft:
            elementRect.setTopLeft(mousePoint);
            setElementXPos(mapToScene(elementRect.topLeft()).x());
            setElementYPos(mapToScene(elementRect.topLeft()).y());
            setElementWidth(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.bottomRight()).x()));
            setElementHeight(qAbs(mapToScene(elementRect.topLeft()).y() - mapToScene(elementRect.bottomRight()).y()));
            updateBoundingElement();
            break;
        case RdNone:
            QGraphicsObject::mouseMoveEvent(event);
            break;
        }

        scene()->update();
        return;
    }
    else {
        QGraphicsObject::mouseMoveEvent(event);
    }
}

void ElementPushButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y()))
    {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y()))
    {
        rd = RdBottomRight;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else {
        resizing = false;
        rd = RdNone;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

void ElementPushButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    setCursor(Qt::ArrowCursor);
    elementXPos = pos().x();
    elementYPos = pos().y();
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementPushButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y()))
    {

        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y()))
    {

        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}

void ElementPushButton::writeAsXml(QXmlStreamWriter &writer) {

    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeAttribute("elementtext", elementText);
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("fontsize", QString::number(fontSize));
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeAttribute("functions", funcs_.join("|"));
    writer.writeEndElement();
}

void ElementPushButton::readFromXml(const QXmlStreamAttributes &attributes) {

    if (attributes.hasAttribute("elementId")) {
        setElementId(attributes.value("elementId").toString());
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

    if (attributes.hasAttribute("elementtext")) {
        elementText = attributes.value("elementtext").toString();
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("fontsize")) {
        fontSize = attributes.value("fontsize").toString().toInt();
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    if (attributes.hasAttribute("functions")) {
        QString listString = attributes.value("functions").toString();
        funcs_ = listString.split('|');
    }

    updateBoundingElement();
    updatePropertyModel();
}

void ElementPushButton::writeData(QDataStream &out) {

    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->elementText
        << this->textColor
        << this->fontSize
        << this->elemAngle
        << this->funcs_;
}

void ElementPushButton::readData(QDataStream &in) {

    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QColor textColor;
    int fontSize;
    qreal angle;
    QStringList funcs;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> textColor
       >> fontSize
       >> angle
       >> funcs;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->elementText = text;
    this->textColor = textColor;
    this->fontSize = fontSize;
    this->setAngle(angle);
    this->funcs_ = funcs;
    this->updateBoundingElement();
    this->updatePropertyModel();
}

void ElementPushButton::getSupportEvents(QStringList &listValue) {

    QString xmlFileName = Helper::AppDir() + "/Config/ElementSupportEvents.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists()) {
        QMessageBox::critical(0, tr("提示"), tr("事件配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, 0)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* eventGroup, childrenFuncSupport) {
        QString szEventGroupName = eventGroup->getProperty("name");
        if(szEventGroupName == "PushButton") {

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


QDataStream &operator<<(QDataStream &out,const ElementPushButton &ele) {

    out << ele.elementId
        << ele.x()
        << ele.y()
        << ele.zValue()
        << ele.elementWidth
        << ele.elementHeight
        << ele.elementText
        << ele.textColor
        << ele.fontSize
        << ele.elemAngle
        << ele.funcs_;
    return out;
}

QDataStream &operator>>(QDataStream &in,ElementPushButton &ele) {

    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QColor textColor;
    int fontSize;
    qreal angle;
    QStringList funcs;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> textColor
       >> fontSize
       >> angle
       >> funcs;

    ele.setElementId(id);
    ele.setElementXPos(xpos);
    ele.setElementYPos(ypos);
    ele.setElementZValue(zvalue);
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.elementText = text;
    ele.textColor = textColor;
    ele.fontSize = fontSize;
    ele.setAngle(angle);
    ele.funcs_ = funcs;
    ele.updateBoundingElement();
    ele.updatePropertyModel();

    return in;
}

