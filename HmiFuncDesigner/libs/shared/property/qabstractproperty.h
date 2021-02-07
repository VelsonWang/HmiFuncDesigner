#ifndef QABSTRACTPROPERTY_H
#define QABSTRACTPROPERTY_H

#include "../sharedlibglobal.h"

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QIcon>


#define PROPERTY_TITLE "Property"



#define ATTR_EDITABLE "Editable"
#define ATTR_NEEDSAVE "Need_Save"
#define ATTR_VISIBLE  "Visible"
#define ATTR_CAN_SAME "Can_Same"
#define ATTR_RESET_ABLEABLE "Can_Reset"

class XMLObject;
class QAbstractHost;

class SHAREDLIB_EXPORT QAbstractProperty : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractProperty(QAbstractProperty *parent = 0);
    ~QAbstractProperty();

    virtual void set_value(const QVariant& value);
    virtual QVariant get_value();
    void reset();
    virtual QString get_value_text() = 0;
    virtual QIcon get_value_icon() = 0;

    void setDefault();
    bool modified();

    virtual void toObject(XMLObject* xml);
    virtual void fromObject(XMLObject* xml);

    void setAttribute(const QString &key, const QVariant& value);
    QVariant getAttribute(const QString &key);

    void setObjectProperty(const QString &key, const QVariant& value);
    QVariant getObjectProperty(const QString &key);

    QAbstractProperty& operator =(const QAbstractProperty &pro);

    QList<QAbstractProperty *> getChildren();
    QAbstractProperty *getParent();
    QAbstractProperty *getChild(const QString &name);

    void emit_edit_value(const QVariant& value);

    void set_host(QAbstractHost* host);
    QAbstractHost* get_host();
	
protected slots:
    virtual void child_value_changed(const QVariant& old, const QVariant &now);
	
protected:
    void connect_children();
    void disconnect_children();
    virtual void make_value();
    virtual void write_value();
	
signals:
    void value_chaged(const QVariant& old, const QVariant &now);
    void refresh();
    void edit_value(const QVariant &value);
	
public slots:
	
protected:
    QAbstractProperty *m_parent;
    QList<QAbstractProperty *> m_children;
    QMap<QString, QVariant> m_attributes;
    QMap<QString, QVariant> m_propertys;
    QVariant m_value;
    QVariant m_defaultValue;
    QAbstractHost *m_host;
};

#endif // QABSTRACTPROPERTY_H

