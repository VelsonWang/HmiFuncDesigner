#include "GraphPage.h"
#include <cmath>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsItem>
#include "ElementGroup.h"
#include "IDrawApplicationPlugin.h"
#include "PluginManager.h"
#include "Helper.h"
#include "XMLObject.h"
#include "GetWidthHeightDialog.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "ProjectData.h"

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

const QString MimeType = "rti/designer";


GraphPage::GraphPage(const QRectF &rect,
                     QtVariantPropertyManager *propertyMgr,
                     QtTreePropertyBrowser *propertyEditor,
                     QObject *parent) :
    QGraphicsScene(parent),
    filename(QString()),
    unsavedFlag_(false),
    szProjPath_(""),
    szProjName_(""),
    variantPropertyManager_(propertyMgr),
    propertyEditor_(propertyEditor)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);

    if (static_cast<int>(rect.width()) == 0 || static_cast<int>(rect.height()) == 0) {
        setSceneRect(0, 0, 800, 480);
    } else {
        setSceneRect(rect);
    }

    gridVisible = false;

    graphPageWidth = static_cast<int>(sceneRect().width());
    graphPageHeight = static_cast<int>(sceneRect().height());
    graphPagePriority.clear();
    graphPagePriority.append(tr("主要的"));
    graphPageBackground = QColor(Qt::white);

    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(20);

    createContextMenuActions();
    updateActions();

    createPropertyList();
    fillGraphPagePropertyModel();

    connect(variantPropertyManager_, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                    this, SLOT(slotElementPropertyChanged(QtProperty *, const QVariant &)));
    connect(variantPropertyManager_, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                    this, SLOT(slotGraphPagePropertyChanged(QtProperty *, const QVariant &)));
    connect(this, SIGNAL(selectionChanged()), SLOT(slotSelectionChanged()));
}

void GraphPage::setActive(bool active)
{
    onActive = active;
}

bool GraphPage::active()
{
    return onActive;
}

bool GraphPage::getUnsavedFlag()
{
    return unsavedFlag_;
}

void GraphPage::setUnsavedFlag(bool bFlag)
{
    unsavedFlag_ = bFlag;
}


void GraphPage::fillGridPixmap()
{
    gridPixmap = QPixmap(QSize(static_cast<int>(sceneRect().width()),
                               static_cast<int>(sceneRect().height())));
    gridPixmap.fill(graphPageBackground);

    QPainter painter(&gridPixmap);

    const int maxX = static_cast<int>(std::ceil(width())/gridSize)*gridSize;
    const int maxY = static_cast<int>(std::ceil(height())/gridSize)*gridSize;

    if (graphPageBackground == Qt::black) {
        painter.setPen(Qt::white);
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
    }

    for (int i = 0; i < maxX; i += gridSize) {
        for (int j = 0; j < maxY; j += gridSize) {
            painter.drawPoint(i,j);
        }
    }
}

void GraphPage::setGridVisible(bool on)
{
    gridVisible = on;

    if (gridVisible) {
        fillGridPixmap();
    }
    else {
        gridPixmap = QPixmap(QSize(static_cast<int>(sceneRect().width()),
                                   static_cast<int>(sceneRect().height())));
        gridPixmap.fill(graphPageBackground);
    }

    invalidate(sceneRect(), BackgroundLayer);
}

bool GraphPage::isGridVisible() const
{
    return gridVisible;
}

void GraphPage::addProperty(QtVariantProperty *property, const QString &id)
{
    propList.append(property);
    propertyToId_[property] = id;
    idToProperty_[id] = property;
    QtBrowserItem *item = propertyEditor_->addProperty(property);
    if (idToExpanded_.contains(id)) {
        propertyEditor_->setExpanded(item, idToExpanded_[id]);
    }
}

void GraphPage::updateExpandState()
{
    QList<QtBrowserItem *> list = propertyEditor_->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        QtProperty *prop = item->property();
        idToExpanded_[propertyToId_[prop]] = propertyEditor_->isExpanded(item);
    }
}

void GraphPage::slotGraphPagePropertyChanged(QtProperty *property, const QVariant &value)
{
    if (!selectedItems().isEmpty()) {
        return;
    }

    if (!active()) {
        return;
    }

    if (!propertyToId_.contains(property))
        return;

    //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << propertyToId_[property] << value;
    QString id = propertyToId_[property];
    if (id == QLatin1String("id")) {
        setGraphPageId(value.toString());
    } else if (id == QLatin1String("background")) {
        setGraphPageBackground(value.value<QColor>());
    } else if (id == QLatin1String("width")) {
        setGraphPageWidth(value.toInt());
        updateAllElementGraphPageSize(graphPageWidth, graphPageHeight);
    } else if (id == QLatin1String("height")) {
        setGraphPageHeight(value.toInt());
        updateAllElementGraphPageSize(graphPageWidth, graphPageHeight);
    } else if (id == QLatin1String("functions")) {
        QString szFuncs = value.toString();
        setSelectedFunctions(szFuncs.split('|'));
    }

    fillGridPixmap();

    unsavedFlag_ = true;
    emit GraphPagePropertyChanged();
}

