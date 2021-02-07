#ifndef QABSTRACTSTYLESHEETITEM_H
#define QABSTRACTSTYLESHEETITEM_H

#include "../../sharedlibglobal.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>

class XMLObject;

class SHAREDLIB_EXPORT QAbstractStylesheetItem : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractStylesheetItem(QAbstractStylesheetItem *parent = 0);
    ~QAbstractStylesheetItem();

    virtual QVariant value();
    virtual void setValue(const QVariant &value);

    QString attribute(const QString &key, const QString &def="");
    void setAttribute(const QString &key, const QString &value);

    virtual void read(XMLObject *xml);
    virtual void write(XMLObject *xml);

    virtual QString makeStylesheet();

    virtual bool isUsed();
    virtual void setDefault(const QVariant &value);

    void clear();
	
protected:
    QAbstractStylesheetItem *getChild(const QString &name);

signals:
    void valueChanged();
	
protected slots:
    virtual void subValueChanged();
	
protected:
    QList<QAbstractStylesheetItem*> m_children;
    QAbstractStylesheetItem* m_parent;
    QMap<QString, QString> m_attributes;
    QVariant m_value;
};

#endif // QABSTRACTSTYLESHEETITEM_H
