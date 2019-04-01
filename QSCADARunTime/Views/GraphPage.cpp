#include "GraphPage.h"
#include <cmath>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include "ElementGroup.h"
#include "IDrawGraphPage.h"

#include "EArrow/EArrow.h"
#include "ELine/ELine.h"
#include "EEllipse/EEllipse.h"

template<template<typename T> class S, typename T>
T min(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T minimum = sequence.first();
    foreach (const T &item, sequence)
        if (item < minimum)
            minimum = item;
    return minimum;
}

template<template<typename T> class S, typename T>
T max(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T maximum = sequence.first();
    foreach (const T &item, sequence)
        if (item > maximum)
            maximum = item;
    return maximum;
}


static QMap<QString, CreateObjFunc> mapNameFuncData_;
static QMap<int, CreateObjFunc> mapIDFuncData_;
static QMap<QString, CreateObjFunc> mapIDStringFuncData_;

#define REGISTER_CREATEOR(elementName, elementID, elementIDString, className) \
do{ \
    mapNameFuncData_[elementName] = className::creatObjFunc; \
    mapIDFuncData_[elementID] = className::creatObjFunc; \
    mapIDStringFuncData_[elementIDString] = className::creatObjFunc; \
    }while(0)

/**
 * @brief registerCreateObjectFunc
 * @details 注册元素对象创建函数
 */
void registerCreateObjectFunc() {

    mapNameFuncData_.clear();
    mapIDFuncData_.clear();
    mapIDStringFuncData_.clear();

    REGISTER_CREATEOR(QObject::trUtf8("箭头"), ArrowItemType, "Arrow", EArrow);
    REGISTER_CREATEOR(QObject::trUtf8("直线"), LineItemType, "Line", ELine);
    REGISTER_CREATEOR(QObject::trUtf8("椭圆形"), EllipseItemType, "Ellipse", EEllipse);



}

GraphPage::GraphPage(const QRectF &rect, QObject *parent)
    : QGraphicsScene(parent),
      filename(QString()),
      unsavedFlag(false),
      projpath_(QString())
{
    setItemIndexMethod(QGraphicsScene::NoIndex);

    if (rect.width() == 0 || rect.height() == 0) {
        setSceneRect(0, 0, 800, 480);
    }
    else {
        setSceneRect(rect);
    }

    gridVisible = false;

    GraphPageWidth = sceneRect().width();
    GraphPageHeight = sceneRect().height();
    GraphPagePriority.clear();
    GraphPagePriority.append(trUtf8("主要的"));
    GraphPageBackground = QColor(Qt::white);

    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(20);

    createPropertyList();
}

void GraphPage::setActive(bool active) {

    onActive = active;
}

bool GraphPage::active() {

    return onActive;
}

bool GraphPage::getUnsavedFlag() {

    return unsavedFlag;
}

void GraphPage::fillGridPixmap() {

    gridPixmap = QPixmap(QSize(sceneRect().width(), sceneRect().height()));
    gridPixmap.fill(GraphPageBackground);

    QPainter painter(&gridPixmap);

    const int maxX = static_cast<int>(std::ceil(width())/gridSize)*gridSize;
    const int maxY = static_cast<int>(std::ceil(height())/gridSize)*gridSize;

    if (GraphPageBackground == Qt::black) {
        painter.setPen(Qt::white);
        painter.setBrush(Qt::NoBrush);
    }
    else {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
    }

    for (int i = 0; i < maxX; i += gridSize) {
        for (int j = 0; j < maxY; j += gridSize) {
            painter.drawPoint(i,j);
        }
    }
}

void GraphPage::setGridVisible(bool on) {

    gridVisible = on;

    if (gridVisible) {
        fillGridPixmap();
    }
    else {
        gridPixmap = QPixmap(QSize(sceneRect().width(), sceneRect().height()));
        gridPixmap.fill(GraphPageBackground);
    }

    invalidate(sceneRect(), BackgroundLayer);
}

bool GraphPage::isGridVisible() const {

    return gridVisible;
}

