#include "ELine.h"
#include "PubTool.h"
#include "EditElementPropertyDialog.h"
#include <QPoint>
#include <QEvent>

#include <QDebug>

ELine::ELine()
{
    this->m_iGraphType = GRAPH_LINE;
    this->m_Color = Qt::black;
    this->m_iWidth = 1;
    this->m_iLineStyle = 1;
    this->m_iX = 0;
    this->m_iY = 0;
    this->m_iX1 = 42;
    this->m_iY1 = 26;
    this->m_iZ = 0;

    this->m_bDragEdit = false;
    this->m_pX = nullptr;
    this->m_pY = nullptr;
}


ELine::~ELine()
{

}

bool ELine::event(QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress){
        this->mousePress(static_cast<QMouseEvent *>(e));
        return true;
    }else if(e->type() == QEvent::MouseButtonDblClick){
        this->mouseDoubleClick(static_cast<QMouseEvent *>(e));
        return true;
    }else if(e->type() == QEvent::MouseMove){
        this->mouseMove(static_cast<QMouseEvent *>(e));
        return true;
    }else if(e->type() == QEvent::MouseButtonRelease){
        this->mouseRelease(static_cast<QMouseEvent *>(e));
        return true;
    }else{

    }
    return QObject::event(e);
}

/*
* 获取元素操作接口
*/
IElement* ELine::GetInterface()
{
    return static_cast<IElement *>(this);
}


/*
* 加载元素数据
*/
void ELine::load(const QJsonObject &json)
{
    this->StdLoad(json);
    this->m_iX1 = json["X1"].toInt();
    this->m_iY1 = json["Y1"].toInt();

    this->m_Color = QColor(json["color_red"].toInt(), json["color_green"].toInt(), json["color_blue"].toInt());

    int iLineStyle = json["line_style"].toInt();
    this->m_iLineStyle = (iLineStyle < 1) ? 1 : iLineStyle;
    int iWidth = json["line_width"].toInt();
    this->m_iWidth = (iWidth < 1) ? 1 : iWidth;
}


/*
* 保存元素数据
*/
void ELine::save(QJsonObject &json)
{
    this->StdSave(json);
    json["X1"] = this->m_iX1;
    json["Y1"] = this->m_iY1;

    json["color_red"] = this->m_Color.red();
    json["color_green"] = this->m_Color.green();
    json["color_blue"] = this->m_Color.blue();

    json["line_style"] = this->m_iLineStyle;
    json["line_width"] = this->m_iWidth;
}

/*
* 绘制图形元素
*/
void ELine::darw(QPainter *painter)
{
    QPen pen = QPen();
    QBrush brush = QBrush(Qt::NoBrush);
    painter->setBrush(brush);

    pen.setWidth(this->m_iWidth);
    pen.setColor(this->m_Color);
    pen.setStyle((Qt::PenStyle)this->m_iLineStyle);
    painter->setPen(pen);

    if(this->m_bIsSelected)
    {     
        painter->drawLine(this->m_iX, this->m_iY, this->m_iX1, this->m_iY1);
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DashLine);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawRect(this->m_iX, this->m_iY, this->m_iX1 - this->m_iX, this->m_iY1 - this->m_iY);

        PubTool::DrawFrameRect(painter, this->m_iX - 4, this->m_iY - 4, 8, 8, Qt::blue);
        PubTool::DrawFrameRect(painter, this->m_iX1 - 4, this->m_iY1 - 4, 8, 8, Qt::blue);
    }
    else
    {
        painter->drawLine(this->m_iX, this->m_iY, this->m_iX1, this->m_iY1);
    }

}


