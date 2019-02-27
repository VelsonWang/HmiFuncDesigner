#include "Element.h"
#include <QtDebug>

/**
Calling updatePropertyModel() for properties which can change
without using PropertyEditor - height, width, xpos,ypos,
zvalue (using context menu on the scene)
*/

Element::~Element() {

}

void Element::setElementId(const QString &id) {
    elementId = id;
}

QString Element::getElementId() const {
    return elementId;
}

QString Element::getInternalElementType() const {

    return internalElementType;
}

QIcon Element::getElementIcon() const {
    return elementIcon;
}

void Element::setElementWidth(int width) {
    elementWidth = width;
    updatePropertyModel();
}

int Element::getElementWidth() const {
    return elementWidth;
}

void Element::setElementHeight(int height) {
    elementHeight = height;
    updatePropertyModel();
}

int Element::getElementHeight() const {
    return elementHeight;
}

void Element::setElementXPos(int xpos) {
    elementXPos = xpos;
    setX(elementXPos);
    updatePropertyModel();
}

int Element::getElementXPos() const {
    return elementXPos;
}

void Element::setElementYPos(int ypos) {
    elementYPos = ypos;
    setY(elementYPos);
    updatePropertyModel();
}

int Element::getElementYPos() const {
    return elementYPos;
}

void Element::setGraphPageLink(const QString &link) {
    graphPageLink = link;
}

QString Element::getGraphPageLink() const {
    return graphPageLink;
}

void Element::setMessageType(const QString &msgType) {
    messageType = msgType;
}

QString Element::getMessageType() const {
    return messageType;
}

void Element::setElementZValue(int zval) {
    elementZValue = zval;
    setZValue(elementZValue);
    updatePropertyModel();
}

int Element::getElementZValue() const {
    return elementZValue;
}

void Element::setIndicationRule(const QString &rule) {
    indicationRule = rule;
}

QString Element::getIndicationRule() const {
    return indicationRule;
}

void Element::setLinkingType(const QString &mLinkingType) {
    linkingType = mLinkingType;
}

QString Element::getLinkingType() const {
    return linkingType;
}

void Element::setDeviceLink(const QString &mDeviceLink) {
    deviceLink = mDeviceLink;
}

QString Element::getDeviceLink() const {
    return deviceLink;
}

void Element::setSignalLink(const QString &mSignalLink) {
    signalLink = mSignalLink;
}

QString Element::getSignalLink() const {
    return signalLink;
}

QList <Property*> Element::getPropertyList() const {
    return propList;
}

void Element::moveTo(int x, int y) {

    elementXPos = elementXPos + x;
    elementYPos = elementYPos + y;

    setPos(elementXPos,elementYPos);
    updatePropertyModel();
}

void Element::setAngle(qreal angle_) {

    QTransform transform;
    transform.rotate(angle_);
    elemAngle = angle_;
    setTransform(transform);
}

qreal Element::angle() const {
    return elemAngle;
}

void Element::init() {

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

void Element::setBlocked(bool blocked) {

    setFlag(QGraphicsItem::ItemIsMovable,!blocked);
}

void Element::addNodePoint() {
}

void Element::removeNodePoint() {
}
