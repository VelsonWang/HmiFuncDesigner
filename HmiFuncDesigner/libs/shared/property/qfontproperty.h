#ifndef QFONTPROPERTY_H
#define QFONTPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;
class QEnumProperty;
class QBoolProperty;

class QFontProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QFontProperty(QAbstractProperty* parent = 0);

    void set_value(const QVariant &value);

    QString getValueText();
    QIcon getValueIcon();
	
protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void makeValue();
    void writeValue();
	
protected:
    QEnumProperty *m_family;
    QIntProperty *m_pointSize;
    QBoolProperty *m_bold;
    QBoolProperty *m_italic;
    QBoolProperty *m_underline;
    QBoolProperty *m_strikeout;
};

#endif // QFONTPROPERTY_H