/*
* 单击事件响应
*/
void ELine::mousePress(QMouseEvent * event)
{
    QPoint pos = event->pos();
    // 鼠标光标是否在直线所在的区域内
    if(IsPointInRect(pos))
    {
        if(!this->m_bMouseLeftButtonDown)
        {
            this->m_ptMouseLeftButtonNew = this->m_ptMouseLeftButtonOld = event->pos();
            this->m_bMouseLeftButtonDown = true;
        }
        this->m_bIsSelected = true;

        this->m_bDragEdit = false;
        this->m_pX = nullptr;
        this->m_pY = nullptr;

        if(pos.x() >= (this->m_iX - 4) && pos.x() <= (this->m_iX + 4) &&
           pos.y() >= (this->m_iY - 4) && pos.y() <= (this->m_iY + 4))
        {
            this->m_bDragEdit = true;
            this->m_pX = &this->m_iX;
            this->m_pY = &this->m_iY;
        }


        if(pos.x() >= (this->m_iX1 - 4) && pos.x() <= (this->m_iX1 + 4) &&
           pos.y() >= (this->m_iY1 - 4) && pos.y() <= (this->m_iY1 + 4))
        {
            this->m_bDragEdit = true;
            this->m_pX = &this->m_iX1;
            this->m_pY = &this->m_iY1;
        }
    }
    else
    {
        this->m_bIsSelected = false;
        this->m_bDragEdit = false;
        this->m_pX = nullptr;
        this->m_pY = nullptr;
    }
}


/*
* 双击事件响应
*/
void ELine::mouseDoubleClick(QMouseEvent * event)
{
    Q_UNUSED(event)
    if(!this->m_bIsSelected)
        return;
    EditElementPropertyDialog *pDlg = new EditElementPropertyDialog();
    if(pDlg->exec() == QDialog::Accepted)
    {

    }

}


/*
* 左键拖动响应
*/
void ELine::mouseMove(QMouseEvent * event)
{
    Q_UNUSED(event)
    if(!this->m_bMouseLeftButtonDown || !this->m_bIsSelected)
        return;

    this->m_ptMouseLeftButtonNew = event->pos();

    int deta_x = this->m_ptMouseLeftButtonNew.x() - this->m_ptMouseLeftButtonOld.x();
    int deta_y = this->m_ptMouseLeftButtonNew.y() - this->m_ptMouseLeftButtonOld.y();

    if(this->m_bDragEdit)
    {
        *this->m_pX += deta_x;
        *this->m_pY += deta_y;
    }
    else
    {
        this->m_iX += deta_x;
        this->m_iX1 += deta_x;
        this->m_iY += deta_y;
        this->m_iY1 += deta_y;
    }

    this->m_ptMouseLeftButtonOld = this->m_ptMouseLeftButtonNew;
}


/*
* 鼠标释放
*/
void ELine::mouseRelease(QMouseEvent * event)
{
    Q_UNUSED(event)
    if(this->m_bMouseLeftButtonDown)
    {
        this->m_bMouseLeftButtonDown = false;
    }

}


/*
* 元素是否选中
*/
bool ELine::isSelected()
{
    return this->m_bIsSelected;
}


