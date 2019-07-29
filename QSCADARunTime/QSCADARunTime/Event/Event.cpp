
#include "Event.h"

RuntimeEvent::RuntimeEvent(void *pObj, TEvent e, QVariant data)
{
    m_pObj = pObj;
    m_type = e;
    m_data = data;
}

RuntimeEvent::~RuntimeEvent()
{

}

QVariant RuntimeEvent::getData()
{
    return m_data;
}

void *RuntimeEvent::getObj()
{
    return m_pObj;
}

TEvent RuntimeEvent::getType()
{
    return m_type;
}




ListenerRegister::ListenerRegister(QObject* parent):
    QObject(parent)
{

}

ListenerRegister::~ListenerRegister()
{
    qDeleteAll(m_listeners);
    m_listeners.clear();
}


void ListenerRegister::addListener(Listener *pObj)
{
    if (pObj == NULL)
        return;
    m_listeners.append(pObj);
}

void ListenerRegister::removeListener(Listener *pObj)
{
    if (pObj == NULL)
        return;
    m_listeners.removeOne(pObj);
}

void ListenerRegister::fireAEvent(RuntimeEvent e)
{
    QVector<Listener*> tmpListeners;
    lock.lockForWrite();
    foreach (Listener *pListener, m_listeners)
    {
        tmpListeners.append(pListener);
    }
    lock.unlock();
    foreach (Listener *pListener, tmpListeners)
    {
        pListener->performed(e);
    }
}



EventProducer::EventProducer(QObject* parent):
    QObject(parent)
{

}

EventProducer::~EventProducer()
{

}

void EventProducer::addListener(Listener *pObj)
{
    m_register.addListener(pObj);
}

void EventProducer::removeListener(Listener *pObj)
{
    m_register.removeListener(pObj);
}

void EventProducer::fireAEvent(RuntimeEvent event)
{
    m_register.fireAEvent(event);
}



/*
class testObj1 : public EventProducer
{
public:
    void doEvent(QString msg)
    {
        RuntimeEvent event(this, VALUE_CHANGE, QVariant(msg));
        fireAEvent(event);
    }
};

class testObj1_Listener : public Listener
{
public:
    void performed(RuntimeEvent e)
    {
        if(e.getType() == VALUE_CHANGE)
        {
            qDebug()<<e.getData().toString();
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    testObj1 *pObj = new testObj1();
    testObj1_Listener *pListener = new testObj1_Listener();

    pObj->addListener(pListener);
    pObj->doEvent("lalala");
    pObj->doEvent("xxxxxx");
    pObj->doEvent("oooooo");

    return 0;
}
*/