void GraphPage::cleanPropertyModel()
{
    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId_.constBegin();
    while (itProp != propertyToId_.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId_.clear();
    idToProperty_.clear();
}

void GraphPage::fillGraphPagePropertyModel()
{
    propertyEditor_->clear();

    QtVariantProperty *property;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(graphPageId);
    }

    property = idToProperty_[QLatin1String("background")];
    if(property != Q_NULLPTR) {
        property->setValue(graphPageBackground);
    }

    property = idToProperty_[QLatin1String("width")];
    if(property != Q_NULLPTR) {
        property->setValue(graphPageWidth);
    }

    property = idToProperty_[QLatin1String("height")];
    if(property != Q_NULLPTR) {
        property->setValue(graphPageHeight);
    }

    property = idToProperty_[QLatin1String("functions")];
    if(property != Q_NULLPTR) {
        property->setValue(funcs_.join('|'));
    }

    QListIterator<QtProperty*> iter(propList);
    while (iter.hasNext()) {
        propertyEditor_->addProperty(iter.next());
    }
}

void GraphPage::createPropertyList()
{
    propList.clear();
    updateExpandState();
//    cleanPropertyModel();

    QtVariantProperty *property;

    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    property = variantPropertyManager_->addProperty(QVariant::Color, tr("背景颜色"));
    addProperty(property, QLatin1String("background"));

    property = variantPropertyManager_->addProperty(QVariant::Int, tr("宽度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("width"));

    property = variantPropertyManager_->addProperty(QVariant::Int, tr("高度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("height"));

    property = variantPropertyManager_->addProperty(VariantManager::functionTypeId(), tr("功能操作"));
    QStringList listEvents;
    getSupportEvents(listEvents);
    property->setAttribute(QLatin1String("supportevents"), listEvents.join("|"));
    addProperty(property, QLatin1String("functions"));
}

void GraphPage::createContextMenuActions()
{
    inGroupAction = new QAction(QIcon(":/images/group.png"), tr("分组"), &contextServiceMenu);
    connect(inGroupAction, SIGNAL(triggered()), SLOT(slotGroupElements()));

    outGroupAction = new QAction(QIcon(":/images/ungroup.png"), tr("分类"), &contextServiceMenu);

    alignTopAction = new QAction(QIcon(":/images/align-top.png"), tr("顶部对齐"), &contextServiceMenu);
    alignTopAction->setData(Qt::AlignTop);
    connect(alignTopAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    alignDownAction = new QAction(QIcon(":/images/align-bottom.png"), tr("底部对齐"), &contextServiceMenu);
    alignDownAction->setData(Qt::AlignBottom);
    connect(alignDownAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    alignRightAction = new QAction(QIcon(":/images/align-right.png"), tr("右对齐"), &contextServiceMenu);
    alignRightAction->setData(Qt::AlignRight);
    connect(alignRightAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    alignLeftAction = new QAction(QIcon(":/images/align-left.png"), tr("左对齐"), &contextServiceMenu);
    alignLeftAction->setData(Qt::AlignLeft);
    connect(alignLeftAction, SIGNAL(triggered()), SLOT(slotAlignElements()));

    hUniformDistributeAction_ = new QAction(QIcon(":/images/align_hsame.png"), tr("水平均匀分布"), &contextServiceMenu);
    connect(hUniformDistributeAction_, SIGNAL(triggered()), SLOT(slotHUniformDistributeElements()));

    vUniformDistributeAction_ = new QAction(QIcon(":/images/align_vsame.png"), tr("垂直均匀分布"), &contextServiceMenu);
    connect(vUniformDistributeAction_, SIGNAL(triggered()), SLOT(slotVUniformDistributeElements()));

    setTheSameSizeAction_ = new QAction(QIcon(":/images/the-same-size.png"), tr("大小一致"), &contextServiceMenu);
    connect(setTheSameSizeAction_, SIGNAL(triggered()), SLOT(slotSetTheSameSizeElements()));

    upLayerAction_ = new QAction(QIcon(":/images/posfront.png"), tr("上移一层"), &contextServiceMenu);
    connect(upLayerAction_, SIGNAL(triggered()), SLOT(slotUpLayerElements()));

    downLayerAction_ = new QAction(QIcon(":/images/posback.png"), tr("下移一层"), &contextServiceMenu);
    connect(downLayerAction_, SIGNAL(triggered()), SLOT(slotDownLayerElements()));

    saveAsLibraryAction = new QAction(QIcon(":/images/library.png"), tr("保存为库"),&contextMenu);
    connect(saveAsLibraryAction, SIGNAL(triggered()), SLOT(slotSaveAsLibrary()));

    actionDelete = new QAction(QIcon(":/images/delete.png"), tr("删除"),&contextMenu);
    actionDelete->setShortcut(QKeySequence::Delete);
    connect(actionDelete, SIGNAL(triggered()), SLOT(slotEditDelete()));

    actionCopy = new QAction(QIcon(":/images/editcopy.png"),tr("拷贝"),&contextMenu);
    actionCopy->setShortcut(QKeySequence::Copy);
    connect(actionCopy,SIGNAL(triggered()),SLOT(slotEditCopy()));

    actionPaste = new QAction(QIcon(":/images/editpaste.png"),tr("粘贴"),&contextMenu);
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste,SIGNAL(triggered()),SLOT(slotEditPaste()));

    actionSelectAll = new QAction(QIcon(), tr("全选"),&contextMenu);
    actionSelectAll->setShortcut(QKeySequence::SelectAll);
    connect(actionSelectAll, SIGNAL(triggered()), SLOT(slotSelectAll()));

    contextServiceMenu.setTitle(tr("工具"));
    contextServiceMenu.setIcon(QIcon(":/images/settings.png"));
    contextServiceMenu.addSeparator();
    contextServiceMenu.addAction(alignTopAction);
    contextServiceMenu.addAction(alignDownAction);
    contextServiceMenu.addAction(alignRightAction);
    contextServiceMenu.addAction(alignLeftAction);
    contextServiceMenu.addAction(hUniformDistributeAction_);
    contextServiceMenu.addAction(vUniformDistributeAction_);
    contextServiceMenu.addAction(setTheSameSizeAction_);
    contextServiceMenu.addSeparator();
    contextServiceMenu.addAction(upLayerAction_);
    contextServiceMenu.addAction(downLayerAction_);
    contextServiceMenu.addSeparator();

    contextMenu.addMenu(&contextServiceMenu);
    contextMenu.addAction(saveAsLibraryAction);
    contextMenu.addAction(actionDelete);
    contextMenu.addAction(actionCopy);
    contextMenu.addAction(actionPaste);
    contextMenu.addAction(actionSelectAll);
}

void GraphPage::updateActions()
{
    actionCopy->setEnabled(selectedItems().count() == 0 ? false : true);
    actionDelete->setEnabled(selectedItems().count() == 0 ? false : true);
    saveAsLibraryAction->setEnabled(selectedItems().count() == 0 ? false : true);
    downLayerAction_->setEnabled(selectedItems().count() == 0 ? false : true);
    upLayerAction_->setEnabled(selectedItems().count() == 0 ? false : true);
    alignTopAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignDownAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignRightAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignLeftAction->setEnabled(selectedItems().count() < 2 ? false : true);
    hUniformDistributeAction_->setEnabled(selectedItems().count() < 2 ? false : true);
    vUniformDistributeAction_->setEnabled(selectedItems().count() < 2 ? false : true);
    setTheSameSizeAction_->setEnabled(selectedItems().count() < 2 ? false : true);
}


void GraphPage::slotElementPropertyChanged(QtProperty *property, const QVariant &value)
{
    if (selectedItems().isEmpty()) {
        return;
    }

    if (!currentItem) {
        return;
    }

    currentItem->updateElementProperty(property, value);

    unsavedFlag_ = true;
    emit elementPropertyChanged();

    if (property->propertyName() == tr("ID")) {
        emit elementIdChanged();
    }
}

void GraphPage::slotSelectionChanged()
{
    updateActions();

    if (selectedItems().isEmpty()) {
        propertyEditor_->clear();
        currentItem = Q_NULLPTR;
        return;
    }

    propertyEditor_->clear();
    currentItem = dynamic_cast<Element *>(selectedItems().first());

    if (!currentItem) {
        return;
    }

    currentItem->updatePropertyModel();
    QListIterator<QtProperty*> iter(currentItem->getPropertyList());
    while (iter.hasNext()) {
        propertyEditor_->addProperty(iter.next());
    }
}

void GraphPage::slotElementMoved(QPointF oldPos)
{
    Element *ele = (Element *)sender();

    m_undoStack->push(new ChangePositionCommand(ele, oldPos));
}

void GraphPage::slotElementResized(int width, int height,QPointF pos)
{
    Element *ele = (Element *)sender();
    m_undoStack->push(new ChangeSizeCommand(ele, width, height, pos));
}

void GraphPage::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(graphPageBackground);
    painter->drawRect(rect);

    if (gridVisible) {
        painter->drawPixmap(0, 0, gridPixmap);
    }

    painter->restore();
}

QUndoStack *GraphPage::undoStack() const
{
    return m_undoStack;
}

void GraphPage::setFileName(const QString &file)
{
    filename = file;
}

QString GraphPage::getFileName() const
{
    return filename;
}

void GraphPage::setGraphPageId(const QString &id)
{
    graphPageId = id;
    emit changeGraphPageName();
}

QString GraphPage::getGraphPageId() const
{
    return graphPageId;
}

void GraphPage::setGraphPageBackground(const QColor &color)
{
    graphPageBackground = color;
    setBackgroundBrush(graphPageBackground);
}

QColor GraphPage::getGraphPageBackground() const
{
    return graphPageBackground;
}

int GraphPage::getGraphPageWidth() const
{
    return graphPageWidth;
}

void GraphPage::setGraphPageWidth(int width)
{
    graphPageWidth = width;
    setSceneRect(0, 0, graphPageWidth, graphPageHeight);
}

int GraphPage::getGraphPageHeight() const
{
    return graphPageHeight;
}

void GraphPage::setGraphPageHeight(int height)
{
    graphPageHeight = height;
    setSceneRect(0, 0, graphPageWidth, graphPageHeight);
}

void GraphPage::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
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

void GraphPage::dropEvent(QGraphicsSceneDragDropEvent *event)
{
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

void GraphPage::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void GraphPage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    if (!itemAt(event->scenePos(), QTransform())) {
        fillGraphPagePropertyModel();
    }
}


void GraphPage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);

    if (!itemAt(event->scenePos(), QTransform())) {
        fillGraphPagePropertyModel();
    }
}

void GraphPage::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    foreach (QGraphicsItem *item, selectedItems()) {
        Element *ele = dynamic_cast<Element *>(item);
        if (ele != nullptr) {
            ele->RestrictedRectangularRegion();
        }
    }
}

void GraphPage::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    contextMenu.exec(event->screenPos());
}

void GraphPage::connectItem(Element *item)
{
    connect(item, SIGNAL(elementMoved(QPointF)), SLOT(slotElementMoved(QPointF)));
    connect(item, SIGNAL(elementResized(int,int,QPointF)), SLOT(slotElementResized(int,int,QPointF)));
}

void GraphPage::createItems(const QString &typeId, QPointF position)
{
    Element *last = Q_NULLPTR;

    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > iter(PluginManager::getInstance()->plugins_);
    while(iter.hasNext()) {
        iter.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  it(iter.value());
        while(it.hasNext()) {
            it.next();
            IDrawApplicationPlugin *plugin = it.value();
            if(plugin != nullptr && plugin->getElementName() == typeId) {
                Element *ele = plugin->createElement(szProjPath_, szProjName_,
                                                     variantPropertyManager_,
                                                     ProjectData::getInstance());
                if(ele != Q_NULLPTR) {
                    ele->setClickPosition(position);
                    ele->setGraphPageSize(getGraphPageWidth(), getGraphPageHeight());
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

void GraphPage::slotGroupElements()
{
    if (selectedItems().isEmpty()) {
        return;
    }

    ElementGroup *group = new ElementGroup;

    foreach (QGraphicsItem *item, selectedItems()) {
        group->addToGroup(item);
    }

    group->setClickPosition(QPointF(500, 500));
    group->setHandlesChildEvents(false);
    addItem(group);
}

void GraphPage::slotUngroupElements()
{

}

void GraphPage::onAlignElements(Qt::Alignment alignment, QList<QGraphicsItem*> items)
{
    QVector<double> coordinates;
    populateCoordinates(alignment, &coordinates, items);
    double offset;

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop) {
        offset = min(coordinates);
    } else {
        offset = max(coordinates);
    }

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight) {
        for (int i = 0; i < items.count(); ++i) {
            Element *pEle = dynamic_cast<Element *>(items.at(i));
            pEle->moveTo(offset - coordinates.at(i), 0);
            pEle->updateBoundingElement();
        }
    } else {
        for (int i = 0; i < items.count(); ++i) {
            Element *pEle = dynamic_cast<Element *>(items.at(i));
            pEle->moveTo(0, offset - coordinates.at(i));
            pEle->updateBoundingElement();
        }
    }
}

void GraphPage::slotAlignElements()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;
    Qt::Alignment alignment = static_cast<Qt::Alignment>(action->data().toInt());
    onAlignElements(alignment, selectedItems());
}

/**
 * @brief GraphPage::horizontalSort
 * @details 水平排序
 * @param dat 待排序对象集
 */
void GraphPage::horizontalSort(QList<QGraphicsItem *> &dat)
{
    for(int i=0; i<dat.size()-1; i++){
        for(int j=0; j<dat.size()-1-i; j++){
            if(dat.at(j)->sceneBoundingRect().x()>dat.at(j+1)->sceneBoundingRect().x()){
                dat.swap(j, j+1);
            }
        }
    }
}

void GraphPage::onHUniformDistributeElements(QList<QGraphicsItem*> items)
{
    if(items.count() < 2) {
        return;
    }

    double totalWidth = 0;
    horizontalSort(items);

    QListIterator<QGraphicsItem *> iter(items);
    while (iter.hasNext()) {
        QRectF rect = iter.next()->sceneBoundingRect();
        totalWidth += rect.width();
    }

    double minX = items.first()->sceneBoundingRect().x();
    double maxX = items.last()->sceneBoundingRect().x() + items.last()->sceneBoundingRect().width();
    int space = (maxX - minX - totalWidth) / (items.count() -1);
    int x = items.at(0)->sceneBoundingRect().x()+ items.at(0)->sceneBoundingRect().width() + space;

    for(int i=1; i<items.count()-1; i++) {
        QGraphicsItem * pItem = items.at(i);
        Element *pEle = (Element *)items.at(i);
        pEle->setElementXPos(x);
        pEle->updateBoundingElement();
        x = x + pItem->sceneBoundingRect().width() + space;
    }
}

/**
 * @brief GraphPage::slotHUniformDistributeElements
 * @details 水平均匀分布
 */
void GraphPage::slotHUniformDistributeElements()
{
    onHUniformDistributeElements(selectedItems());
}


/**
 * @brief GraphPage::verticalSort
 * @details 垂直排序
 * @param dat 待排序对象集
 */
void GraphPage::verticalSort(QList<QGraphicsItem *> &dat)
{
    for(int i=0; i<dat.size()-1; i++){
        for(int j=0; j<dat.size()-1-i; j++){
            if(dat.at(j)->sceneBoundingRect().y()>dat.at(j+1)->sceneBoundingRect().y()){
                dat.swap(j, j+1);
            }
        }
    }
}

void GraphPage::onVUniformDistributeElements(QList<QGraphicsItem*> items)
{
    if(items.count() < 2) {
        return;
    }

    double totalHeight = 0;
    verticalSort(items);

    QListIterator<QGraphicsItem *> iter(items);
    while (iter.hasNext()) {
        QRectF rect = iter.next()->sceneBoundingRect();
        totalHeight += rect.height();
    }

    double minY = items.first()->sceneBoundingRect().y();
    double maxY = items.last()->sceneBoundingRect().y() + items.last()->sceneBoundingRect().height();
    int space = (maxY - minY - totalHeight) / (items.count() -1);
    int y = items.at(0)->sceneBoundingRect().y()+ items.at(0)->sceneBoundingRect().height() + space;

    for(int i=1; i<items.count()-1; i++) {
        QGraphicsItem * pItem = items.at(i);
        Element *pEle = (Element *)items.at(i);
        pEle->setElementYPos(y);
        pEle->updateBoundingElement();
        y = y + pItem->sceneBoundingRect().height() + space;
    }
}


/**
 * @brief GraphPage::slotVUniformDistributeElements
 * @details 垂直均匀分布
 */
void GraphPage::slotVUniformDistributeElements()
{
    onVUniformDistributeElements(selectedItems());
}


void GraphPage::onSetTheSameSizeElements(QList<QGraphicsItem*> items)
{
    if(items.count() < 2) {
        return;
    }

    double dTotalWidth = 0;
    double dTotalHeight = 0;

    QListIterator<QGraphicsItem *> iter(items);
    while (iter.hasNext()) {
        QRectF rect = iter.next()->sceneBoundingRect();
        dTotalWidth += rect.width();
        dTotalHeight += rect.height();
    }

    int iWidth = static_cast<int>(dTotalWidth / items.count());
    int iHeight = static_cast<int>(dTotalHeight / items.count());

    GetWidthHeightDialog dlg;
    dlg.setWidth(iWidth);
    dlg.setHeight(iHeight);
    if(dlg.exec() == GetWidthHeightDialog::Accepted) {
        iWidth = dlg.getWidth();
        iHeight = dlg.getHeight();
        QListIterator<QGraphicsItem *> iterEle(items);
        while (iterEle.hasNext()) {
            QGraphicsItem * pItem = iterEle.next();
            Element *pEle = (Element *)pItem;
            pEle->setElementWidth(iWidth);
            pEle->setElementHeight(iHeight);
            pEle->updateBoundingElement();
        }
    }
    this->update();
}


/**
 * @brief GraphPage::slotSetTheSameSizeElements
 * @details 设置选中控件大小一致
 */
void GraphPage::slotSetTheSameSizeElements()
{
    onSetTheSameSizeElements(selectedItems());
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


void GraphPage::onUpLayerElements(QList<QGraphicsItem*> items)
{
    foreach (QGraphicsItem *item, items) {
        Element *ele = (Element *)item;
        ele->setElementZValue(ele->getElementZValue() + 1);
    }
}


/**
 * @brief GraphPage::slotUpLayerElements
 * @details 上移一层
 */
void GraphPage::slotUpLayerElements()
{
    onUpLayerElements(selectedItems());
}


void GraphPage::onDownLayerElements(QList<QGraphicsItem*> items)
{
    foreach (QGraphicsItem *item, items) {
        Element *ele = (Element*)item;
        ele->setElementZValue(ele->getElementZValue() - 1);
    }
}


/**
 * @brief GraphPage::slotDownLayerElements
 * @details 下移一层
 */
void GraphPage::slotDownLayerElements()
{
    onDownLayerElements(selectedItems());
}


void GraphPage::onEditDelete(QList<QGraphicsItem*> items)
{
    m_undoStack->push(new RemoveCommand(items, this));
    propertyEditor_->clear();
    emit elementsDeleted();
}


/**
 * @brief GraphPage::slotEditDelete
 * @details 删除
 */
void GraphPage::slotEditDelete()
{
    onEditDelete(selectedItems());
}

void GraphPage::removeElementEvent()
{
    emit elementsDeleted();
}

void GraphPage::addElementEvent()
{
    emit newElementAdded();
}

void GraphPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        if (!selectedItems().isEmpty()) {
            slotEditDelete();
        }
    } else if (event->matches(QKeySequence::Copy)) {
        slotEditCopy();
    } else if (event->matches(QKeySequence::Paste)) {
        slotEditPaste();
    } else if (event->matches(QKeySequence::SelectAll)) {
        slotSelectAll();
    } else if (event->key() == Qt::Key_Up) {
        moveSelectedElements(0, -10);
    } else if (event->key() == Qt::Key_Down) {
        moveSelectedElements(0, 10);
    } else if (event->key() == Qt::Key_Left) {
        moveSelectedElements(-10, 0);
    } else if (event->key() == Qt::Key_Right) {
        moveSelectedElements(10, 0);
    }
}

void GraphPage::moveSelectedElements(int xOffset, int yOffset)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        Element *ele = dynamic_cast<Element *>(item);
        if (ele) {
            int x = ele->getElementXPos() + xOffset;
            if(x < 0) {
                xOffset = 0 - ele->getElementXPos();
            }
            if(x > this->getGraphPageWidth() - ele->getElementWidth()) {
                xOffset = this->getGraphPageWidth() - ele->getElementWidth() - ele->getElementXPos();
            }

            int y = ele->getElementYPos() + yOffset;
            if(y < 0) {
                yOffset = 0 - ele->getElementYPos();
            }
            if(y > this->getGraphPageHeight() - ele->getElementHeight()) {
                yOffset = this->getGraphPageHeight() - ele->getElementHeight() - ele->getElementYPos();
            }

            ele->moveTo(xOffset, yOffset);
        }
    }
}

