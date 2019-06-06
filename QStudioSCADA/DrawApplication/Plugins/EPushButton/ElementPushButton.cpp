#include "ElementPushButton.h"
#include "PubTool.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include "Helper.h"
#include "XMLObject.h"
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
    szHAlign_ = tr("居中对齐");
    szVAlign_ = tr("居中对齐");
    font_ = QFont("宋体", 12);
    init();
    elementWidth = 100;
    elementHeight = 40;
    backgroundColor_ = QColor(240, 240, 240);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    elementText = trUtf8("弹出按钮");
    enableOnInitial_ = true;
    showOnInitial_ = true;
    transparent_ = false;
    TagManager::setProjectPath(projPath);
    DrawListUtils::setProjectPath(projPath);
    ElementIDHelper::setProjectPath(projPath);

    createPropertyList();
    updatePropertyModel();
}

void ElementPushButton::regenerateElementId() {
    elementId = QString(tr("PushButton_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementPushButton::release
 * @details 释放占用的资源
 */
void ElementPushButton::release()
{
    qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__;
}

QRectF ElementPushButton::boundingRect() const {
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementPushButton::shape() const {
    QPainterPath path;
    path.addRect(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3), elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3), elementRect.bottomRight() + QPointF(3,3)));
    }

    return path;
}

void ElementPushButton::createPropertyList()
{
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
    if(bShowContentText_)
        propList.insert(propList.end(), elementTextProperty);

    // 水平对齐
    hAlignProperty_ = new ListProperty(tr("水平对齐"));
    hAlignProperty_->setId(EL_H_ALIGN);
    QStringList hAlignList;
    hAlignList << tr("左对齐") << tr("居中对齐") << tr("右对齐");
    hAlignProperty_->setList(hAlignList);
    if(bShowContentText_)
        propList.insert(propList.end(), hAlignProperty_);

    // 垂直对齐
    vAlignProperty_ = new ListProperty(tr("水平对齐"));
    vAlignProperty_->setId(EL_V_ALIGN);
    QStringList vAlignList;
    vAlignList << tr("上对齐") << tr("居中对齐") << tr("下对齐");
    vAlignProperty_->setList(vAlignList);
    if(bShowContentText_)
        propList.insert(propList.end(), vAlignProperty_);

    // 图片
    fileProperty = new FileProperty(trUtf8("选择图片"));
    fileProperty->setId(EL_FILE);
    fileProperty->setValue(filePicture_);
    if(!bShowContentText_)
        propList.insert(propList.end(), fileProperty);

    // 按钮背景颜色
    backgroundColorProperty_ = new ColorProperty(tr("背景颜色"));
    backgroundColorProperty_->setId(EL_BACKGROUND);
    backgroundColorProperty_->setValue(backgroundColor_);
    if(bShowContentText_)
        propList.insert(propList.end(), backgroundColorProperty_);

    // 透明
    transparentProperty_ = new BoolProperty(tr("透明显示"));
    transparentProperty_->setId(EL_TRANSPARENT_BACKGROUND);
    transparentProperty_->setTrueText(tr("透明"));
    transparentProperty_->setFalseText(tr("不透明"));
    transparentProperty_->setValue(transparent_);
    propList.insert(propList.end(), transparentProperty_);

    // 字体
    fontProperty_ = new FontProperty(tr("字体"));
    fontProperty_->setId(EL_FONT);
    fontProperty_->setValue(QFont("Arial Black", 12));
    if(bShowContentText_)
        propList.insert(propList.end(), fontProperty_);

    // 文本颜色
    textColorProperty = new ColorProperty(trUtf8("颜色"));
    textColorProperty->setId(EL_FONT_COLOR);
    if(bShowContentText_)
        propList.insert(propList.end(), textColorProperty);

    // 旋转角度
    angleProperty = new IntegerProperty(trUtf8("角度"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0, 360);
    propList.insert(propList.end(), angleProperty);

    // 初始有效性
    enableOnInitialProperty_ = new BoolProperty(tr("初始有效性"));
    enableOnInitialProperty_->setId(EL_ENABLE_ON_INITIAL);
    enableOnInitialProperty_->setTrueText(tr("有效"));
    enableOnInitialProperty_->setFalseText(tr("失效"));
    enableOnInitialProperty_->setValue(enableOnInitial_);
    propList.insert(propList.end(), enableOnInitialProperty_);

    // 初始可见性
    showOnInitialProperty_ = new BoolProperty(tr("初始可见性"));
    showOnInitialProperty_->setId(EL_SHOW_ON_INITIAL);
    showOnInitialProperty_->setTrueText(tr("显示"));
    showOnInitialProperty_->setFalseText(tr("不显示"));
    showOnInitialProperty_->setValue(showOnInitial_);
    propList.insert(propList.end(), showOnInitialProperty_);

    // 选择功能
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
        // 属性集发生改变需要更新属性表
        updatePropertyTableView();
        break;
    case EL_FONT:
        font_ = value.value<QFont>();
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
        QFileInfo infoSrc(filePicture_);
        if(infoSrc.exists()) {
            QString picturePath = getProjectPath() + "/Pictures";
            QDir dir(picturePath);
            if(!dir.exists())
                dir.mkpath(picturePath);
            QString fileDes = picturePath + "/" + infoSrc.fileName();
            QFileInfo infoDes(fileDes);
            if(!infoDes.exists())
                QFile::copy(filePicture_, fileDes);
            filePicture_ = infoSrc.fileName();
            updatePropertyModel();
        }
    }break;
    case EL_BACKGROUND:
        backgroundColor_ = value.value<QColor>();
        break;
    case EL_TRANSPARENT_BACKGROUND:
        transparent_ = value.toBool();
        break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_ANGLE:
        elemAngle = value.toInt();
        setAngle(elemAngle);
        break;
    case EL_ENABLE_ON_INITIAL:
        enableOnInitial_ = value.toBool();
        break;
    case EL_SHOW_ON_INITIAL:
        showOnInitial_ = value.toBool();
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
    backgroundColorProperty_->setValue(backgroundColor_);
    transparentProperty_->setValue(transparent_);
    fontProperty_->setValue(font_);
    textColorProperty->setValue(textColor);
    angleProperty->setValue(elemAngle);
    enableOnInitialProperty_->setValue(enableOnInitial_);
    showOnInitialProperty_->setValue(showOnInitial_);
    funcProperty->setValue(funcs_);
}

