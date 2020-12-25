#include "userevent.h"

const QEvent::Type UserEvent::EVT_USER_SHOW_UPDATE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 100);
const QEvent::Type UserEvent::EVT_USER_HIDE_UPDATE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 101);


UserEvent::UserEvent(Type type, QVariant data) :
    QEvent(type),
    m_data(data){

}

QVariant UserEvent::data() const {
    return m_data;
}