void GraphPage::slotSelectAll()
{
    foreach (QGraphicsItem *item, items()) {
        item->setSelected(true);
    }
}


void GraphPage::onEditCopy(QList<QGraphicsItem*> items)
{
    if (items.isEmpty()) {
        return;
    }
    copyItems(items);
}


/**
 * @brief GraphPage::slotEditCopy
 * @details 拷贝
 */
void GraphPage::slotEditCopy()
{
    onEditCopy(selectedItems());
}

void GraphPage::copyItems(const QList<QGraphicsItem *> &items)
{
    QByteArray copiedItems;
    QDataStream out(&copiedItems, QIODevice::WriteOnly);
    writeItems(out, items);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeType, copiedItems);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}


void GraphPage::onEditPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (!mimeData) {
        return;
    }

    if (mimeData->hasFormat(MimeType)) {
        QByteArray copiedItems = mimeData->data(MimeType);
        QDataStream in(&copiedItems, QIODevice::ReadOnly);
        readItems(in, pasteOffset, true);
    }
}


/**
 * @brief GraphPage::slotEditPaste
 * @details 粘贴
 */
void GraphPage::slotEditPaste()
{
    onEditPaste();
}

void GraphPage::readItems(QDataStream &in, int offset, bool select)
{
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
                    Element *ele = plugin->createElement(szProjPath_,
                                                         szProjName_,
                                                         variantPropertyManager_,
                                                         ProjectData::getInstance());
                    if(ele != Q_NULLPTR) {
                        ele->setGraphPageSize(getGraphPageWidth(), getGraphPageHeight());
                        ele->readData(in);
                        if(select) {
                            ele->regenerateElementId();
                        }
                        connectItem(ele);
                        copyList.insert(copyList.end(), ele);
                    }
                }
            }
        }
    } //for

    foreach (QGraphicsItem *item, copyList) {
        Element *ele = (Element *)item;
        ele->setSelected(select);
        ele->moveTo(offset, offset);
    }

    m_undoStack->push(new AddCommand(copyList, this));
}