void ElementPushButton::reloadPropertyList()
{
    propList.clear();

    // ID
    propList.insert(propList.end(), idProperty);
    // 标题
    propList.insert(propList.end(), titleProperty);
    // 坐标 X
    propList.insert(propList.end(), xCoordProperty);
    //坐标 Y
    propList.insert(propList.end(), yCoordProperty);
    // Z 值
    propList.insert(propList.end(), zValueProperty);
    // 宽度
    propList.insert(propList.end(), widthProperty_);
    // 高度
    propList.insert(propList.end(), heightProperty_);
    // 显示内容
    propList.insert(propList.end(), showContentProperty_);

    if(bShowContentText_) {
        // 文本
        propList.insert(propList.end(), elementTextProperty);
        // 水平对齐
        propList.insert(propList.end(), hAlignProperty_);
        // 垂直对齐
        propList.insert(propList.end(), vAlignProperty_);
    }
    // 图片
    if(!bShowContentText_)
        propList.insert(propList.end(), fileProperty);
    // 按钮背景颜色
    if(bShowContentText_)
        propList.insert(propList.end(), backgroundColorProperty_);
    // 透明
    propList.insert(propList.end(), transparentProperty_);

    if(bShowContentText_) {
        // 字体
        propList.insert(propList.end(), fontProperty_);
        // 文本颜色
        propList.insert(propList.end(), textColorProperty);
    }

    // 旋转角度
    propList.insert(propList.end(), angleProperty);
    // 初始有效性
    propList.insert(propList.end(), enableOnInitialProperty_);
    // 初始可见性
    propList.insert(propList.end(), showOnInitialProperty_);
    // 选择功能
    propList.insert(propList.end(), funcProperty);
}

/**
 * @brief ElementPushButton::updatePropertyTableView
 * @details 更新属性表
 */
