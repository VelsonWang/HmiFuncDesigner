#ifndef INPUTEDITITEM_H
#define INPUTEDITITEM_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementInputEdit : public Element
{
    Q_OBJECT

public:
    explicit ElementInputEdit(const QString &szProjPath,
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
        return InputEditItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementInputEdit &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementInputEdit &textItem);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    void drawInputEdit(QPainter *painter);

private:
    QStringList tagNames_;
    QStringList hAlignList_;
    QStringList vAlignList_;

private:
    static int iLastIndex_;
    // 允许编辑输入
    bool enableEdit_;
    // 关联的变量
    QString szTagSelected_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 字体
    QFont font_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 密码输入
    bool inputPassword_;
    // 初始有效性
    bool enableOnInitial_;
    // 初始可见性
    bool showOnInitial_;

};

#endif // INPUTEDITITEM_H
