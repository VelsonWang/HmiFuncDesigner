#ifndef QFONTPROPERTY_H
#define QFONTPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;
class QEnumProperty;
class QBoolProperty;

class QFontProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QFontProperty(QAbstractProperty* parent=0);

    void            set_value(const QVariant &value);

    QString         get_value_text();
    QIcon           get_value_icon();
protected:
    void            child_value_changed(const QVariant &old, const QVariant &now);
    void            make_value();
    void            write_value();
protected:
    QEnumProperty   *m_family;
    QIntProperty    *m_pointSize;
    QBoolProperty   *m_bold;
    QBoolProperty   *m_italic;
    QBoolProperty   *m_underline;
    QBoolProperty   *m_strikeout;
};
#endif // QFONTPROPERTY_H
