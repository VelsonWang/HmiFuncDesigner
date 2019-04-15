#include "ElementPushButton.h"
#include "PubTool.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "Helper.h"
#include "xmlobject.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

int ElementPushButton::iLastIndex_ = 1;

ElementPushButton::ElementPushButton(const QString &projPath) :
    Element(projPath) {
    elementId = QString(tr("PushButton_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = trUtf8("PushButton");
    elementIcon = QIcon(":/images/PushButton.png");
    showContent_ = tr("文本");
    bShowContentText_ = true;
    szHAlign_ = tr("左对齐");
    szVAlign_ = tr("居中对齐");
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
    idProperty->setReadOnly(true);
    propList.insert(propList.end(), idProperty);

    titleProperty = new EmptyProperty(trUtf8("标题"));
    propList.insert(propList.end(), titleProperty);

    xCoordProperty = new IntegerProperty(trUtf8("坐标 X"));
    xCoordProperty->setSettings(0, 5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(), xCoordProperty);

    yCoordProperty = new IntegerProperty(trUtf8("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0, 5000);
    propList.insert(propList.end(), yCoordProperty);

    zValueProperty = new IntegerProperty(trUtf8("Z 值"));
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

    // 显示内容
    showContentProperty_ = new ListProperty(tr("显示内容"));
    showContentProperty_->setId(EL_SHOW_CONTENT);
    QStringList contents;
    contents << tr("文本") << tr("图片");
    showContentProperty_->setList(contents);
    showContentProperty_->setValue(showContent_);
    propList.insert(propList.end(), showContentProperty_);

    // 文本
    elementTextProperty = new TextProperty(trUtf8("文本"));
    elementTextProperty->setId(EL_TEXT);
    elementTextProperty->setValue(elementText);
    propList.insert(propList.end(),elementTextProperty);

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

    // 图片
    fileProperty = new FileProperty(trUtf8("选择图片"));
    fileProperty->setId(EL_FILE);
    fileProperty->setValue(filePicture_);
    propList.insert(propList.end(), fileProperty);


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
    case EL_SHOW_CONTENT:
        showContent_ = value.toString();
        bShowContentText_ = true;
        if(showContent_ == tr("图片")) {
            bShowContentText_ = false;
        }
        updateBoundingElement();
        break;
    case EL_TEXT:
        elementText = value.toString();
        break;
    case EL_H_ALIGN:
        szHAlign_ = value.toString();
        break;
    case EL_V_ALIGN:
        szVAlign_ = value.toString();
        break;
    case EL_FILE:
    {
        filePicture_ = value.toString();
        QFileInfo info(filePicture_);
        if(info.exists()) {
            QString picturePath = getProjectPath() + "/Pictures";
            QDir dir(picturePath);
            if(!dir.exists())
                dir.mkpath(picturePath);
            QFile::copy(filePicture_, picturePath + "/" + info.fileName());
            filePicture_ = info.fileName();
            updatePropertyModel();
        }
    }break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_FONT_SIZE:
        fontSize = value.toInt();
        break;
    case EL_ANGLE:
        elemAngle = value.toInt();
        setAngle(elemAngle);
        break;
    case EL_FUNCTION:
        funcs_ = value.toStringList();
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
    widthProperty_->setValue(elementWidth);
    heightProperty_->setValue(elementHeight);

    showContentProperty_->setValue(showContent_);
    elementTextProperty->setValue(elementText);
    hAlignProperty_->setValue(szHAlign_);
    vAlignProperty_->setValue(szVAlign_);
    fileProperty->setValue(filePicture_);

    textColorProperty->setValue(textColor);
    fontSizeProperty->setValue(fontSize);
    angleProperty->setValue(elemAngle);
    funcProperty->setValue(funcs_);
}

void ElementPushButton::setClickPosition(QPointF position) {
    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementPushButton::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPushButton::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget) {
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

    /*
    if(filePicture_ != QString()) {
        QString picture = getProjectPath() + "/Pictures/" + filePicture_;
        if(QFile::exists(picture)) {
            QImage image(getProjectPath() + "/Pictures/" + filePicture_);
            QImage scaleImage;
            if(showNoScale_) {
                scaleImage = image;
            } else {
                scaleImage = image.scaled((int)elementRect.width(), (int)elementRect.height(), Qt::IgnoreAspectRatio);
            }
            painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
            painter->drawImage(elementRect, scaleImage);
        }
    }
*/
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
    writer.writeAttribute("showContent", showContent_);
    writer.writeAttribute("picture", filePicture_);
    writer.writeAttribute("elementtext", elementText);
    writer.writeAttribute("halign", getHAlignString(szHAlign_));
    writer.writeAttribute("valign", getVAlignString(szVAlign_));
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("fontsize", QString::number(fontSize));
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeAttribute("functions", funcs_.join("|"));
    writer.writeEndElement();
}

void ElementPushButton::readFromXml(const QXmlStreamAttributes &attributes) {
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
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

    if (attributes.hasAttribute("showContent")) {
        showContent_ = attributes.value("showContent").toString();
    }

    if (attributes.hasAttribute("picture")) {
        filePicture_ = attributes.value("picture").toString();
    }

    if (attributes.hasAttribute("elementtext")) {
        elementText = attributes.value("elementtext").toString();
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->setHAlignString(align, szHAlign_);
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->setVAlignString(align, szVAlign_);
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
        << this->showContent_
        << this->filePicture_
        << this->elementText
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
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
    QString showContent;
    QString pic;
    QString text;
    QString hAlign;
    QString vAlign;
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
       >> showContent
       >> pic
       >> text
       >> hAlign
       >> vAlign
       >> textColor
       >> fontSize
       >> angle
       >> funcs;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->showContent_ = showContent;
    this->filePicture_ = pic;
    this->elementText = text;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
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
        << ele.showContent_
        << ele.filePicture_
        << ele.elementText
        << ele.getHAlignString(ele.szHAlign_)
        << ele.getVAlignString(ele.szVAlign_)
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
    QString showContent;
    QString pic;
    QString text;
    QString hAlign;
    QString vAlign;
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
       >> showContent
       >> pic
       >> text
       >> hAlign
       >> vAlign
       >> textColor
       >> fontSize
       >> angle
       >> funcs;

    ele.setElementId(id);
    int index = ele.getIndexFromIDString(id);
    if(ele.iLastIndex_ < index) {
        ele.iLastIndex_ = index;
    }
    ele.setElementXPos(xpos);
    ele.setElementYPos(ypos);
    ele.setElementZValue(zvalue);
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.showContent_ = showContent;
    ele.filePicture_ = pic;
    ele.elementText = text;
    ele.setHAlignString(hAlign, ele.szHAlign_);
    ele.setVAlignString(vAlign, ele.szVAlign_);
    ele.textColor = textColor;
    ele.fontSize = fontSize;
    ele.setAngle(angle);
    ele.funcs_ = funcs;
    ele.updateBoundingElement();
    ele.updatePropertyModel();

    return in;
}

