#include "Element.h"
#include <QtDebug>

/**
Calling updatePropertyModel() for properties which can change
without using PropertyEditor - height, width, xpos,ypos,
zvalue (using context menu on the scene)
*/

Element::Element(const QString &projPath) :
    elementId(""),
    strProjectPath_(projPath) {
}

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

void Element::setElementZValue(int zval) {
    elementZValue = zval;
    setZValue(elementZValue);
    updatePropertyModel();
}

int Element::getElementZValue() const {
    return elementZValue;
}

QList <Property*> Element::getPropertyList() const {
    return propList;
}

void Element::moveTo(int x, int y) {
    elementXPos = elementXPos + x;
    elementYPos = elementYPos + y;
    setPos(elementXPos, elementYPos);
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

    elementWidth = 80;
    elementHeight = 32;
    elementZValue = zValue();
    backgroundColor = QColor(Qt::white);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 2;
    borderColor = QColor(Qt::black);
    elementText = trUtf8("文本");
    fontSize = 10;

    elemAngle = 0;
    rotationCount = 0;

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

/**
 * @brief Element::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void Element::setProjectPath(const QString &path) {
    if(strProjectPath_ != path)
        strProjectPath_ = path;
}


/**
 * @brief Element::getProjectPath
 * @details 获取工程路径
 * @return 工程路径
 */
QString Element::getProjectPath() const {
    return strProjectPath_;
}


/**
 * @brief Element::getIndexFromIDString
 * @details 控件唯一标识字符串，形如："PushButton_0001"
 * @param szID 控件唯一标识
 * @return 分配的索引值
 */
int Element::getIndexFromIDString(const QString &szID) {
    int pos = szID.indexOf("_");
    if(pos > -1) {
        QString szIndex = szID.right(4);
        bool ok = false;
        int iRet = szIndex.toInt(&ok);
        if(!ok) {
            return 0;
        }
        return iRet;
    }
    return 0;
}

/**
 * @brief Element::getHAlignString
 * @param szAlign 对齐方式
 * @return 水平方向对齐方式
 */
QString Element::getHAlignString(const QString& szAlign) const {
    if(szAlign == tr("左对齐")) {
        return QString("left");
    } else if(szAlign == tr("居中对齐")) {
        return QString("center");
    } else if(szAlign == tr("右对齐")) {
        return QString("right");
    }
    return QString("");
}


/**
 * @brief Element::setHAlignString
 * @details 设置水平方向对齐方式
 * @param szAlign 水平方向对齐方式
 * @param szAlignSet 待设置垂直方向对齐方式
 */
void Element::setHAlignString(const QString& szAlign, QString& szAlignSet) {
    if(szAlign == QString("left")) {
        szAlignSet = tr("左对齐");
    } else if(szAlign == QString("center")) {
        szAlignSet = tr("居中对齐");
    } else if(szAlign == QString("right")) {
        szAlignSet = tr("右对齐");
    }
}


/**
 * @brief Element::getVAlignString
 * @param szAlign 对齐方式
 * @return 垂直方向对齐方式
 */
QString Element::getVAlignString(const QString& szAlign) const {
    if(szAlign == tr("上对齐")) {
        return QString("top");
    } else if(szAlign == tr("居中对齐")) {
        return QString("center");
    } else if(szAlign == tr("下对齐")) {
        return QString("bottom");
    }
    return QString("");
}

/**
 * @brief Element::setVAlignString
 * @details 设置垂直方向对齐方式
 * @param szAlign 垂直方向对齐方式
 * @param szAlignSet 待设置垂直方向对齐方式
 */
void Element::setVAlignString(const QString& szAlign, QString& szAlignSet) {
    if(szAlign == QString("top")) {
        szAlignSet = tr("上对齐");
    } else if(szAlign == QString("center")) {
        szAlignSet = tr("居中对齐");
    } else if(szAlign == QString("bottom")) {
        szAlignSet = tr("下对齐");
    }
}


/**
 * @brief Element::setGraphPageSize
 * @details 设置所属画面大小
 * @param width 画面宽度
 * @param height 画面高度
 */
void Element::setGraphPageSize(int width, int height) {
    this->iGraphPageWidth_ = width;
    this->iGraphPageHeight_ = height;
}
