#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QCursor>
#include "itemtypes.h"
#include "propertyeditor/propertymodel.h"
#include "propertyeditor/boolproperty.h"
#include "propertyeditor/emptyproperty.h"
#include "propertyeditor/colorproperty.h"
#include "propertyeditor/textproperty.h"
#include "propertyeditor/integerproperty.h"
#include "DrawAppDefs.h"
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

class Element : public QGraphicsObject
{
    Q_OBJECT
public:
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

    /** Properties*/
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

    void setMessageType(const QString &);
    QString getMessageType() const;

    void setIndicationRule(const QString &);
    QString getIndicationRule() const;

    void setLinkingType(const QString &mLinkingType);
    QString getLinkingType() const;

    void setDeviceLink(const QString &mDeviceLink);
    QString getDeviceLink() const;

    void setSignalLink(const QString &mSignalLink);
    QString getSignalLink() const;

    void moveTo(int x,int y);

protected:
    QString elementId;
    QString graphPageLink;
    QString messageType;
    QColor backgroundColor;
    QColor signBackgroundColor;
    QColor borderColor;
    int borderWidth;
    int elementWidth;
    int elementHeight;
    int elementXPos;
    int elementYPos;
    int elementZValue;
    bool block;
    QString elementText;
    QColor textColor;
    int fontSize;
    qreal elemAngle;
    QString indicationRule;
    QString linkingType;
    QString deviceLink;
    QString signalLink;

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

signals:
    void elementMoved(QPointF);
    void elementResized(int,int,QPointF);
};

#endif // ELEMENT_H