void ElementPushButton::updatePropertyTableView()
{
    PropertyModel *pModel = showContentProperty_->getPropertyModel();
    if(pModel != nullptr) {
        pModel->resetModel();
        updatePropertyModel();
        reloadPropertyList();
        QListIterator<Property*> iter(getPropertyList());
        while (iter.hasNext()) {
            pModel->addProperty(iter.next());
        }
    }
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

    if(transparent_) {
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
    } else {
        for(int i=0; i<borderWidth; i++) {
            PubTool::DrawFrameRect(painter, rect, borderColor);
            if(i<borderWidth/2) rect.adjust(1, 1, -1, -1);
            else rect.adjust(1, 1, 0, 0);
        }

        PubTool::DrawFrameRect(painter, rect, QColor(252, 252, 252));
        rect.adjust(1, 1, -1, -1);
        rect.adjust(-1, -1, 0, 0);

        if(bShowContentText_) { // 文本+背景
            PubTool::FillFullRect(painter, rect, backgroundColor_);
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
            QRectF textRect = rect.normalized().adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);
            painter->drawText(textRect, hFlags|vFlags, elementText);

        } else { // 图片按钮
            if(filePicture_ != QString()) {
                QString picture = getProjectPath() + "/Pictures/" + filePicture_;
                if(QFile::exists(picture)) {
                    QImage image(getProjectPath() + "/Pictures/" + filePicture_);
                    QImage scaleImage = image.scaled((int)elementRect.width(), (int)elementRect.height(), Qt::IgnoreAspectRatio);
                    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
                    painter->drawImage(elementRect, scaleImage);
                }
            }
        }
    }
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
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        QPointF pos = scenePos();

        if(pos.x() < 0) {
            this->setX(0);
        }
        if(pos.x() > iGraphPageWidth_ - getElementWidth()) {
            this->setX(iGraphPageWidth_ - getElementWidth());
        }

        if(pos.y() < 0) {
            this->setY(0);
        }
        if(pos.y() > iGraphPageHeight_ - getElementHeight()) {
            this->setY(iGraphPageHeight_ - getElementHeight());
        }
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
    writer.writeAttribute("backgroundColor", backgroundColor_.name());
    writer.writeAttribute("transparent", transparent_?"true":"false");
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("fontsize", QString::number(fontSize));
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeAttribute("enableOnInitial", enableOnInitial_?"true":"false");
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
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
        bShowContentText_ = true;
        if(showContent_ == tr("图片")) {
            bShowContentText_ = false;
        }
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

    if (attributes.hasAttribute("backgroundColor")) {
        backgroundColor_ = QColor(attributes.value("backgroundColor").toString());
    }

    if (attributes.hasAttribute("transparent")) {
        QString value = attributes.value("transparent").toString();
        transparent_ = false;
        if(value == "true") {
            transparent_ = true;
        }
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
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

    if (attributes.hasAttribute("enableOnInitial")) {
        QString value = attributes.value("enableOnInitial").toString();
        enableOnInitial_ = false;
        if(value == "true") {
            enableOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("functions")) {
        QString listString = attributes.value("functions").toString();
        funcs_ = listString.split('|');
    }

    updateBoundingElement();
    updatePropertyModel();

    reloadPropertyList();
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
        << this->backgroundColor_
        << this->transparent_
        << this->font_.toString()
        << this->textColor
        << this->fontSize
        << this->elemAngle
        << this->enableOnInitial_
        << this->showOnInitial_
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
    QColor backgroundColor;
    bool transparent;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
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
       >> backgroundColor
       >> transparent
       >> font
       >> textColor
       >> fontSize
       >> angle
       >> enableOnInitial
       >> showOnInitial
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
    this->backgroundColor_ = backgroundColor;
    this->transparent_ = transparent;
    this->textColor = textColor;
    this->fontSize = fontSize;
    this->setAngle(angle);
    this->enableOnInitial_ = enableOnInitial;
    this->showOnInitial_ = showOnInitial;
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
        << ele.backgroundColor_
        << ele.transparent_
        << ele.font_
        << ele.textColor
        << ele.fontSize
        << ele.elemAngle
        << ele.enableOnInitial_
        << ele.showOnInitial_
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
    QColor backgroundColor;
    bool transparent;
    QString text;
    QString hAlign;
    QString vAlign;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
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
       >> backgroundColor
       >> transparent
       >> font
       >> textColor
       >> fontSize
       >> angle
       >> enableOnInitial
       >> showOnInitial
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
    ele.backgroundColor_ = backgroundColor;
    ele.transparent_ = transparent;
    ele.font_ = font;
    ele.textColor = textColor;
    ele.fontSize = fontSize;
    ele.setAngle(angle);
    ele.enableOnInitial_ = enableOnInitial;
    ele.showOnInitial_ = showOnInitial;
    ele.funcs_ = funcs;
    ele.updateBoundingElement();
    ele.updatePropertyModel();

    return in;
}

