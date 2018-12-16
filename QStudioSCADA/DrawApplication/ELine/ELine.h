#ifndef ELINE_H
#define ELINE_H

#include <QPoint>
#include "IElement.h"

class ELine : public IElement, public StdElement
{
public:
    ELine();
    ~ELine();

    //////////////////////////////////////////

public:
    // 加载元素数据
    void load(const QJsonObject &json);
    // 保存元素数据
    void save(QJsonObject &json);
    // 绘制图形元素
    void darw(QPainter *painter);
    // 元素是否选中
    bool isSelected();

    // 获取元素属性
    void GetElementProperty(QtTreePropertyBrowser *propertyBrowser, QtVariantPropertyManager *variantManager, QtVariantEditorFactory *variantFactory);
    // 属性值改变
    void PropertyValueChanged(QtProperty *property, const QVariant &value);

    ///////////////////////////////////////////

    // 鼠标光标是否在元素所在的区域内
    bool IsPointInRect(QPoint p);

public:
    // 单击事件响应
    void mousePress(QMouseEvent * event);
    // 双击事件响应
    void mouseDoubleClick(QMouseEvent * event);
    // 左键拖动响应
    void mouseMove(QMouseEvent * event);
    // 鼠标释放
    void mouseRelease(QMouseEvent * event);
    IElement* GetInterface(); //获取元素操作接口
    void setDefault(); // 设置缺省属性

public:

protected:
    bool event(QEvent *e) override;

public://private:
    QColor m_Color; // 直线颜色
    int m_iWidth; // 直线宽度
    int m_iLineStyle; // 直线类型
    int m_iX1; // 直线x终点坐标
    int m_iY1; // 直线y终点坐标


private:
    bool m_bDragEdit;
    int *m_pX;
    int *m_pY;
};

#endif // ELINE_H
