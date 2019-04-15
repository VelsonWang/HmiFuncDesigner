#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QCursor>
#include "PublicDefine.h"
#include "propertymodel.h"
#include "boolproperty.h"
#include "emptyproperty.h"
#include "colorproperty.h"
#include "textproperty.h"
#include "integerproperty.h"
#include "fileproperty.h"
#include "listproperty.h"
#include "fontproperty.h"
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

class Element : public QGraphicsObject
{
    Q_OBJECT
public:
    Element(const QString &projPath);
    virtual ~Element();

    void init();

    void setAngle(qreal angle);
    qreal angle() const;

    virtual void setClickPosition(QPointF) = 0;
    virtual void updateElementProperty(uint id, const QVariant &value) = 0;
    virtual void updateBoundingElement() = 0;
    QList <Property*> getPropertyList() const;
    virtual void updatePropertyModel() = 0;
    virtual void createPropertyList() = 0;
    virtual void writeAsXml(QXmlStreamWriter &) = 0;
    virtual void readFromXml(const QXmlStreamAttributes &) = 0;
    virtual void setBlocked(bool);
    virtual void addNodePoint();
    virtual void removeNodePoint();

    virtual void writeData(QDataStream &out) = 0;
    virtual void readData(QDataStream &in) = 0;

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

    void setGraphPageLink(const QString &);
    QString getGraphPageLink() const;

    void moveTo(int x,int y);

    // 设置工程路径
    void setProjectPath(const QString &path);
    // 获取工程路径
    QString getProjectPath() const;
    // 获取分配的索引值
    int getIndexFromIDString(const QString &szID);
    QString getHAlignString(const QString& szAlign) const;
    void setHAlignString(const QString& szAlign, QString& szAlignSet);
    QString getVAlignString(const QString& szAlign) const;
    void setVAlignString(const QString& szAlign, QString& szAlignSet);

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
    QList <Property*> propList;

    enum ResizeDirection{
        RdBottomRight,
        RdTopLeft,
        RdNone
    };

    ResizeDirection rd;

    QString strProjectPath_; // 工程路径

signals:
    void elementMoved(QPointF);
    void elementResized(int,int,QPointF);
};

#endif // ELEMENT_H
