#ifndef IELEMENT_H
#define IELEMENT_H

#include <QJsonObject>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QEvent>
#include <QMoveEvent>
#include <QWidget>
#include <QVariant>
#include <QtProperty>
#include <QMap>

#include "PublicDefine.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"


/*
* 元素操作接口
*/
class IElement
{
public:
    // 加载元素数据
    virtual void load(const QJsonObject &json) = 0;
    // 保存元素数据
    virtual void save(QJsonObject &json) = 0;
    // 绘制图形元素
    virtual void darw(QPainter *painter) = 0;
    // 元素是否选中
    virtual bool isSelected() = 0;

    // 获取元素属性
    virtual void GetElementProperty(QtTreePropertyBrowser *propertyBrowser, QtVariantPropertyManager *variantManager, QtVariantEditorFactory *variantFactory) = 0;
    // 属性值改变
    virtual void PropertyValueChanged(QtProperty *property, const QVariant &value) = 0;



};

/*
* 元素父类
*/
class StdElement : public QObject
{
    Q_OBJECT

public:
    StdElement();
    virtual ~StdElement();

public:
    virtual IElement* GetInterface(){} //获取元素操作接口   
    virtual void setDefault() = 0; // 设置缺省属性

public:
    tGraphType m_iGraphType; // 图形类型
    QString m_strName; // 图形名称
    bool m_bIsVisible; // 是否可见
    int m_iX; // 元素x坐标
    int m_iY; // 元素y坐标
    int m_iZ; // 元素Z坐标

public:
    bool m_bIsSelected; // 元素是否选中
    bool m_bMouseLeftButtonDown; //鼠标左键是否按下
    QPoint m_ptMouseLeftButtonNew; //鼠标左键按下新位置点
    QPoint m_ptMouseLeftButtonOld; // 鼠标左键按下老位置点

public:
    // 加载元素数据
    void StdLoad(const QJsonObject &json);
    // 保存元素数据
    void StdSave(QJsonObject &json);

    /////////////< 属性操作 >////////////////
public:
    QMap<QtProperty *, QString> propertyToId;
    QMap<QString, QtVariantProperty *> idToProperty;
    QMap<QString, bool> idToExpanded;

public:
    // 增加属性
    void addProperty(QtTreePropertyBrowser *propertyBrowser, QtVariantProperty *property, const QString &id);


protected:
    virtual bool event(QEvent *e);


};


#endif // IELEMENT_H

