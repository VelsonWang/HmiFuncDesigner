#include <QtCore/QCoreApplication>
#include "eventdispatcher_libev.h"
#include "eventdispatcher_libev_p.h"

EventDispatcherLibEvPrivate::EventDispatcherLibEvPrivate(EventDispatcherLibEv* const q)
	: q_ptr(q), m_interrupt(false), m_base(0), m_wakeup(),m_wakeups(),
	  m_notifiers(), m_timers(), m_event_list(), m_zero_timers(), m_awaken(false)
{
	this->m_base = ev_loop_new(EVFLAG_AUTO);
	Q_CHECK_PTR(this->m_base != 0);

	ev_set_userdata(this->m_base, this);

	struct ev_async* tmp = &this->m_wakeup;
	ev_async_init(tmp, EventDispatcherLibEvPrivate::wake_up_handler);
	ev_async_start(this->m_base, &this->m_wakeup);
}

EventDispatcherLibEvPrivate::~EventDispatcherLibEvPrivate(void)
{
	if (this->m_base) {
		ev_async_stop(this->m_base, &this->m_wakeup);

		this->killTimers();
		this->killSocketNotifiers();

		ev_loop_destroy(this->m_base);
		this->m_base = 0;
	}
}

bool EventDispatcherLibEvPrivate::processEvents(QEventLoop::ProcessEventsFlags flags)
{
	Q_Q(EventDispatcherLibEv);

	const bool exclude_notifiers = (flags & QEventLoop::ExcludeSocketNotifiers);
	const bool exclude_timers    = (flags & QEventLoop::X11ExcludeTimers);

	exclude_notifiers && this->disableSocketNotifiers(true);
	exclude_timers    && this->disableTimers(true);

	this->m_interrupt = false;
	this->m_awaken    = false;

	bool result = q->hasPendingEvents();

	Q_EMIT q->awake();
	QCoreApplication::sendPostedEvents();

	bool can_wait = !this->m_interrupt && (flags & QEventLoop::WaitForMoreEvents) && !result;
	int f         = EVLOOP_NONBLOCK;

	if (!this->m_interrupt) {
		if (!exclude_timers && this->m_zero_timers.size() > 0) {
			result |= this->processZeroTimers();
			if (result) {
				can_wait = false;
			}
		}

		if (can_wait) {
			Q_EMIT q->aboutToBlock();
			f = EVLOOP_ONESHOT;
		}

		// Work around a bug when libev returns from ev_loop(loop, EVLOOP_ONESHOT) without processing any events
		do {
			ev_loop(this->m_base, f);
		} while (can_wait && !this->m_awaken && !this->m_event_list.size());

		EventList list;
		this->m_event_list.swap(list);

		result |= (list.size() > 0) | this->m_awaken;

		for (int i=0; i<list.size(); ++i) {
			const PendingEvent& e = list.at(i);
			if (!e.first.isNull()) {
				QCoreApplication::sendEvent(e.first, e.second);
			}
		}

		struct timeval now;
		gettimeofday(&now, 0);

		// Now that all event handlers have finished (and we returned from the recusrion), reactivate all pending timers
		for (int i=0; i<list.size(); ++i) {
			const PendingEvent& e = list.at(i);
			if (!e.first.isNull() && e.second->type() == QEvent::Timer) {
				QTimerEvent* te = static_cast<QTimerEvent*>(e.second);
				TimerHash::Iterator tit = this->m_timers.find(te->timerId());
				if (tit != this->m_timers.end()) {
					TimerInfo* info = tit.value();

					struct ev_timer* tmp = &info->ev;
					if (!ev_is_pending(tmp) && !ev_is_active(tmp)) { // false in tst_QTimer::restartedTimerFiresTooSoon()
						ev_tstamp delta = calculateNextTimeout(info, now);
						ev_timer_set(tmp, delta, 0);
						ev_timer_start(this->m_base, tmp);
					}
				}
			}

			delete e.second;
		}
	}

	exclude_notifiers && this->disableSocketNotifiers(false);
	exclude_timers    && this->disableTimers(false);

	return result;
}

bool EventDispatcherLibEvPrivate::processZeroTimers(void)
{
	bool result    = false;
	QList<int> ids = this->m_zero_timers.keys();

	for (int i=0; i<ids.size(); ++i) {
		int tid = ids.at(i);
		ZeroTimerHash::Iterator it = this->m_zero_timers.find(tid);
		if (it != this->m_zero_timers.end()) {
			ZeroTimer& data = it.value();
			if (data.active) {
				data.active = false;

				QTimerEvent event(tid);
				QCoreApplication::sendEvent(data.object, &event);
				result   = true;

				it = this->m_zero_timers.find(tid);
				if (it != this->m_zero_timers.end()) {
					ZeroTimer& data = it.value();
					if (!data.active) {
						data.active = true;
					}
				}
			}
		}
	}

	return result;
}

void EventDispatcherLibEvPrivate::wake_up_handler(struct ev_loop* loop, struct ev_async* w, int revents)
{
	Q_UNUSED(w)
	Q_UNUSED(revents)

	EventDispatcherLibEvPrivate* disp = reinterpret_cast<EventDispatcherLibEvPrivate*>(ev_userdata(loop));
	disp->m_awaken = true;

	if (!disp->m_wakeups.testAndSetRelease(1, 0)) {
		qCritical("%s: internal error, wakeUps.testAndSetRelease(1, 0) failed!", Q_FUNC_INFO);
	}
}
