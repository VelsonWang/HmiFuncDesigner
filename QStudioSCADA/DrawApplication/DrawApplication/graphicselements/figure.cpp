#include "figure.h"
#include <QtDebug>

/**
Calling updatePropertyModel() for properties which can change
without using PropertyEditor - height, width, xpos,ypos,
zvalue (using context menu on the scene)
*/

Figure::~Figure() {

}

void Figure::setElementId(const QString &id) {
    elementId = id;
}

QString Figure::getElementId() const {
    return elementId;
}

QString Figure::getInternalElementType() const {

    return internalElementType;
}

QIcon Figure::getElementIcon() const {
    return elementIcon;
}

void Figure::setElementWidth(int width) {
    elementWidth = width;
    updatePropertyModel();
}

int Figure::getElementWidth() const {
    return elementWidth;
}

void Figure::setElementHeight(int height) {
    elementHeight = height;
    updatePropertyModel();
}

int Figure::getElementHeight() const {
    return elementHeight;
}

void Figure::setElementXPos(int xpos) {
    elementXPos = xpos;
    setX(elementXPos);
    updatePropertyModel();
}

int Figure::getElementXPos() const {
    return elementXPos;
}

void Figure::setElementYPos(int ypos) {
    elementYPos = ypos;
    setY(elementYPos);
    updatePropertyModel();
}

int Figure::getElementYPos() const {
    return elementYPos;
}

void Figure::setGraphPageLink(const QString &link) {
    graphPageLink = link;
}

QString Figure::getGraphPageLink() const {
    return graphPageLink;
}

void Figure::setMessageType(const QString &msgType) {
    messageType = msgType;
}

QString Figure::getMessageType() const {
    return messageType;
}

void Figure::setElementZValue(int zval) {
    elementZValue = zval;
    setZValue(elementZValue);
    updatePropertyModel();
}

int Figure::getElementZValue() const {
    return elementZValue;
}

void Figure::setIndicationRule(const QString &rule) {
    indicationRule = rule;
}

QString Figure::getIndicationRule() const {
    return indicationRule;
}

void Figure::setLinkingType(const QString &mLinkingType) {
    linkingType = mLinkingType;
}

QString Figure::getLinkingType() const {
    return linkingType;
}

void Figure::setDeviceLink(const QString &mDeviceLink) {
    deviceLink = mDeviceLink;
}

QString Figure::getDeviceLink() const {
    return deviceLink;
}

void Figure::setSignalLink(const QString &mSignalLink) {
    signalLink = mSignalLink;
}

QString Figure::getSignalLink() const {
    return signalLink;
}

QList <Property*> Figure::getPropertyList() const {
    return propList;
}

void Figure::moveTo(int x, int y) {

    elementXPos = elementXPos + x;
    elementYPos = elementYPos + y;

    setPos(elementXPos,elementYPos);
    updatePropertyModel();
}

void Figure::setAngle(qreal angle_) {

    QTransform transform;
    transform.rotate(angle_);
    elemAngle = angle_;
    setTransform(transform);
}

qreal Figure::angle() const {
    return elemAngle;
}

void Figure::init() {

    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);

    resizing = false;

    elementWidth = 70;
    elementHeight = 70;
    elementZValue = zValue();
    backgroundColor = QColor(Qt::blue);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 2;
    borderColor = QColor(Qt::black);
    elementText = trUtf8("文本");
    fontSize = 10;

    elemAngle = 0;
    rotationCount = 0;
    block = false;

    setSelected(true);

    rd = RdNone;
}

void Figure::setBlocked(bool blocked) {

    setFlag(QGraphicsItem::ItemIsMovable,!blocked);
}

void Figure::addNodePoint() {
}

void Figure::removeNodePoint() {
}
