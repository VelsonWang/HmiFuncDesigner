#ifndef SYSRUNTIMEEVENT_H
#define SYSRUNTIMEEVENT_H

#include <QEvent>

enum ESysRuntimeEvent{
    EV_StartRuntime = QEvent::User,
    EV_StopRuntime,
    EV_RestartRuntime,

};

class SysRuntimeEvent : public QEvent
{
public:
    explicit SysRuntimeEvent(Type type);
    ~SysRuntimeEvent();

};


#endif // SYSRUNTIMEEVENT_H