void GraphPage::createPropertyList() {

    idProperty = new TextProperty(trUtf8("ID"));
    idProperty->setId(GRAPHPAGE_ID);
    propList.insert(propList.end(),idProperty);

    titleProperty = new EmptyProperty(trUtf8("基本属性"));
    propList.insert(propList.end(),titleProperty);

    backgroundProperty = new ColorProperty(trUtf8("背景"));
    backgroundProperty->setId(GRAPHPAGE_BACKGROUND);
    propList.insert(propList.end(),backgroundProperty);

    widthProperty = new IntegerProperty(trUtf8("宽度"));
    widthProperty->setId(GRAPHPAGE_WIDTH);
    widthProperty->setSettings(0, 5000);
    propList.insert(propList.end(),widthProperty);

    heightProperty = new IntegerProperty(trUtf8("高度"));
    heightProperty->setId(GRAPHPAGE_HEIGHT);
    heightProperty->setSettings(0, 5000);
    propList.insert(propList.end(), heightProperty);
}


void GraphPage::slotAddNodePoint() {

    Element *ele = static_cast<Element *>(selectedItems().first());
    ele->addNodePoint();
}

void GraphPage::slotRemoveNodePoint() {

    Element *ele = static_cast<Element *>(selectedItems().first());
    ele->removeNodePoint();
}

void GraphPage::slotShowAdditionalProperties() {

    //emit signalShowAddProperties();
}

void GraphPage::drawBackground(QPainter *painter, const QRectF &rect) {

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(GraphPageBackground);
    painter->drawRect(rect);

    if (gridVisible) {
        painter->drawPixmap(0, 0, gridPixmap);
    }

    painter->restore();
}

QUndoStack *GraphPage::undoStack() const {
    return m_undoStack;
}

void GraphPage::setFileName(const QString &file) {
    filename = file;
}

QString GraphPage::getFileName() const {
    return filename;
}

void GraphPage::setGraphPageId(const QString &id) {
    GraphPageId = id;
    emit changeGraphPageName();
}

QString GraphPage::getGraphPageId() const {
    return GraphPageId;
}

void GraphPage::setGraphPageBackground(const QColor &color) {
    GraphPageBackground = color;
    setBackgroundBrush(GraphPageBackground);
}

QColor GraphPage::getGraphPageBackground() const {
    return GraphPageBackground;
}

int GraphPage::getGraphPageWidth() const {
    return GraphPageWidth;
}

void GraphPage::setGraphPageWidth(int width) {
    GraphPageWidth = width;
    setSceneRect(0, 0, GraphPageWidth, GraphPageHeight);
}

int GraphPage::getGraphPageHeight() const {
    return GraphPageHeight;
}

void GraphPage::setGraphPageHeight(int height) {
    GraphPageHeight = height;
    setSceneRect(0, 0, GraphPageWidth, GraphPageHeight);
}

void GraphPage::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {

    if (event->mimeData()->hasText()) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        event->setAccepted(true);
    }
    else if (event->mimeData()->hasFormat("rti/lib")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        event->setAccepted(true);
    }
    else {
        event->setAccepted(false);
    }
}

void GraphPage::dropEvent(QGraphicsSceneDragDropEvent *event) {

    QPointF position = event->scenePos();

    if (event->mimeData()->hasFormat(QString("rti/lib"))) {
        QByteArray ba = event->mimeData()->data("rti/lib");
        loadLibrary(ba);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }

    if (event->mimeData()->hasText()) {
        createItems(event->mimeData()->text(),position);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void GraphPage::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {

    if (event->mimeData()->hasText()) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void GraphPage::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QGraphicsScene::mousePressEvent(event);

    if (!itemAt(event->scenePos(), QTransform())) {

    }
}

void GraphPage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

    QGraphicsScene::mouseDoubleClickEvent(event);

    if (!itemAt(event->scenePos(), QTransform())) {

    }
}

void GraphPage::createItems(const QString &typeId, QPointF position) {

    Element *last = 0;

    CreateObjFunc func = mapNameFuncData_[typeId];
    IDrawGraphPage *pEleObj = dynamic_cast<IDrawGraphPage *>(func());
    if(pEleObj != nullptr) {
        Element *ele = pEleObj->createElement();
        if(ele != Q_NULLPTR) {
            ele->setProjectPath(projpath_);
            ele->setClickPosition(position);
            last = ele;
        }
    }

    if (last) {
        QList <QGraphicsItem*> addList;
        addList.push_back(last);
        m_undoStack->push(new AddCommand(addList,this));
        emit newElementAdded();
    }
}

void GraphPage::slotGroupElements() {

    /** Trash*/
    if (selectedItems().isEmpty()) {
        return;
    }

    ElementGroup *group = new ElementGroup;

    foreach (QGraphicsItem *item, selectedItems()) {
        group->addToGroup(item);
    }

    group->setClickPosition(QPointF(500,500));
    group->setHandlesChildEvents(false);
    addItem(group);
}

void GraphPage::slotUngroupElements() {

}

void GraphPage::slotAlignElements() {

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(
            action->data().toInt());

    QList<QGraphicsItem*> items = selectedItems();
    QVector<double> coordinates;
    populateCoordinates(alignment, &coordinates, items);
    double offset;

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop) {
        offset = min(coordinates);
    }
    else
    {
        offset = max(coordinates);
    }

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight) {
        for (int i = 0; i < items.count(); ++i)
            ((Element *)items.at(i))->moveTo(offset - coordinates.at(i), 0);
    }
    else {
        for (int i = 0; i < items.count(); ++i)
            ((Element *)items.at(i))->moveTo(0, offset - coordinates.at(i));
    }
}

