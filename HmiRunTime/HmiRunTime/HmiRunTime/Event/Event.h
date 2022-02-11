#ifndef EVENT_H
#define EVENT_H

#include <QVector>
#include <QVariant>
#include <QReadWriteLock>
#include <QDebug>

/**
 *  which event is?
 */
enum EventType {
    NONE = 0,
    BEFORE_READ,
    VALUE_CHANGE,

};
typedef EventType TEvent;

class RuntimeEvent
{
public:
    RuntimeEvent(void *pObj, TEvent e, void *pDataObj);
    ~RuntimeEvent();

public:

    void *getData();
    void *getObj();
    TEvent getType();

private:
    TEvent m_type;
    void *m_pDataObj;
    void *m_pObj;
};

class Listener
{
public:
    virtual void performed(RuntimeEvent e) = 0;

};

class ListenerRegister : public QObject
{
    Q_OBJECT

public:
    ListenerRegister(QObject* parent = NULL);
    ~ListenerRegister();

public:
    void addListener(Listener *pObj);
    void removeListener(Listener *pObj);
    void fireAEvent(RuntimeEvent e);

private:
    QVector<Listener*> m_listeners;
    QReadWriteLock lock;
};

class EventProducer : public QObject
{
    Q_OBJECT

public:
    EventProducer(QObject* parent = NULL);
    ~EventProducer();

public:
    void addListener(Listener *pObj);
    void removeListener(Listener *pObj);
    void fireAEvent(RuntimeEvent event);

private:
    ListenerRegister m_register;
};


#endif // EVENT_H