void GraphPage::writeItems(QDataStream &out, const QList<QGraphicsItem *> &items)
{
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

void GraphPage::saveAsBinary(const QString &filename)
{
    QFile file(filename);
    QFileInfo fi(filename);

    if (graphPageId != fi.baseName()) {
        QString newName = fi.absolutePath() + "/" + graphPageId + ".drwb";
        file.rename(newName);
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(nullptr,
                                 tr("错误"),
                                 tr("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream out(&file);
    out << *this;
    writeItems(out,items());
    m_undoStack->setClean();
    unsavedFlag_ = false;
    file.close();
    emit GraphPageSaved();
}

void GraphPage::loadAsBinary(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr,
                                 tr("错误"),
                                 tr("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream in(&file);
    in >> *this;
    readItems(in, 0, false);
    file.close();
}

void GraphPage::saveAsXML(const QString &filename)
{
    QFile file(filename);
    QFileInfo fi(filename);

    if (graphPageId != fi.baseName()) {
        QString newName = fi.absolutePath() + "/" + graphPageId + ".drw";
        file.rename(newName);
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(nullptr,
                                 tr("错误"),
                                 tr("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    writeGraphPage(file, this);

    unsavedFlag_ = false;
    m_undoStack->setClean();
    file.close();
    emit GraphPageSaved();
}


void GraphPage::loadAsXML(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr,
                                 tr("错误"),
                                 tr("无法打开文件"),
                                 QMessageBox::Ok);
        return;
    }

    readGraphPageConfig(file);
    addElementEvent();

    file.close();
}

void GraphPage::readGraphPageConfig(QFile &file)
{
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

void GraphPage::readGraphPageTag(QXmlStreamReader &xml)
{
    setGraphPageAttributes(xml);
    copyList.clear();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "graphPage")) {
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            if (xml.name() == "element") {
                if (xml.attributes().hasAttribute("internalType")) {
                    Element *ele = createElement(xml.attributes().value("internalType").toString());
                    if (ele) {
                        ele->setGraphPageSize(getGraphPageWidth(), getGraphPageHeight());
                        ele->setProjectName(szProjName_);
                        ele->readFromXml(xml.attributes());
                        ele->setSelected(false);
                        connectItem(ele);
                        addItem(ele);
                    }
                }
            }
        }

        xml.readNext();
    }
}

void GraphPage::setGraphPageAttributes(QXmlStreamReader &xml)
{
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

    if (xml.attributes().hasAttribute("functions")) {
        QString listString = xml.attributes().value("functions").toString();
        setSelectedFunctions(listString.split('|'));
    }

    fillGridPixmap();
}

Element *GraphPage::createElement(const QString &internalType)
{
    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > iter(PluginManager::getInstance()->plugins_);
    while(iter.hasNext()) {
        iter.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  it(iter.value());
        while(it.hasNext()) {
            it.next();
            IDrawApplicationPlugin *plugin = it.value();
            if(plugin != nullptr && plugin->getElementIDString() == internalType) {
                return plugin->createElement(szProjPath_, szProjName_,
                                             variantPropertyManager_,
                                             ProjectData::getInstance());
            }
        }
    }

    return nullptr;
}


void GraphPage::loadLibrary(QByteArray &data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString filename;
    in >> filename;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr,
                                 tr("错误"),
                                 tr("无法打开文件"),
                                 QMessageBox::Ok);
        return;
    }

    readLibraryConfig(file);
    addElementEvent();

    file.close();
}

void GraphPage::readLibraryConfig(QFile &file)
{
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

void GraphPage::readLibraryTag(QXmlStreamReader &xml)
{
    copyList.clear();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Library")) {
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            if (xml.name() == "element") {
                if (xml.attributes().hasAttribute("internalType")) {
                    Element *ele = createElement(xml.attributes().value("internalType").toString());
                    if (ele) {
                        ele->setProjectName(szProjName_);
                        ele->setGraphPageSize(getGraphPageWidth(), getGraphPageHeight());
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

void GraphPage::slotSaveAsLibrary()
{
    QString filename = QFileDialog::getSaveFileName(nullptr,
                                                    tr("Save graph library"),
                                                    QString("."),
                                                    tr("Library (*.drwlib)"));

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(nullptr,
                                 tr("错误"),
                                 tr("无法打开文件"),
                                 QMessageBox::Ok);
        return;
    }

    writeLibrary(file,this);

    file.close();
}

/**
 * @brief GraphPage::getProjectPath
 * @details 获取工程路径
 * @return 工程路径
 */
QString GraphPage::getProjectPath() const
{
    return szProjPath_;
}


/**
 * @brief GraphPage::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void GraphPage::setProjectPath(const QString &path)
{
    szProjPath_ = path;
}


QString GraphPage::getProjectName() const
{
    return szProjName_;
}

void GraphPage::setProjectName(const QString &name)
{
    szProjName_ = name;
}

/**
 * @brief GraphPage::setSelectedFunctions
 * @details 设置功能操作属性数据
 * @param funcs
 */
void GraphPage::setSelectedFunctions(QStringList funcs) {
    funcs_ = funcs;
}


/**
 * @brief GraphPage::getSelectedFunctions
 * @details 获取功能操作属性数据
 * @param funcs
 */
QStringList GraphPage::getSelectedFunctions()
{
    return funcs_;
}

void GraphPage::getSupportEvents(QStringList &listValue)
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
        if(szEventGroupName == "GraphPage") {

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

void GraphPage::writeGraphPage(QFile &file, GraphPage *graphPage)
{
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("graphPage");
    writer.writeAttribute("fileName", graphPage->getFileName());
    writer.writeAttribute("graphPageId", graphPage->getGraphPageId());
    writer.writeAttribute("width", QString::number(graphPage->getGraphPageWidth()));
    writer.writeAttribute("height", QString::number(graphPage->getGraphPageHeight()));
    writer.writeAttribute("background", graphPage->getGraphPageBackground().name());
    writer.writeAttribute("functions", graphPage->getSelectedFunctions().join("|"));
    QListIterator <QGraphicsItem*> it(graphPage->items());

    while (it.hasNext()) {
        Element *ele = static_cast<Element *>(it.next());
        ele->writeAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}

void GraphPage::writeLibrary(QFile &file, GraphPage *graphPage)
{
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Library");

    QListIterator <QGraphicsItem*> it(graphPage->selectedItems());

    while (it.hasNext()) {
        Element *ele = static_cast<Element *>(it.next());
        ele->writeAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}



/**
 * @brief GraphPage::updateAllElementGraphPageSize
 * @details 页面宽度或高度发生改变时更新当前页面子控件所属页面大小
 * @param width 页面宽度
 * @param height 页面高度
 */
void GraphPage::updateAllElementGraphPageSize(int width, int height)
{
    foreach (QGraphicsItem *item, items()) {
        Element *pEle = dynamic_cast<Element *>(item);
        if (pEle != nullptr) {
            pEle->setGraphPageSize(width, height);
        }
    }
}

QDataStream &operator<<(QDataStream &out, GraphPage &page)
{
    out << page.getFileName()
        << page.getGraphPageId()
        << page.getGraphPageBackground()
        << page.getGraphPageHeight()
        << page.getGraphPageWidth()
        << page.getSelectedFunctions();

    return out;
}

QDataStream &operator>>(QDataStream &in, GraphPage &page)
{
    QString filename;
    QString id;

    QColor backColor;
    int height;
    int width;
    QStringList funcs;

    in >> filename
       >> id
       >> backColor
       >> height
       >> width
       >> funcs;

    page.setFileName(filename);
    page.setGraphPageId(id);
    page.setGraphPageWidth(width);
    page.setGraphPageHeight(height);
    page.setGraphPageBackground(backColor);
    page.setSelectedFunctions(funcs);
    page.fillGridPixmap();

    return in;
}

