#include "qpropertyfactory.h"

QMap<QString, tagPropertyInfo*> QPropertyFactory::m_meta_map;

QPropertyFactory::QPropertyFactory()
{
}

void QPropertyFactory::register_property(const QString &name, const QMetaObject *meta)
{
    tagPropertyInfo *info = m_meta_map.value(name);
    if(info == NULL) {
        info = new tagPropertyInfo;
        info->m_editor_meta_object = NULL;
    }
    info->m_property_meta_object = meta;
    m_meta_map.insert(name, info);
}

void QPropertyFactory::register_editor(const QString &name, const QMetaObject *editor)
{
    tagPropertyInfo *info = m_meta_map.value(name);
    if(info == NULL) {
        info = new tagPropertyInfo;
        info->m_property_meta_object = NULL;
    }
    info->m_editor_meta_object = editor;
    m_meta_map.insert(name, info);
}

QAbstractProperty* QPropertyFactory::create_property(const QString &name)
{
    tagPropertyInfo *info = m_meta_map.value(name);
    if(info == NULL) {
        return NULL;
    }
    const QMetaObject* obj = info->m_property_meta_object;

    QAbstractProperty* pro = NULL;
    if(obj != NULL) {
        pro = (QAbstractProperty*)obj->newInstance();
    }
    return pro;
}

QWidget* QPropertyFactory::create_editor(const QString &name,
        QAbstractProperty *property,
        QUndoStack *stack)
{
    tagPropertyInfo *info = m_meta_map.value(name);
    if(info == NULL) {
        return NULL;
    }
    const QMetaObject* obj = info->m_editor_meta_object;
    QWidget* pro = NULL;
    if(obj != NULL) {
        pro = (QWidget*)obj->newInstance(Q_ARG(QAbstractProperty*, property),
                                         Q_ARG(QUndoStack*, stack));
    }
    return pro;
}
