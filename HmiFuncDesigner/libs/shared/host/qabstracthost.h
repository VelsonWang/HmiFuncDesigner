#ifndef QABSTRACTHOST_H
#define QABSTRACTHOST_H

#include "../sharedlibglobal.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QAction>
#include <QTimer>
#include <QUndoStack>

#define OBJECT_TITLE   "Object"
#define  HOST_TYPE      "type"

class QAbstractProperty;
class XMLObject;
class QPageManager;
class QScriptEngine;
class QDataManager;
class QResourceManager;

class SHAREDLIB_EXPORT QAbstractHost : public QObject
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

    QList<QAbstractProperty*> getPropertys();
    QAbstractProperty* getProperty(const QString &name);

    QVariant getPropertyValue(const QString &name);
    void setPropertyValue(const QString &name, const QVariant &value);

    void setDefault();

    void setAttribute(const QString& key, const QString& value);
    QString getAttribute(const QString& key);

    QList<QAction*> getActions();
    QAction* getAction(const QString& name);

    QObject* getObject();

    void toObject(XMLObject* xml);
    void fromObject(XMLObject* xml);

    void clear();
    void init();

    virtual void createObject();

    QString getUuid();
    void setUuid(const QString& uuid);

    void setPageManager(QPageManager* page);
    void setDataManager(QDataManager* data);
    void setResourceManager(QResourceManager* resource);

    virtual QString functionInformation(const QString &name);

    QScriptEngine* getScriptEngine();

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

    void makeStyleSheet();
    QString getHostType();

protected:
    void insertProperty(QAbstractProperty* property, int index=-1);
    void removeProperty(const QString &name);

    void insertAction(const QString& name, QAction* ac, int index=-1);
    void removeAction(const QString& name);

    virtual void initProperty();
    bool eventFilter(QObject *, QEvent *);

    void exec(const QString &code, const QMap<QString, QString> &param);

protected:
    virtual bool handlePaintEvent(QPaintEvent* event);
    virtual bool handleFocusInEvent(QFocusEvent *event);
    virtual bool handleFocusOutEvent(QFocusEvent *event);
    virtual bool handleMouseReleaseEvent(QMouseEvent* event);
    virtual bool handleMousePressEvent(QMouseEvent* event);
    virtual bool handleMouseMoveEvent(QMouseEvent* event);
    virtual bool handleDoubleClickedEvent(QMouseEvent* event);
    virtual bool handleContextMenuEvent(QContextMenuEvent *event);

protected slots:
    void onPropertyRefresh();
    void onCurTextChanged(const QString &uuid);

signals:
    void notifyShowWidget(QWidget* wid);
    void notifyShowDialog(QAbstractHost* host);

    void notifyInsertChildren(const QList<QAbstractHost*> &host, const QList<int> &index);
    void notifyRemoveChildren(const QList<QAbstractHost*> &host);
    void notifyParentChanged();

public slots:
    
protected:
    QList<QAbstractHost*> m_children;
    QAbstractHost* m_parent;
    QList<QAbstractProperty*> m_propertys;
    QMap<QString, QAbstractProperty*> m_nameToProperty;
    QList<QAction*> m_actions;
    QMap<QString, QAction*> m_nameToAction;
    QObject* m_object;
    QMap<QString, QString> m_attributes;
    QTimer* m_timer;
    QPageManager* m_page_manager;
    QDataManager* m_data_manager;
    QScriptEngine* m_engine;
    QResourceManager* m_resource_manager;
};

#endif // QABSTRACTHOST_H
