#ifndef QPROPERTYFACTORY_H
#define QPROPERTYFACTORY_H

#include "sharedlibglobal.h"

#include <QMetaObject>
#include <QMap>
#include <QString>
#include <QWidget>
#include <QUndoStack>

class QAbstractProperty;

struct tagPropertyInfo
{
    const QMetaObject *m_property_meta_object;
    const QMetaObject *m_editor_meta_object;
};

class SHAREDLIB_EXPORT QPropertyFactory
{
protected:
    QPropertyFactory();

public:
    static void register_property(const QString &name,const QMetaObject* property);
    static void register_editor(const QString &name,const QMetaObject* editor);
    static QAbstractProperty* create_property(const QString& name);
    static QWidget*           createEditor(const QString& name,QAbstractProperty* property,QUndoStack* stack=NULL);
protected:
    static QMap<QString,tagPropertyInfo*>        m_meta_map;
};

#ifndef qRegisterProperty
    #define qRegisterProperty(name,type) QPropertyFactory::register_property(name,&type::staticMetaObject)
#endif

#ifndef qRegisterPropertyEditor
    #define qRegisterPropertyEditor(name,type) QPropertyFactory::register_editor(name,&type::staticMetaObject)
#endif

#endif // QPROPERTYFACTORY_H
