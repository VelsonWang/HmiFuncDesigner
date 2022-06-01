#include "qstringchangedundocommand.h"
#include "../qsoftcore.h"
#include "../../shared/qprojectcore.h"
#include "../../shared/property/qabstractproperty.h"
#include "../../shared/host/qabstracthost.h"

QStringChangedUndoCommand::QStringChangedUndoCommand(const QString &host_id,
        const QString &property_name,
        const QString &id,
        bool old_tr,
        bool new_tr,
        const QString &old_text,
        const QString &new_text,
        const QMap<QString, QString> &old_translate,
        const QMap<QString, QString> &new_translate,
        QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_host_id(host_id),
    m_property_name(property_name),
    m_id(id),
    m_old_tr(old_tr),
    m_new_tr(new_tr),
    m_old_text(old_text),
    m_new_text(new_text),
    m_old_translates(old_translate),
    m_new_translates(new_translate)
{
}

int QStringChangedUndoCommand::id() const
{
    return STRING_CHANGED_UNDO_COMMAND;
}

void QStringChangedUndoCommand::redo()
{
    QAbstractHost *h = QSoftCore::getCore()->getProjectCore()->getHostByID(m_host_id);
    if(h != NULL) {
        QAbstractProperty* pro = h->getProperty(m_property_name);
        if(pro != NULL) {
            pro->setObjectProperty("tr", m_new_tr);
            pro->setObjectProperty("id", m_new_tr ? m_id : QVariant());
        }
        if(m_old_tr) {
            if(!m_new_tr) {
                h->setPropertyValue(m_property_name, m_new_text);
            } else {
                QMapIterator<QString, QString> it(m_new_translates);
                while(it.hasNext()) {
                    it.next();
                }
            }
        } else {
            if(m_new_tr) {
                QMapIterator<QString, QString> it(m_new_translates);
                while(it.hasNext()) {
                    it.next();
                }
            } else {
                h->setPropertyValue(m_property_name, m_new_text);
            }
        }
    }
}

void QStringChangedUndoCommand::undo()
{
    QAbstractHost *h = QSoftCore::getCore()->getProjectCore()->getHostByID(m_host_id);
    if(h != NULL) {
        QAbstractProperty* pro = h->getProperty(m_property_name);
        if(pro != NULL) {
            pro->setObjectProperty("tr", m_old_tr);
            pro->setObjectProperty("id", m_old_tr ? m_id : QVariant());
        }
        if(m_new_tr) {
            if(!m_old_tr) {
                h->setPropertyValue(m_property_name, m_old_text);
            } else {
                QMapIterator<QString, QString> it(m_old_translates);
                while(it.hasNext()) {
                    it.next();
                }
            }
        } else {
            if(m_old_tr) {
                QMapIterator<QString, QString> it(m_old_translates);
                while(it.hasNext()) {
                    it.next();
                }
            } else {
                h->setPropertyValue(m_property_name, m_old_text);
            }
        }
    }
}
