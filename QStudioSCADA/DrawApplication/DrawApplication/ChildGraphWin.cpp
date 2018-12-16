
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QPalette>

#include <QDebug>

#include "ChildGraphWin.h"
#include "ui_ChildGraphWin.h"
#include "GraphPage.h"
#include "MainWindow.h"

ChildGraphWin::ChildGraphWin(QString filePathName, QString page, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChildGraphWin),
    isModified(false),
    m_FilePathName(filePathName),
    m_GraphPageName(page),
    m_pCurrentGraph(NULL)
{
    ui->setupUi(this);
    this->setWindowTitle(page);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_BackColor = Qt::white;
    m_IsBackground = false;
    m_Width = 800;
    m_Height = 480;

    this->m_PaintArea = new PaintArea();
    this->m_PaintArea->setImageSize(m_Width, m_Height);
    this->m_PaintArea->installEventFilter(this);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(this->m_PaintArea);  //将画布添加到scrollArea

    connect(this, SIGNAL(signalPaintAreaUpdate()), this, SLOT(slotsPaintAreaUpdate()));

    m_pVariantManager = new QtVariantPropertyManager();
    m_pVariantFactory = new QtVariantEditorFactory();
    connect(m_pVariantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                    this, SLOT(valueChanged(QtProperty *, const QVariant &)));

    emit signalPaintAreaUpdate();
}

ChildGraphWin::~ChildGraphWin()
{
    delete ui;
}


// 加载json文档
void ChildGraphWin::load(const QJsonObject &json)
{
    int r = json["BackColor.red"].toInt();
    int g = json["BackColor.green"].toInt();
    int b = json["BackColor.blue"].toInt();
    m_BackColor = QColor(r, g, b);
    m_IsBackground = json["IsBackground"].toBool();
    m_Width = json["Width"].toInt();
    m_Height = json["Height"].toInt();
    //qDebug() << m_Width << m_Height;

    if(this->m_Width < 1)
        this->m_Width = 800;
    if(this->m_Height < 1)
        this->m_Height = 480;
    this->m_PaintArea->setImageColor(this->m_BackColor);
    this->m_PaintArea->setImageSize(this->m_Width, this->m_Height);

    this->m_GraphPage.load(json);

    GetGraphPageProperty(this->m_pPropertyBrowser);
    DrawGraphPage();
}


// 保存json文档
void ChildGraphWin::save(QJsonObject &json)
{
    json["BackColor.red"] = m_BackColor.red();
    json["BackColor.green"] = m_BackColor.green();
    json["BackColor.blue"] = m_BackColor.blue();
    json["IsBackground"] = m_IsBackground;
    json["Width"] = m_Width;
    json["Height"] = m_Height;
    this->m_GraphPage.save(json);
    this->isModified = false;
}



/*
* 增加属性
*/
void ChildGraphWin::addProperty(QtTreePropertyBrowser *propertyBrowser, QtVariantProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyBrowser->addProperty(property);
    if (idToExpanded.contains(id))
        propertyBrowser->setExpanded(item, idToExpanded[id]);
}


// 获取画面属性
void ChildGraphWin::GetGraphPageProperty(QtTreePropertyBrowser *propertyBrowser)
{
    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd())
    {
        delete itProp.key();
        itProp++;
    }
    propertyToId.clear();
    idToProperty.clear();

    propertyBrowser->clear();

    QtVariantProperty *backColor = this->m_pVariantManager->addProperty(QVariant::Color, tr("背景颜色"));
    backColor->setValue(m_BackColor);
    addProperty(propertyBrowser, backColor, QLatin1String("背景颜色"));

    QtVariantProperty *isBackground = this->m_pVariantManager->addProperty(QVariant::Bool, tr("是否背景"));
    isBackground->setValue(m_IsBackground);
    addProperty(propertyBrowser, isBackground, QLatin1String("是否背景"));

    QtVariantProperty *width = this->m_pVariantManager->addProperty(QVariant::Int, tr("宽度"));
    width->setValue(m_Width);
    addProperty(propertyBrowser, width, QLatin1String("宽度"));

    QtVariantProperty *height = this->m_pVariantManager->addProperty(QVariant::Int, tr("高度"));
    height->setValue(m_Height);
    addProperty(propertyBrowser, height, QLatin1String("高度"));

    propertyBrowser->setFactoryForManager(this->m_pVariantManager, this->m_pVariantFactory);
    propertyBrowser->setPropertiesWithoutValueMarked(true);
    propertyBrowser->setRootIsDecorated(false);

}


