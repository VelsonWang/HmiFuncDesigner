#ifndef PUSHBUTTONITEM_HPP
#define PUSHBUTTONITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>


class ElementPushButton : public Element
{
    Q_OBJECT

public:
    explicit ElementPushButton(const QString &szProjPath,
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
        return PushButtonItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementPushButton &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementPushButton &textItem);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    void drawPushButton(QPainter *painter);
    void getSupportEvents(QStringList &listValue);

    void reloadPropertyList();
    void updatePropertyEditor();

private:
    QStringList hAlignList_;
    QStringList vAlignList_;
    QStringList contents_;

private:
    static int iLastIndex_;
    QStringList funcs_;
    // JavaScript脚本
    QString script_;
    // 显示内容
    QString showContent_;
    bool bShowContentText_;
    // 图片名
    QString filePicture_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 按钮背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparent_;
    // 字体
    QFont font_;
    // 初始有效性
    bool enableOnInitial_;
    // 初始可见性
    bool showOnInitial_;
};

#endif // PUSHBUTTONITEM_HPP
