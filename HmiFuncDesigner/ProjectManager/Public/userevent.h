#ifndef USEREVENT_H
#define USEREVENT_H

#include <QEvent>
#include <QVariant>

class UserEvent : public QEvent
{
public:
    static const QEvent::Type EVT_USER_SHOW_UPDATE;
    static const QEvent::Type EVT_USER_HIDE_UPDATE;

public:
    UserEvent(Type type, QVariant data = QVariant());
    QVariant data() const;

private:
    QVariant m_data;
};

#endif 
