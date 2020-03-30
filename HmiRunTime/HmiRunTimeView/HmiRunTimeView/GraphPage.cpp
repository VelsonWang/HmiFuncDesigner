#include "GraphPage.h"
#include <cmath>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include "../Public/ElementGroup.h"
#include "HmiRunTime.h"
#include "ViewElementPluginManager.h"
#include "../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h"


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


GraphPage::GraphPage(const QRectF &rect, QWidget *parent)
    : QWidget(parent),
      filename(QString()),
      projpath_(QString())
{
    //setItemIndexMethod(QGraphicsScene::NoIndex);

    if (rect.width() == 0 || rect.height() == 0) {
        //setSceneRect(0, 0, 800, 480);
    }
    else {
        //setSceneRect(rect);
    }

    graphPageWidth = width();
    graphPageHeight = height();
    graphPagePriority.clear();
    graphPagePriority.append(tr("主要的"));
    graphPageBackground = QColor(Qt::white);

    connect(&refreshTmr_, SIGNAL(timeout()), this, SLOT(refreshGraphPage()));
    refreshTmr_.stop();
}

void GraphPage::setActive(bool active)
{
    onActive = active;
}

bool GraphPage::active()
{
    return onActive;
}

/**
 * @brief GraphPage::zValueSort
 * @details Z坐标由小到大排列元素
 * @param dat 待排序对象集
 */
void GraphPage::zValueSort(QList<Element *> &dat)
{
    for(int i=0; i<dat.size()-1; i++){
        for(int j=0; j<dat.size()-1-i; j++){
            if(dat.at(j)->getElementZValue()>dat.at(j+1)->getElementZValue()){
                dat.swap(j, j+1);
            }
        }
    }
}

void GraphPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(graphPageBackground);
    painter.fillRect(this->rect(), graphPageBackground);

    foreach (Element *pEle, elementList_) {
        pEle->paint(&painter);
    }
}

void GraphPage::mousePressEvent(QMouseEvent *event)
{
    foreach (Element *pEle, elementList_) {
        pEle->mousePressEvent(event);
    }
    QWidget::mousePressEvent(event);
}

void GraphPage::mouseMoveEvent(QMouseEvent *event)
{
    foreach (Element *pEle, elementList_) {
        pEle->mouseMoveEvent(event);
    }
    QWidget::mouseMoveEvent(event);
}

void GraphPage::mouseReleaseEvent(QMouseEvent *event)
{
    foreach (Element *pEle, elementList_) {
        pEle->mouseReleaseEvent(event);
    }
    QWidget::mouseReleaseEvent(event);
}


void GraphPage::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    //if (!itemAt(event->scenePos(), QTransform())) {
    //}
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
    //setBackgroundBrush(GraphPageBackground);
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
    this->setGeometry(0, 0, graphPageWidth, graphPageHeight);
}

int GraphPage::getGraphPageHeight() const
{
    return graphPageHeight;
}

void GraphPage::setGraphPageHeight(int height)
{
    graphPageHeight = height;
    this->setGeometry(0, 0, graphPageWidth, graphPageHeight);
}

/**
 * @brief GraphPage::setSelectedFunctions
 * @details 设置功能操作属性数据
 * @param funcs
 */
