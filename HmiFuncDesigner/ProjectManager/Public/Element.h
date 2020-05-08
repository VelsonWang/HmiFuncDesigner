#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QCursor>
#include "PublicDefine.h"
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

class Element : public QGraphicsObject
{
    Q_OBJECT
public:
    Element(const QString &szProjPath,
            const QString &szProjName,
            QtVariantPropertyManager *propertyMgr);
    virtual ~Element();

    void init();

    void setAngle(qreal angle);
    qreal angle() const;

    virtual void setClickPosition(QPointF) = 0;
    virtual void updateElementProperty(QtProperty *property, const QVariant &value) = 0;
    virtual void updateBoundingElement() = 0;
    QList<QtProperty*> getPropertyList() const;
    virtual void updatePropertyModel() = 0;
    virtual void createPropertyList() = 0;
    virtual void writeAsXml(QXmlStreamWriter &) = 0;
    virtual void readFromXml(const QXmlStreamAttributes &) = 0;
    virtual void setBlocked(bool);
    virtual void writeData(QDataStream &out) = 0;
    virtual void readData(QDataStream &in) = 0;
    virtual void regenerateElementId() = 0;
    virtual void release() = 0; // 释放占用的资源

    void setElementId(const QString &);
    QString getElementId() const;

    void setElementWidth(int);
    int getElementWidth() const;

    void setElementHeight(int);
    int getElementHeight() const;

    void setElementXPos(int);
    int getElementXPos() const;

    void setElementYPos(int);
    int getElementYPos() const;

    void setElementZValue(int);
    int getElementZValue() const;

    QIcon getElementIcon() const;

    QString getInternalElementType() const;

    void moveTo(int x,int y);

    // 设置工程路径
    void setProjectPath(const QString &path);
    // 获取工程路径
    QString getProjectPath() const;

    // 设置工程名称
    void setProjectName(const QString &name);
    // 获取工程名称
    QString getProjectName() const;

    // 获取分配的索引值
    int getIndexFromIDString(const QString &szID);
    QString getHAlignString(const QString& szAlign) const;
    void setHAlignString(const QString& szAlign, QString& szAlignSet);
    QString getVAlignString(const QString& szAlign) const;
    void setVAlignString(const QString& szAlign, QString& szAlignSet);

    // 设置所属画面大小
    void setGraphPageSize(int width, int height);
    // 限制矩形区域
    void RestrictedRectangularRegion();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    // 绘制选中状态
    virtual void paintSelected(QPainter *painter, int iLineWidth);


protected:
    QString elementId;
    QColor backgroundColor;
    QColor signBackgroundColor;
    QColor borderColor;
    int borderWidth;
    int elementWidth;
    int elementHeight;
    int elementXPos;
    int elementYPos;
    int elementZValue;
    QString elementText;
    QColor textColor;
    int fontSize;
    qreal elemAngle;

    QIcon elementIcon;
    QString internalElementType;

    bool resizing;
    int rotationCount;
    QPointF oldPos;
    int oldWidth;
    int oldHeight;
    QRectF handleTop;
    QRectF handleBottom;
    QList<QtProperty*> propList;

    QRectF elementRect;

    enum ResizeDirection {
        ResizeNone = 0,
        ResizeLeft = 1,
        ResizeRight = 2,
        ResizeTop = 4,
        ResizeBottom = 8,
        AllDirections = 15,
    };

    quint32 rd;

    QString szProjectPath_; // 工程路径
    QString szProjectName_; // 工程名称
    int iGraphPageWidth_; // 画面宽度
    int iGraphPageHeight_; // 画面高度

signals:
    void elementMoved(QPointF);
    void elementResized(int,int,QPointF);


public:
    void addProperty(QtVariantProperty *property, const QString &id, bool bAddToList = true);
    void clearProperties();

    QtVariantPropertyManager *variantPropertyManager_;
    QMap<QtProperty *, QString> propertyToId_;
    QMap<QString, QtVariantProperty *> idToProperty_;
    QMap<QString, bool> idToExpanded_;

};

#endif // ELEMENT_H