/*
* 获取元素属性
*/
void ELine::GetElementProperty(QtTreePropertyBrowser *propertyBrowser,
                               QtVariantPropertyManager *variantManager,
                               QtVariantEditorFactory *variantFactory)
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

    QtVariantProperty *propGraphType = variantManager->addProperty(QVariant::String, tr("图形类型"));
    propGraphType->setValue("线");
    addProperty(propertyBrowser, propGraphType, QLatin1String("图形类型"));

    QtVariantProperty *propName = variantManager->addProperty(QVariant::String, tr("图形名称"));
    propName->setValue(this->m_strName);
    addProperty(propertyBrowser, propName, QLatin1String("图形名称"));

    QtVariantProperty *propIsVisible = variantManager->addProperty(QVariant::Bool, tr("是否可见"));
    propIsVisible->setValue(this->m_bIsVisible);
    addProperty(propertyBrowser, propIsVisible, QLatin1String("是否可见"));

    // 颜色
    QtVariantProperty *lineColor = variantManager->addProperty(QVariant::Color, tr("线颜色"));
    lineColor->setValue(this->m_Color);
    addProperty(propertyBrowser, lineColor, QLatin1String("线颜色"));

    // 线型
    QtVariantProperty *lineStyle = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), tr("线型"));
    QStringList lineStyleNames;
    lineStyleNames << tr("实线") << tr("虚线") << tr("点线") << tr("双点线");
    lineStyle->setAttribute(QLatin1String("enumNames"), lineStyleNames);
    lineStyle->setValue(this->m_iLineStyle - 1);
    addProperty(propertyBrowser, lineStyle, QLatin1String("线型"));

    // 线宽
    QtVariantProperty *propWidth = variantManager->addProperty(QVariant::Int, tr("线宽"));
    propWidth->setValue(this->m_iWidth);
    addProperty(propertyBrowser, propWidth, QLatin1String("线宽"));

    QtVariantProperty *propX = variantManager->addProperty(QVariant::Int, tr("X坐标"));
    propX->setValue(this->m_iX);
    addProperty(propertyBrowser, propX, QLatin1String("X坐标"));

    QtVariantProperty *propY = variantManager->addProperty(QVariant::Int, tr("Y坐标"));
    propY->setValue(this->m_iY);
    addProperty(propertyBrowser, propY, QLatin1String("Y坐标"));

    QtVariantProperty *propZ = variantManager->addProperty(QVariant::Int, tr("Z坐标"));
    propZ->setValue(this->m_iZ);
    addProperty(propertyBrowser, propZ, QLatin1String("Z坐标"));

    QtVariantProperty *propX1 = variantManager->addProperty(QVariant::Int, tr("X1坐标"));
    propX1->setValue(this->m_iX1);
    addProperty(propertyBrowser, propX1, QLatin1String("X1坐标"));

    QtVariantProperty *propY1 = variantManager->addProperty(QVariant::Int, tr("Y1坐标"));
    propY1->setValue(this->m_iY1);
    addProperty(propertyBrowser, propY1, QLatin1String("Y1坐标"));

    propertyBrowser->setFactoryForManager(variantManager, variantFactory);
    propertyBrowser->setPropertiesWithoutValueMarked(true);
    propertyBrowser->setRootIsDecorated(false);

}


/*
* 插槽： 属性值改变
*/
void ELine::PropertyValueChanged(QtProperty *property, const QVariant &value)
{

    if (!propertyToId.contains(property))
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("图形类型"))
    {
        this->m_iGraphType = GRAPH_LINE;
    }
    else if (id == QLatin1String("图形名称"))
    {
        this->m_strName = value.toString();
    }
    else if (id == QLatin1String("是否可见"))
    {
        this->m_bIsVisible = value.toBool();
    }
    else if (id == QLatin1String("线颜色"))
    {
        this->m_Color = value.value<QColor>();
    }
    else if (id == QLatin1String("线型"))
    {
        this->m_iLineStyle = value.toInt() + 1;
    }
    else if (id == QLatin1String("线宽"))
    {
        this->m_iWidth = value.toInt();
    }
    else if (id == QLatin1String("X坐标"))
    {
        this->m_iX = value.toInt();
    }
    if (id == QLatin1String("Y坐标"))
    {
        this->m_iY =value.toInt();
    }
    else if (id == QLatin1String("Z坐标"))
    {
        this->m_iZ = value.toInt();
    }
    else if (id == QLatin1String("X1坐标"))
    {
        this->m_iX1 = value.toInt();
    }
    else if (id == QLatin1String("Y1坐标"))
    {
        this->m_iY1 = value.toInt();
    }

}

/*
* 鼠标光标是否在元素所在的区域内
*/
bool ELine::IsPointInRect(QPoint p)
{
    int top, left, right, bottom;

    left = (this->m_iX < this->m_iX1) ? this->m_iX : this->m_iX1;
    right = (this->m_iX > this->m_iX1) ? this->m_iX : this->m_iX1;
    top = (this->m_iY < this->m_iY1) ? this->m_iY : this->m_iY1;
    bottom = (this->m_iY > this->m_iY1) ? this->m_iY : this->m_iY1;

    if(p.x() >= (left - 4) && p.x() <= (right + 4) &&
       p.y() >= (top - 4) && p.y() <= (bottom + 4))
    {
        return true;
    }
    return false;
}

/*
* 设置缺省属性
*/
void ELine::setDefault()
{
    this->m_iX1 = this->m_iX + 42;
    this->m_iY1 = this->m_iY + 26;

}