void GraphPage::setSelectedFunctions(QStringList funcs)
{
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

void GraphPage::createItems(const QString &typeId, QPointF position)
{
    Q_UNUSED(position)
    Element *last = nullptr;

    IHmiRunTimeViewPlugin* pPluginObj = ViewElementPluginManager::getInstance()->getPluginByElementName(typeId);
    if(pPluginObj != nullptr) {
        Element *ele = pPluginObj->createElement(this);
        if(ele != Q_NULLPTR) {
            connect(ele, &Element::notifyExecScriptFunction, this, &GraphPage::onExecScriptFunction);
            connect(ele, &Element::notifyExecScriptText, this, &GraphPage::onExecScriptText);
            ele->setRealTimeDB(RealTimeDB::instance());
            ele->setProjectPath(projpath_);
            ele->setClickPosition(position);
            last = ele;
        }
    }

    if (last) {
        emit newElementAdded();
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


void GraphPage::addElementEvent()
{
    emit newElementAdded();
}


void GraphPage::readItems(QDataStream &in, int offset, bool select)
{
    Q_UNUSED(select)
    int objectType;
    int itemsCount;
    copyList.clear();

    in >> itemsCount;

    for (int i = 0; i < itemsCount; i++) {
        in >> objectType;
        IHmiRunTimeViewPlugin* pPluginObj = ViewElementPluginManager::getInstance()->getPluginByElementID(objectType);
        if(pPluginObj != nullptr) {
            Element *ele = pPluginObj->createElement(this);
            if(ele != Q_NULLPTR) {
                connect(ele, &Element::notifyExecScriptFunction, this, &GraphPage::onExecScriptFunction);
                connect(ele, &Element::notifyExecScriptText, this, &GraphPage::onExecScriptText);
                ele->setRealTimeDB(RealTimeDB::instance());
                ele->setProjectPath(projpath_);
                ele->readData(in);
                copyList.insert(copyList.end(), ele);
            }
        }
    } //for

    foreach (Element *ele, copyList) {
        //ele->setSelected(select);
        ele->moveTo(offset, offset);
    }
}

void GraphPage::loadAsBinary(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(Q_NULLPTR,
                                 tr("错误"),
                                 tr("文件无法保存"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream in(&file);

    in >> *this;

    readItems(in,0,false);

    file.close();
}

void GraphPage::loadAsXML(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(Q_NULLPTR,
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
                        connect(ele, &Element::notifyExecScriptFunction, this, &GraphPage::onExecScriptFunction);
                        connect(ele, &Element::notifyExecScriptText, this, &GraphPage::onExecScriptText);
                        ele->setRealTimeDB(RealTimeDB::instance());
                        ele->setProjectPath(projpath_);
                        ele->readFromXml(xml.attributes());
                        elementList_.append(ele);
                    }
                }
            }
        }

        xml.readNext();
    }
    zValueSort(elementList_);
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
}

Element *GraphPage::createElement(const QString &internalType)
{
    IHmiRunTimeViewPlugin* pPluginObj = ViewElementPluginManager::getInstance()->getPluginByElementIDString(internalType);
    if(pPluginObj != nullptr) {
        return pPluginObj->createElement(this);
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
        QMessageBox::information(Q_NULLPTR,
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
                        connect(ele, &Element::notifyExecScriptFunction, this, &GraphPage::onExecScriptFunction);
                        connect(ele, &Element::notifyExecScriptText, this, &GraphPage::onExecScriptText);
                        ele->setRealTimeDB(RealTimeDB::instance());
                        ele->setProjectPath(projpath_);
                        ele->readFromXml(xml.attributes());
                        elementList_.append(ele);
                    }
                }
            }
        }

        xml.readNext();
    }
    zValueSort(elementList_);
}

/**
 * @brief GraphPage::getProjectPath
 * @details 获取工程路径
 * @return 工程路径
 */
QString GraphPage::getProjectPath() const
{
    return projpath_;
}


/**
 * @brief GraphPage::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void GraphPage::setProjectPath(const QString &path)
{
    projpath_ = path;
}

/**
 * @brief GraphPage::refreshGraphPage
 * @details 画面刷新
 */
void GraphPage::refreshGraphPage()
{
    this->update();
}


/**
 * @brief GraphPage::openGraphPage
 * @details 画面打开
 */
void GraphPage::openGraphPage()
{
    refreshTmr_.start(100);
    // 处理"打开画面"功能
    HmiRunTime::execScriptFunction(funcs_, tr("OpenGraphPage"));
}


/**
 * @brief GraphPage::closeGraphPage
 * @details 画面关闭
 */
void GraphPage::closeGraphPage()
{
    refreshTmr_.stop();
    // 处理"关闭画面"功能
    HmiRunTime::execScriptFunction(funcs_, tr("CloseGraphPage"));
}


/**
 * @brief GraphPage::getElement
 * @details 通过控件ID取得控件
 * @param eleId 控件ID
 * @return 控件对象
 */
Element *GraphPage::getElement(const QString &eleId)
{
    foreach (Element *pEle, elementList_) {
        if(pEle->getElementId() == eleId)
            return pEle;
    }
    return nullptr;
}


QDataStream &operator>>(QDataStream &in,GraphPage &page)
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

    return in;
}

/**
 * @brief GraphPage::onExecScriptFunction
 * @details 执行脚本功能
 * @param szFuncList 脚本文本列表
 * @param szMatchEvent 匹配事件
 */
void GraphPage::onExecScriptFunction(const QStringList &szFuncList, const QString &szMatchEvent)
{
    HmiRunTime::execScriptFunction(szFuncList, szMatchEvent);
}

/**
 * @brief GraphPage::notifyExecScriptText
 * @details 执行脚本文本
 * @param szScriptText 脚本文本
 * @param szMatchEvent 匹配事件
 */
void GraphPage::onExecScriptText(const QString &szScriptText, const QString &szMatchEvent)
{
    HmiRunTime::execScriptText(szScriptText, szMatchEvent);
}
