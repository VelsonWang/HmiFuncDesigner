#ifndef SWITCHBUTTONITEM_H
#define SWITCHBUTTONITEM_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>


class ElementSwitchButton : public Element
{
    Q_OBJECT

public:
    explicit ElementSwitchButton(const QString &szProjPath,
                                 const QString &szProjName,
                                 QtVariantPropertyManager *propertyMgr);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(QtProperty *property, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    int type() const override {
        return SwitchButtonItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementSwitchButton &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementSwitchButton &textItem);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    void drawSwitchButton(QPainter *painter);
    void getSupportEvents(QStringList &listValue);

    void reloadPropertyList();
    void updatePropertyEditor();

private:
    QStringList tagNames_;
    QStringList hAlignList_;
    QStringList vAlignList_;
    QStringList contents_;

private:
    static int iLastIndex_;
    QStringList funcs_;
    // 关联的变量
    QString szTagSelected_;
    // 初始状态
    bool stateOnInitial_;
    // 显示内容
    QString showContent_;
    bool bShowContentText_;

    // 复位图片
    QString resetPictureFile_;
    // 置位图片
    QString setPictureFile_;
    // 原尺寸显示
    bool showNoScale_;

    // 复位显示文本
    QString resetText_;
    // 置位显示文本
    QString setText_;

    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 复位按钮背景颜色
    QColor resetBackgroundColor_;
    // 置位按钮背景颜色
    QColor setBackgroundColor_;
    // 透明背景颜色
    bool transparent_;
    // 字体
    QFont font_;
    // 初始有效性
    bool enableOnInitial_;
    // 初始可见性
    bool showOnInitial_;
};

#endif // SWITCHBUTTONITEM_H
