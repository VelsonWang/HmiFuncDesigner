#ifndef ELEMENTMOVINGTEXT_H
#define ELEMENTMOVINGTEXT_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementMovingText : public Element
{
    Q_OBJECT

public:
    explicit ElementMovingText(const QString &szProjPath,
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

    QString getMoveDirString(const QString& szAlign) const;
    void setMoveDirString(const QString& szAlign, QString& szAlignSet);

    int type() const override {
        return MovingTextItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementMovingText &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementMovingText &textItem);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    void drawMovingText(QPainter *painter);

private:
    QStringList tagNames_;
    QStringList moveDirList_;

private:
    static int iLastIndex_;
    // 关联的变量
    QString szTagSelected_;
    // 变量文本列表
    QStringList tagTextList_;
    // 移动方向
    QString szMoveDir_;
    // 移动字符数
    int iMoveCharNum_;
    // 移动间隔
    double period_;
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
    // 初始可见性
    bool showOnInitial_;
};


#endif // ELEMENTMOVINGTEXT_H