/*
* 设置属性编辑器
*/
void ChildGraphWin::SetPropertyBrowser(QtTreePropertyBrowser *propertyBrowser)
{
    this->m_pPropertyBrowser = propertyBrowser;
}


/*
* 绘制页面
*/
void ChildGraphWin::DrawGraphPage()
{
    this->m_PaintArea->DrawGraphPage(this->m_GraphPage);
}

/*
* 添加一个图形元素
*/
void ChildGraphWin::AddGraphElement(StdElement *e)
{
    if(!e) return;
    this->isModified = true;
    this->m_GraphPage.AddGraphElement(e);
    this->DrawGraphPage();
}


/*
* 删除一个图形元素
*/
void ChildGraphWin::DelGraphElement(StdElement *e)
{
    this->m_GraphPage.DelGraphElement(e);
    this->DrawGraphPage();
}


/*
* 单击事件响应
*/
void ChildGraphWin::mousePress(QMouseEvent * event)
{
    bool bSelectFlag = false;

    if(m_pCurrentGraph)
    {
        m_pCurrentGraph->m_iX = event->pos().x();
        m_pCurrentGraph->m_iY = event->pos().y();
        m_pCurrentGraph->setDefault();
        AddGraphElement(m_pCurrentGraph);
        m_pCurrentGraph = NULL;
    }

    if(event->buttons() & Qt::LeftButton) //鼠标左键按着
    {
        for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
        {
            StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
            if(pStdElement)
            {
                IElement *pIElement = pStdElement->GetInterface();
                if(pIElement)
                {
                    QCoreApplication::sendEvent(pStdElement, event);
                    if(pIElement->isSelected())
                    {
                        bSelectFlag = true;
                        pIElement->GetElementProperty(this->m_pPropertyBrowser, this->m_pVariantManager, this->m_pVariantFactory);
                    }
                }
            }
        }

        if(!bSelectFlag)
            this->GetGraphPageProperty(this->m_pPropertyBrowser);

        DrawGraphPage();
    }
}


/*
* 双击事件响应
*/
void ChildGraphWin::mouseDoubleClick(QMouseEvent * event)
{
    if(event->buttons() & Qt::LeftButton) //鼠标左键按着
    {
        for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
        {
            StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
            if(pStdElement)
            {
                IElement *pIElement = pStdElement->GetInterface();
                if(pIElement)
                {
                    QCoreApplication::sendEvent(pStdElement, event);
                }
            }
        }
        DrawGraphPage();
    }
}


/*
* 左键拖动响应
*/
void ChildGraphWin::mouseMove(QMouseEvent * event)
{
    if(event->buttons() & Qt::LeftButton) //鼠标左键按着
    {
        for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
        {
            StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
            if(pStdElement)
            {
                IElement *pIElement = pStdElement->GetInterface();
                if(pIElement)
                {
                    QCoreApplication::sendEvent(pStdElement, event);
                }
            }
        }
        DrawGraphPage();
    }   

}


/*
* 鼠标释放
*/
void ChildGraphWin::mouseRelease(QMouseEvent * event)
{
    for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
    {
        StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
        if(pStdElement)
        {
            IElement *pIElement = pStdElement->GetInterface();
            if(pIElement)
            {
                QCoreApplication::sendEvent(pStdElement, event);
            }
        }
    }
    DrawGraphPage();
}



