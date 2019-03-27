#include "GraphPage.h"
#include <cmath>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include "xmlGraphPageconfigwriter.h"
#include "ElementGroup.h"
#include "IDrawApplicationPlugin.h"
#include "PluginManager.h"


/** Template algorithms*/
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

const QString MimeType = "rti/mnemodesigner";
/** !!*/


GraphPage::GraphPage(const QRectF &rect, QObject *parent)
    : QGraphicsScene(parent),
      propertyModel(0),
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
    createContextMenuActions();
    updateActions();

    connect(this, SIGNAL(selectionChanged()), SLOT(slotSelectionChanged()));
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

void GraphPage::setPropertyModel(PropertyModel *model) {

    propertyModel = model;
    fillGraphPagePropertyModel();

    connect(propertyModel, SIGNAL(onDataChangedByEditor(Property* )), SLOT(slotElementPropertyChanged(Property* )));
    connect(propertyModel, SIGNAL(onDataChangedByEditor(Property* )), SLOT(slotGraphPagePropertyChanged(Property* )));
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

void GraphPage::slotGraphPagePropertyChanged(Property *property) {

    if (!selectedItems().isEmpty()) {
        return;
    }

    if (!active()) {
        return;
    }

    switch (property->getId()) {

    case GRAPHPAGE_ID:
        setGraphPageId(property->getValue().toString());
        break;
    case GRAPHPAGE_PRIORITY:
        setGraphPagePriority(property->getValue().toString());
        break;
    case GRAPHPAGE_BACKGROUND:
        setGraphPageBackground(property->getValue().value<QColor>());
        break;
    case GRAPHPAGE_WIDTH:
        setGraphPageWidth(property->getValue().toInt());
        break;
    case GRAPHPAGE_HEIGHT:
        setGraphPageHeight(property->getValue().toInt());
        break;
    }

    fillGridPixmap();

    unsavedFlag = true;
    emit GraphPagePropertyChanged();
}

void GraphPage::cleanPropertyModel() {

    propertyModel->resetModel();
}

void GraphPage::fillGraphPagePropertyModel() {

    idProperty->setValue(GraphPageId);
    priorityProperty->setValue(GraphPagePriority);
    backgroundProperty->setValue(GraphPageBackground);
    widthProperty->setValue(GraphPageWidth);
    heightProperty->setValue(GraphPageHeight);

    propertyModel->resetModel();

    QListIterator <Property*> i(propList);

    while (i.hasNext()) {
        propertyModel->addProperty(i.next());
    }
}

void GraphPage::createPropertyList() {

    idProperty = new TextProperty(trUtf8("ID"));
    idProperty->setId(GRAPHPAGE_ID);
    propList.insert(propList.end(),idProperty);

    titleProperty = new EmptyProperty(trUtf8("标题"));
    propList.insert(propList.end(),titleProperty);

    priorityProperty = new EnumProperty(trUtf8("主要"));
    priorityProperty->setId(GRAPHPAGE_PRIORITY);
    priorityProperty->setLiterals(QStringList() << trUtf8("主要") << trUtf8("嵌入"));
    propList.insert(propList.end(),priorityProperty);

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

void GraphPage::createContextMenuActions() {

    inGroupAction = new QAction(QIcon(":/images/group.png"), trUtf8("分组"), &contextServiceMenu);
    connect(inGroupAction, SIGNAL(triggered()), SLOT(slotGroupElements()));

    outGroupAction = new QAction(QIcon(":/images/ungroup.png"), trUtf8("分类"), &contextServiceMenu);

    alignTopAction = new QAction(QIcon(":/images/align-top.png"), trUtf8("顶部对齐"), &contextServiceMenu);
    alignTopAction->setData(Qt::AlignTop);
    connect(alignTopAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    alignDownAction = new QAction(QIcon(":/images/align-bottom.png"), trUtf8("底部对齐"), &contextServiceMenu);
    alignDownAction->setData(Qt::AlignBottom);
    connect(alignDownAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    alignRightAction = new QAction(QIcon(":/images/align-right.png"), trUtf8("右对齐"), &contextServiceMenu);
    alignRightAction->setData(Qt::AlignRight);
    connect(alignRightAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    alignLeftAction = new QAction(QIcon(":/images/align-left.png"), trUtf8("左对齐"), &contextServiceMenu);
    alignLeftAction->setData(Qt::AlignLeft);
    connect(alignLeftAction,SIGNAL(triggered()),SLOT(slotAlignElements()));

    frontPlanAction = new QAction(QIcon(),trUtf8("正面图"),&contextServiceMenu);
    connect(frontPlanAction,SIGNAL(triggered()),SLOT(slotFrontPlanElements()));

    behindPlanAction = new QAction(QIcon(),trUtf8("背面图"),&contextServiceMenu);
    connect(behindPlanAction,SIGNAL(triggered()),SLOT(slotBehindPlanElements()));

    actionAdditional = new QAction(QIcon(":/images/additional.png"),trUtf8("额外"),&contextServiceMenu);
    connect(actionAdditional,SIGNAL(triggered()),SLOT(slotShowAdditionalProperties()));

    saveAsLibraryAction = new QAction(QIcon(":/images/library.png"),trUtf8("保存为库"),&contextMenu);
    connect(saveAsLibraryAction,SIGNAL(triggered()),SLOT(slotSaveAsLibrary()));

    actionRemoveNodePoint = new QAction(QIcon(":/images/delete.png"),trUtf8("删除节点"),&contextNodePointMenu);
    connect(actionRemoveNodePoint,SIGNAL(triggered()),SLOT(slotRemoveNodePoint()));

    actionAddNodePoint = new QAction(QIcon(":/images/add.png"),trUtf8("添加节点"),&contextNodePointMenu);
    connect(actionAddNodePoint,SIGNAL(triggered()),SLOT(slotAddNodePoint()));

    actionDelete = new QAction(QIcon(":/images/delete.png"),trUtf8("删除"),&contextMenu);
    actionDelete->setShortcut(QKeySequence::Delete);
    connect(actionDelete,SIGNAL(triggered()),SLOT(slotEditDelete()));

    actionCopy = new QAction(QIcon(":/images/editcopy.png"),trUtf8("拷贝"),&contextMenu);
    actionCopy->setShortcut(QKeySequence::Copy);
    connect(actionCopy,SIGNAL(triggered()),SLOT(slotEditCopy()));

    actionPaste = new QAction(QIcon(":/images/editpaste.png"),trUtf8("粘贴"),&contextMenu);
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste,SIGNAL(triggered()),SLOT(slotEditPaste()));

    actionSelectAll = new QAction(QIcon(),trUtf8("全选"),&contextMenu);
    actionSelectAll->setShortcut(QKeySequence::SelectAll);
    connect(actionSelectAll,SIGNAL(triggered()),SLOT(slotSelectAll()));

    contextServiceMenu.setTitle(trUtf8("工具"));
    contextServiceMenu.setIcon(QIcon(":/images/settings.png"));
    contextServiceMenu.addSeparator();
    contextServiceMenu.addAction(alignTopAction);
    contextServiceMenu.addAction(alignDownAction);
    contextServiceMenu.addAction(alignRightAction);
    contextServiceMenu.addAction(alignLeftAction);
    contextServiceMenu.addSeparator();
    contextServiceMenu.addAction(frontPlanAction);
    contextServiceMenu.addAction(behindPlanAction);
    contextServiceMenu.addSeparator();
    //contextServiceMenu.addAction(actionAdditional);

    contextNodePointMenu.setTitle(trUtf8("节点"));
    contextNodePointMenu.addAction(actionAddNodePoint);
    contextNodePointMenu.addAction(actionRemoveNodePoint);

    contextMenu.addMenu(&contextServiceMenu);
    contextMenu.addMenu(&contextNodePointMenu);
    contextMenu.addAction(saveAsLibraryAction);
    contextMenu.addAction(actionDelete);
    contextMenu.addAction(actionCopy);
    contextMenu.addAction(actionPaste);
    contextMenu.addAction(actionSelectAll);
}

void GraphPage::updateActions() {

    actionCopy->setEnabled(selectedItems().count() == 0 ? false : true);
    actionDelete->setEnabled(selectedItems().count() == 0 ? false : true);
    saveAsLibraryAction->setEnabled(selectedItems().count() == 0 ? false : true);
    actionAdditional->setEnabled(selectedItems().count() == 1 ? true : false);

    behindPlanAction->setEnabled(selectedItems().count() == 0 ? false : true);
    frontPlanAction->setEnabled(selectedItems().count() == 0 ? false : true);
    alignTopAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignDownAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignRightAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignLeftAction->setEnabled(selectedItems().count() < 2 ? false : true);

    if (selectedItems().isEmpty()) {
        contextNodePointMenu.setEnabled(false);
        return;
    }

    if (selectedItems().first()->type() == PolygonItemType) {
        contextNodePointMenu.setEnabled(true);
    }
    else {
        contextNodePointMenu.setEnabled(false);
    }
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

void GraphPage::slotElementPropertyChanged(Property *property) {

    if (selectedItems().isEmpty()) {
        return;
    }

    currentItem->updateElementProperty(property->getId(),property->getValue());

    unsavedFlag = true;
    emit elementPropertyChanged();

    if (property->getId() == EL_ID) {
        emit elementIdChanged();
    }
}

void GraphPage::slotSelectionChanged() {

    updateActions();

    if (selectedItems().isEmpty()) {
        propertyModel->resetModel();
        return;
    }

    propertyModel->resetModel();
    currentItem = dynamic_cast<Element *>(selectedItems().first());

    if (!currentItem) {
        return;
    }

    QListIterator <Property*> i(currentItem->getPropertyList());

    while (i.hasNext()) {
        propertyModel->addProperty(i.next());
    }
}

void GraphPage::slotElementMoved(QPointF oldPos) {

    Element *ele = (Element *)sender();

    m_undoStack->push(new ChangePositionCommand(ele, oldPos));
}

void GraphPage::slotElementResized(int width, int height,QPointF pos) {

    Element *ele = (Element *)sender();
    m_undoStack->push(new ChangeSizeCommand(ele, width, height, pos));
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

void GraphPage::setGraphPagePriority(const QString &priority) {
    GraphPagePriority =  priority;
}

QString GraphPage::getGraphPagePriority() const {
    return GraphPagePriority;
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
        fillGraphPagePropertyModel();
    }
}

void GraphPage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

    QGraphicsScene::mouseDoubleClickEvent(event);

    if (!itemAt(event->scenePos(), QTransform())) {
        fillGraphPagePropertyModel();
    }
}

void GraphPage::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {

    contextMenu.exec(event->screenPos());
}

void GraphPage::connectItem(Element *item) {

    connect(item, SIGNAL(elementMoved(QPointF)), SLOT(slotElementMoved(QPointF)));
    connect(item, SIGNAL(elementResized(int,int,QPointF)), SLOT(slotElementResized(int,int,QPointF)));
}

void GraphPage::createItems(const QString &typeId, QPointF position) {

    Element *last = 0;

    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > iter(PluginManager::getInstance()->plugins_);
    while(iter.hasNext())
    {
        iter.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  it(iter.value());
        while(it.hasNext())
        {
            it.next();
            IDrawApplicationPlugin *plugin = it.value();
            if(plugin != nullptr && plugin->getElementName() == typeId) {

                Element *ele = plugin->createElement();
                if(ele != Q_NULLPTR) {
                    ele->setProjectPath(projpath_);
                    ele->setClickPosition(position);
                    last = ele;
                    connectItem(ele);
                }
            }
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

void GraphPage::slotEditDelete() {

    m_undoStack->push(new RemoveCommand(selectedItems(),this));
    propertyModel->resetModel();
    emit elementsDeleted();
}

void GraphPage::removeElementEvent() {
    emit elementsDeleted();
}

void GraphPage::addElementEvent() {
    emit newElementAdded();
}

void GraphPage::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Delete) {

        if (!selectedItems().isEmpty()) {
            slotEditDelete();
        }
    }
    else if (event->matches(QKeySequence::Copy)) {
        slotEditCopy();
    }
    else if (event->matches(QKeySequence::Paste)) {
        slotEditPaste();
    }
    else if (event->matches(QKeySequence::SelectAll)) {
        slotSelectAll();
    }
    else if (event->key() == Qt::Key_Up) {
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

void GraphPage::slotSelectAll() {

    foreach (QGraphicsItem *item,items()) {
        item->setSelected(true);
    }
}

void GraphPage::slotEditCopy() {

    QList <QGraphicsItem*> selItems = selectedItems();

    if (selItems.isEmpty()) {
        return;
    }

    copyItems(selItems);
}

void GraphPage::copyItems(const QList<QGraphicsItem *> &items) {

    QByteArray copiedItems;
    QDataStream out(&copiedItems,QIODevice::WriteOnly);
    writeItems(out,items);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeType,copiedItems);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void GraphPage::slotEditPaste() {

    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (!mimeData) {
        return;
    }

    if (mimeData->hasFormat(MimeType)) {
        QByteArray copiedItems = mimeData->data(MimeType);
        QDataStream in(&copiedItems,QIODevice::ReadOnly);
        readItems(in,pasteOffset,true);
    }
}

void GraphPage::readItems(QDataStream &in, int offset, bool select) {

    int objectType;
    int itemsCount;
    copyList.clear();

    in >> itemsCount;

    for (int i = 0; i < itemsCount; i++) {

        in >> objectType;

        QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*>> iter(PluginManager::getInstance()->plugins_);
        while(iter.hasNext()) {
            iter.next();
            QMapIterator<QString, IDrawApplicationPlugin*> it(iter.value());
            while(it.hasNext()) {
                it.next();
                IDrawApplicationPlugin *plugin = it.value();
                if(plugin != nullptr && plugin->getElementID() == objectType) {

                    Element *ele = plugin->createElement();
                    if(ele != Q_NULLPTR) {
                        ele->setProjectPath(projpath_);
                        ele->readData(in);
                        connectItem(ele);
                        copyList.insert(copyList.end(), ele);
                    }
                }
            }
        }
    } //for

    foreach (QGraphicsItem *item,copyList) {
        Element *ele = (Element *)item;
        ele->setSelected(select);
        ele->moveTo(offset,offset);
    }

    m_undoStack->push(new AddCommand(copyList,this));
}

void GraphPage::writeItems(QDataStream &out, const QList<QGraphicsItem *> &items) {

    out << items.count();

    for (int j = 0; j < items.count(); j++) {

        int type = items[j]->type();
        items[j]->setSelected(false);

        out << type;

        QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > iter(PluginManager::getInstance()->plugins_);
        while(iter.hasNext()) {
            iter.next();
            QMapIterator<QString, IDrawApplicationPlugin*>  it(iter.value());
            while(it.hasNext()) {
                it.next();
                IDrawApplicationPlugin *plugin = it.value();
                if(plugin != nullptr && plugin->getElementID() == type) {
                    Element *ele = static_cast<Element *>(items[j]);
                    ele->writeData(out);
                }
            }
        }
    }
}

void GraphPage::saveAsBinary(const QString &filename) {

    QFile file(filename);
    QFileInfo fi(filename);

    if (GraphPageId != fi.baseName()) {
        QString newName = fi.absolutePath() + "/" + GraphPageId + ".drwb";
        file.rename(newName);
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,
                                 trUtf8("错误"),
                                 trUtf8("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream out(&file);

    out << *this;

    writeItems(out,items());

    m_undoStack->setClean();
    unsavedFlag = false;
    file.close();
    emit GraphPageSaved();
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

void GraphPage::saveAsXML(const QString &filename) {

    QFile file(filename);
    QFileInfo fi(filename);

    if (GraphPageId != fi.baseName()) {
        QString newName = fi.absolutePath() + "/" + GraphPageId + ".drw";
        file.rename(newName);
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,
                                 trUtf8("错误"),
                                 trUtf8("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    XmlGraphPageConfigWriter writer;
    writer.writeGraphPageConfig(file, this);

    unsavedFlag = false;
    m_undoStack->setClean();
    file.close();
    emit GraphPageSaved();
}

/**
XML GraphPage LOADING-------------------------------
*/


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
                        connectItem(ele);
                        addItem(ele);
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

    if (xml.attributes().hasAttribute("priority")) {
        setGraphPagePriority(xml.attributes().value("priority").toString());
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

    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > iter(PluginManager::getInstance()->plugins_);
    while(iter.hasNext())
    {
        iter.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  it(iter.value());
        while(it.hasNext())
        {
            it.next();
            IDrawApplicationPlugin *plugin = it.value();
            if(plugin != nullptr && plugin->getElementIDString() == internalType) {
                return plugin->createElement();
            }
        }
    }

    return nullptr;
}

/**
XML LIBRARY LOADING-------------------------------
*/

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
                        connectItem(ele);
                        addItem(ele);
                    }
                }
            }
        }

        xml.readNext();
    }
}

void GraphPage::slotSaveAsLibrary() {

    QString filename = QFileDialog::getSaveFileName(0,
                                                    trUtf8("Save graph library"),
                                                    QString("."),
                                                    trUtf8("Library (*.drwlib)"));

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,trUtf8("错误"),trUtf8("无法打开文件"),
                                 QMessageBox::Ok);
        return;
    }

    XmlGraphPageConfigWriter writer;
    writer.writeLibraryConfig(file,this);

    file.close();
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

    out << GraphPage.getFileName() << GraphPage.getGraphPageId() << GraphPage.getGraphPagePriority()
        << GraphPage.getGraphPageBackground() << GraphPage.getGraphPageHeight()
        << GraphPage.getGraphPageWidth();

    return out;
}

QDataStream &operator>>(QDataStream &in,GraphPage &GraphPage) {

    QString filename;
    QString id;
    QString priority;
    QColor backColor;
    int height;
    int width;

    in >> filename >> id >> priority >> backColor >> height >> width;

    GraphPage.setFileName(filename);
    GraphPage.setGraphPageId(id);
    GraphPage.setGraphPagePriority(priority);
    GraphPage.setGraphPageWidth(width);
    GraphPage.setGraphPageHeight(height);
    GraphPage.setGraphPageBackground(backColor);
    GraphPage.fillGridPixmap();

    return in;
}

