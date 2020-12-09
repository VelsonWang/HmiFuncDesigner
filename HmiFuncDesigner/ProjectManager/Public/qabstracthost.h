#ifndef QABSTRACTHOST_H
#define QABSTRACTHOST_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QAction>
#include <QTimer>
#include <QUndoStack>

#define OBJECT_TITLE   "Object"
#define  HOST_TYPE      "type"

class QAbstractHost : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractHost(QAbstractHost *parent = Q_NULLPTR);
    ~QAbstractHost();

    void insertChildren(const QList<int> &indexs, const QList<QAbstractHost*> &children);
    void removeChildren(const QList<QAbstractHost*> &children);

    void setParent(QAbstractHost* parent, int index);

    QList<QAbstractHost*> getChildren();
    QAbstractHost* getChild(int index);
    QAbstractHost* getParent();
    int getChildCount();
    void setDefault();

    void setAttribute(const QString& key, const QString& value);
    QString getAttribute(const QString& key);

    QList<QAction*> getActions();
    QAction* getAction(const QString& name);

    void clear();
    void init();

    QObject* getObject();
    virtual void createObject();

    QString getUuid();
    void setUuid(const QString& uuid);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

    QString getHostType();

protected:
    void insertAction(const QString& name,QAction* ac,int index=-1);
    void removeAction(const QString& name);
    bool eventFilter(QObject *, QEvent *);

protected:
    virtual bool doPaintEvent(QPaintEvent* event);
    virtual bool doFocusInEvent(QFocusEvent *event);
    virtual bool doFocusOutEvent(QFocusEvent *event);
    virtual bool doMouseReleaseEvent(QMouseEvent* event);
    virtual bool doMousePressEvent(QMouseEvent* event);
    virtual bool doMouseMoveEvent(QMouseEvent* event);
    virtual bool doDoubleClickedEvent(QMouseEvent* event);
    virtual bool doContextMenuEvent(QContextMenuEvent *event);


signals:
    void showWidget(QWidget* wid);
    void showDialog(QAbstractHost* host);

    void insertChildrenSignal(const QList<QAbstractHost*> &host,const QList<int> &index);
    void removeChildrenSignal(const QList<QAbstractHost*> &host);
    void parentChanged();

protected:
    QList<QAbstractHost*> m_children;
    QAbstractHost* m_parent;
    QList<QAction*> m_actions;
    QMap<QString, QAction*> m_nameToAction;
    QObject* m_object;
    QMap<QString, QString> m_attributes;
};

#endif // QABSTRACTHOST_H
