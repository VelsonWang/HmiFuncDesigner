#ifndef ERECTANGLE_H
#define ERECTANGLE_H

#include "./IElement.h"



class ERectangle : public IElement, public StdElement
{
public:
    ERectangle();
    ~ERectangle();


    //////////////////////////////////////////

public:
    // 加载元素数据
    void load(QDomDocument &doc, QDomElement &parent);
    // 保存元素数据
    void save(QDomDocument &doc, QDomElement &parent);
    // 绘制图形元素
    void darw(QPainter &painter);
    // 单击事件响应
    void mousePress(QMouseEvent * event);
    // 双击事件响应
    void mouseDoubleClick(QMouseEvent * event);
    // 左键拖动响应
    void mouseMove(QMouseEvent * event);
    // 鼠标释放
    void mouseRelease(QMouseEvent * event);
    // 元素是否选中
    bool isSelected();

    // 获取元素属性
    void GetElementProperty(QtTreePropertyBrowser *propertyBrowser, QtVariantPropertyManager *variantManager, QtVariantEditorFactory *variantFactory);
    // 属性值改变
    void PropertyValueChanged(QtProperty *property, const QVariant &value);

    ///////////////////////////////////////////


 public:
    IElement* GetInterface(); //获取元素操作接口

 private:



};

#endif // ERECTANGLE_H
