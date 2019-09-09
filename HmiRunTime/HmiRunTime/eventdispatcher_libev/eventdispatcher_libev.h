#ifndef EVENTDISPATCHER_LIBEV_H
#define EVENTDISPATCHER_LIBEV_H

#include <QtCore/QAbstractEventDispatcher>

class EventDispatcherLibEvPrivate;

class EventDispatcherLibEv : public QAbstractEventDispatcher
{
	Q_OBJECT
public:
	explicit EventDispatcherLibEv(QObject* parent = 0);
	virtual ~EventDispatcherLibEv(void);

	virtual bool processEvents(QEventLoop::ProcessEventsFlags flags);
	virtual bool hasPendingEvents(void);

	virtual void registerSocketNotifier(QSocketNotifier* notifier);
	virtual void unregisterSocketNotifier(QSocketNotifier* notifier);

	virtual void registerTimer(int timerId,int interval,
                                Qt::TimerType timerType,QObject* object);

	virtual bool unregisterTimer(int timerId);
	virtual bool unregisterTimers(QObject* object);
	virtual QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject* object) const;
	virtual int remainingTime(int timerId);

	virtual void wakeUp(void);
	virtual void interrupt(void);
	virtual void flush(void);

protected:
	EventDispatcherLibEv(EventDispatcherLibEvPrivate& dd, QObject* parent = 0);

private:
	Q_DISABLE_COPY(EventDispatcherLibEv)
	Q_DECLARE_PRIVATE(EventDispatcherLibEv)
	QScopedPointer<EventDispatcherLibEvPrivate> d_ptr;
};

#endif // EVENTDISPATCHER_LIBEV_H