/*
* 属性更改处理
*/
void ChildGraphWin::valueChanged(QtProperty *property, const QVariant &value)
{
    for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
    {
        StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
        if(pStdElement)
        {
            IElement *pIElement = pStdElement->GetInterface();
            if(pIElement)
            {
                if(pIElement->isSelected())
                {
                    pIElement->PropertyValueChanged(property, value);
                    emit signalPaintAreaUpdate();
                    return;
                }
            }
        }
    }

    if (!propertyToId.contains(property))
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("背景颜色"))
    {
        m_BackColor = value.value<QColor>();
    }
    else if (id == QLatin1String("是否背景"))
    {
        m_IsBackground = value.toBool();
    }
    else if (id == QLatin1String("宽度"))
    {
        m_Width = value.toInt();
    }
    else if (id == QLatin1String("高度"))
    {
        m_Height = value.toInt();
    }

    emit signalPaintAreaUpdate();
}



void ChildGraphWin::slotsPaintAreaUpdate()
{
    this->m_PaintArea->setImageSize(this->m_Width, this->m_Height);
    this->m_PaintArea->setImageColor(this->m_BackColor);
    DrawGraphPage();
}

void ChildGraphWin::ToolButtonClick(StdElement *e)
{
    m_pCurrentGraph = e;
}


/*
* 右键菜单
*/
void ChildGraphWin::contextMenuEvent(QContextMenuEvent * event)
{
    Q_UNUSED(event)
    bool found = false;

    for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
    {
        StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
        if(pStdElement)
        {
            IElement *pIElement = pStdElement->GetInterface();
            if(pIElement)
            {
                if(pIElement->isSelected())
                {
                    found = true;
                    break;
                }
            }
        }
    }

    if(!found)
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pDeleteElementAct = new QAction(tr("删除元素"), this);
    pDeleteElementAct->setStatusTip(tr("删除元素"));
    connect(pDeleteElementAct, SIGNAL(triggered()), this, SLOT(deleteElement()));
    pMenu->addAction(pDeleteElementAct);

    pMenu->move(cursor().pos());
    pMenu->show();




}

/*
 * 插槽： 删除元素
 */
void ChildGraphWin::deleteElement()
{
    QList<StdElement *> elementList;

    for (int i = 0; i < this->m_GraphPage.m_ElementList.size(); i++)
    {
        StdElement *pStdElement = this->m_GraphPage.m_ElementList.at(i);
        if(pStdElement)
        {
            IElement *pIElement = pStdElement->GetInterface();
            if(pIElement)
            {
                if(pIElement->isSelected())
                {
                    elementList.append(pStdElement);
                }
            }
        }
    }
    while (!elementList.isEmpty())
    {
        StdElement *pStdElement = elementList.takeFirst();
        this->m_GraphPage.m_ElementList.removeOne(pStdElement);
        delete pStdElement;
    }

    emit signalPaintAreaUpdate();
}

void ChildGraphWin::closeEvent(QCloseEvent *event)
{
    if(this->isModified)
    {
        QMessageBox::information(this, tr("提示"), tr("文件已修改，请先保存文件在关闭！"));
        event->ignore();
    }
}

bool ChildGraphWin::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this->m_PaintArea){
        //qDebug() << "eMousePress" << event->type();
        if (event->type() == QEvent::MouseButtonPress) {
            mousePress(static_cast<QMouseEvent *>(event));
            return true;
        }else if (event->type() == QEvent::MouseButtonDblClick) {
            mouseDoubleClick(static_cast<QMouseEvent *>(event));
            return true;
        }else if (event->type() == QEvent::MouseMove) {
            mouseMove(static_cast<QMouseEvent *>(event));
            return true;
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            mouseRelease(static_cast<QMouseEvent *>(event));
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}


void ChildGraphWin::resizeEvent(QResizeEvent *event)
{
    ui->scrollArea->widget()->setMinimumSize(this->m_PaintArea->geometry().width(), this->m_PaintArea->geometry().height());
    QWidget::resizeEvent(event);
}
