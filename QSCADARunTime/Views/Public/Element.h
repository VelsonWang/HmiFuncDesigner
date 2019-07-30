#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include <QWidget>
#include "PublicDefine.h"
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>
#include <QTimer>

class Element : public QObject
{
    Q_OBJECT
public:
    explicit Element(QObject *parent = nullptr);
    virtual ~Element();

    void init();

    void setAngle(qreal angle);
    qreal angle() const;

    virtual void setClickPosition(QPointF) = 0;
    virtual void updateBoundingElement() = 0;
    virtual void readFromXml(const QXmlStreamAttributes &) = 0;  
    virtual void readData(QDataStream &in) = 0;
    virtual void paint(QPainter *painter) = 0;
    virtual void mousePressEvent(QMouseEvent *event) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *event) = 0;

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

    QString getInternalElementType() const;

    void moveTo(int x,int y);

    // 设置工程路径
    void setProjectPath(const QString &path);
    // 获取工程路径
    QString getProjectPath() const;

    // 设置元素所属窗口
    void setOwnerWidget(QWidget *owner);
    // 获取元素所属窗口
    QWidget *getOwnerWidget();

    // 隐藏控件
    void hideElement();
    // 显示控件
    void showElement();
    // 生效控件
    void enableElement();
    // 失效控件
    void disableElement();
    // 闪烁控件
    void startBlinkElement();
    // 停止闪烁控件
    void stopBlinkElement();

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
    QString internalElementType;
    QString strProjectPath_; // 工程路径
    QWidget *ownerWidget_;
    bool bShow_; // 显示
    bool bEnable_; // 生效
    QTimer blinkTimer_;  // 闪烁定时器

signals:
    void elementMoved(QPointF);
    void elementResized(int,int,QPointF);

private slots:
    virtual void blinkTimeOut();

};

#endif // ELEMENT_H