void GraphPage::populateCoordinates(const Qt::Alignment &alignment,
        QVector<double> *coordinates,
        const QList<QGraphicsItem*> &items)
{
    QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext()) {
        QRectF rect = i.next()->sceneBoundingRect();
        switch (alignment) {
            case Qt::AlignLeft:
                coordinates->append(rect.x()); break;
            case Qt::AlignRight:
                coordinates->append(rect.x() + rect.width()); break;
            case Qt::AlignTop:
                coordinates->append(rect.y()); break;
            case Qt::AlignBottom:
                coordinates->append(rect.y() + rect.height()); break;
        }
    }
}

void GraphPage::slotFrontPlanElements() {

    foreach (QGraphicsItem *item,selectedItems()) {
        Element *ele = (Element *)item;
        ele->setElementZValue(ele->getElementZValue() + 1);
    }
}

void GraphPage::slotBehindPlanElements() {

    foreach (QGraphicsItem *item,selectedItems()) {
        Element *ele = (Element*)item;
        ele->setElementZValue(ele->getElementZValue() - 1);
    }
}


void GraphPage::removeElementEvent() {
    emit elementsDeleted();
}

void GraphPage::addElementEvent() {
    emit newElementAdded();
}

void GraphPage::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Up) {
        moveSelectedElements(0,-10);
    }
    else if (event->key() == Qt::Key_Down) {
        moveSelectedElements(0,10);
    }
    else if (event->key() == Qt::Key_Left) {
        moveSelectedElements(-10,0);
    }
    else if (event->key() == Qt::Key_Right) {
        moveSelectedElements(10,0);
    }
}

void GraphPage::moveSelectedElements(int xOffset, int yOffset) {

    foreach (QGraphicsItem *item,items()) {
        Element *ele = dynamic_cast<Element *>(item);

        if (ele) {
            ele->moveTo(xOffset, yOffset);
        }
    }
}

void GraphPage::readItems(QDataStream &in, int offset, bool select) {

    int objectType;
    int itemsCount;
    copyList.clear();

    in >> itemsCount;

    for (int i = 0; i < itemsCount; i++) {
        in >> objectType;
        CreateObjFunc func = mapIDFuncData_[objectType];
        IDrawGraphPage *pEleObj = dynamic_cast<IDrawGraphPage *>(func());
        if(pEleObj != nullptr) {
            Element *ele = pEleObj->createElement();
            if(ele != Q_NULLPTR) {
                ele->setProjectPath(projpath_);
                ele->readData(in);
                copyList.insert(copyList.end(), ele);
            }
        }
    } //for

    foreach (QGraphicsItem *item, copyList) {
        Element *ele = (Element *)item;
        ele->setSelected(select);
        ele->moveTo(offset,offset);
    }

    m_undoStack->push(new AddCommand(copyList,this));
}

void GraphPage::loadAsBinary(const QString &filename) {

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,
                                 trUtf8("错误"),
                                 trUtf8("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream in(&file);

    in >> *this;

    readItems(in,0,false);

    file.close();
}

void GraphPage::loadAsXML(const QString &filename) {

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,
                                 trUtf8("错误"),
                                 trUtf8("无法打开文件"),
                                 QMessageBox::Ok);
        return;
    }

    readGraphPageConfig(file);
    addElementEvent();

    file.close();
}

