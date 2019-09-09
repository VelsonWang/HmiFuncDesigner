#ifndef EVENTDISPATCHER_LIBEV_P_H
#define EVENTDISPATCHER_LIBEV_P_H

#include <qplatformdefs.h>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QHash>
#include <QtCore/QMultiHash>
#include <QtCore/QPointer>
#include <ev.h>
#include <QtCore/QAtomicInt>

struct TimerInfo {
	QObject* object;
	struct ev_timer ev;
	struct timeval when;
	int timerId;
	int interval;
	Qt::TimerType type;
};

struct ZeroTimer {
	QObject* object;
	bool active;
};

Q_DECLARE_TYPEINFO(TimerInfo, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(ZeroTimer, Q_PRIMITIVE_TYPE);

ev_tstamp calculateNextTimeout(TimerInfo* info, const struct timeval& now);

class EventDispatcherLibEv;

class  EventDispatcherLibEvPrivate
{
public:
    EventDispatcherLibEvPrivate(EventDispatcherLibEv * const q);
	~EventDispatcherLibEvPrivate(void);
	bool processEvents(QEventLoop::ProcessEventsFlags flags);
	bool processZeroTimers(void);
	void registerSocketNotifier(QSocketNotifier* notifier);
	void unregisterSocketNotifier(QSocketNotifier* notifier);
	void registerTimer(int timerId, int interval, Qt::TimerType type, QObject* object);
	void registerZeroTimer(int timerId, QObject* object);
	bool unregisterTimer(int timerId);
	bool unregisterTimers(QObject* object);
	QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject* object) const;
	int remainingTime(int timerId) const;

	typedef QMultiHash<int, struct ev_io*> SocketNotifierHash;
	typedef QHash<int, TimerInfo*> TimerHash;
	typedef QPair<QPointer<QObject>, QEvent*> PendingEvent;
	typedef QList<PendingEvent> EventList;
	typedef QHash<int, ZeroTimer> ZeroTimerHash;

private:
	Q_DISABLE_COPY(EventDispatcherLibEvPrivate)
	Q_DECLARE_PUBLIC(EventDispatcherLibEv)
	EventDispatcherLibEv* const q_ptr;

	bool m_interrupt;
	struct ev_loop* m_base;
	struct ev_async m_wakeup;
	QAtomicInt m_wakeups;
	SocketNotifierHash m_notifiers;
	TimerHash m_timers;
	EventList m_event_list;
	ZeroTimerHash m_zero_timers;
	bool m_awaken;

	static void socket_notifier_callback(struct ev_loop* loop, struct ev_io* w, int revents);
	static void timer_callback(struct ev_loop* loop, struct ev_timer* w, int revents);
	static void wake_up_handler(struct ev_loop* loop, struct ev_async* w, int revents);

	bool disableSocketNotifiers(bool disable);
	void killSocketNotifiers(void);
	bool disableTimers(bool disable);
	void killTimers(void);
};

#endif // EVENTDISPATCHER_LIBEV_P_H