void GraphPage::readGraphPageConfig(QFile &file) {

    QXmlStreamReader reader;
    reader.setDevice(&file);

    while (!reader.atEnd() && !reader.hasError()) {

        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (reader.name() == "graphPage") {
                readGraphPageTag(reader);
            }
        }
    }
}

void GraphPage::readGraphPageTag(QXmlStreamReader &xml) {

    setGraphPageAttributes(xml);
    copyList.clear();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "graphPage")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "element") {

                if (xml.attributes().hasAttribute("internalType")) {
                    Element *ele = createElement(xml.attributes().value("internalType").toString());
                    if (ele) {
                        ele->setProjectPath(projpath_);
                        ele->readFromXml(xml.attributes());
                        addItem(ele);
                        ele->setSelected(false);
                    }
                }
            }
        }

        xml.readNext();
    }
}

void GraphPage::setGraphPageAttributes(QXmlStreamReader &xml) {

    if (xml.attributes().hasAttribute("fileName")) {
        setFileName(xml.attributes().value("fileName").toString());
    }

    if (xml.attributes().hasAttribute("graphPageId")) {
        setGraphPageId(xml.attributes().value("graphPageId").toString());
    }

    if (xml.attributes().hasAttribute("width")) {
        setGraphPageWidth(xml.attributes().value("width").toString().toInt());
    }

    if (xml.attributes().hasAttribute("height")) {
        setGraphPageHeight(xml.attributes().value("height").toString().toInt());
    }

    if (xml.attributes().hasAttribute("background")) {
        setGraphPageBackground(QColor(xml.attributes().value("background").toString()));
    }

    fillGridPixmap();
}

Element *GraphPage::createElement(const QString &internalType) {

    CreateObjFunc func = mapIDStringFuncData_[internalType];
    IDrawGraphPage *pEleObj = dynamic_cast<IDrawGraphPage *>(func());
    if(pEleObj != nullptr) {
        return pEleObj->createElement();
    }
    return nullptr;
}


void GraphPage::loadLibrary(QByteArray &data) {

    QDataStream in(&data,QIODevice::ReadOnly);
    QString filename;
    in >> filename;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,
                                 trUtf8("错误"),
                                 trUtf8("无法打开文件"),
                                 QMessageBox::Ok);
        return;
    }

    readLibraryConfig(file);
    addElementEvent();

    file.close();
}

void GraphPage::readLibraryConfig(QFile &file) {

    QXmlStreamReader reader;
    reader.setDevice(&file);

    while (!reader.atEnd() && !reader.hasError()) {

        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (reader.name() == "Library") {
                readLibraryTag(reader);
            }
        }
    }
}

void GraphPage::readLibraryTag(QXmlStreamReader &xml) {

    copyList.clear();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Library")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "element") {

                if (xml.attributes().hasAttribute("internalType")) {
                    Element *ele = createElement(xml.attributes().value("internalType").toString());
                    if (ele) {
                        ele->setProjectPath(projpath_);
                        ele->readFromXml(xml.attributes());
                        addItem(ele);
                    }
                }
            }
        }

        xml.readNext();
    }
}

/**
 * @brief GraphPage::getProjectPath
 * @details 获取工程路径
 * @return 工程路径
 */
QString GraphPage::getProjectPath() const {
    return projpath_;
}


/**
 * @brief GraphPage::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void GraphPage::setProjectPath(const QString &path) {
    projpath_ = path;
}


QDataStream &operator<<(QDataStream &out,const GraphPage &GraphPage) {

    out << GraphPage.getFileName()
        << GraphPage.getGraphPageId()
        << GraphPage.getGraphPageBackground()
        << GraphPage.getGraphPageHeight()
        << GraphPage.getGraphPageWidth();

    return out;
}

QDataStream &operator>>(QDataStream &in,GraphPage &GraphPage) {

    QString filename;
    QString id;

    QColor backColor;
    int height;
    int width;

    in >> filename
            >> id
            >> backColor
            >> height
            >> width;

    GraphPage.setFileName(filename);
    GraphPage.setGraphPageId(id);
    GraphPage.setGraphPageWidth(width);
    GraphPage.setGraphPageHeight(height);
    GraphPage.setGraphPageBackground(backColor);
    GraphPage.fillGridPixmap();

    return in;
}

