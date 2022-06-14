#ifndef QCOMMONSTRUCT_H
#define QCOMMONSTRUCT_H


#include <QString>
#include <QVariant>

struct tagComboItem
{
    QString m_text;
    QString m_icon;
    QVariant m_value;
    bool operator ==(const tagComboItem &item) {
        return (m_value == item.m_value);
    }
};

typedef QList<tagComboItem> ComboItems;
Q_DECLARE_METATYPE(ComboItems)


class tagMemeryValueInfo
{
public:
    QString m_propertyName;
    QString m_valueGroup;
    QString m_valueName;
    QString m_script;
    bool operator ==(const tagMemeryValueInfo &info) {
        return (info.m_propertyName == m_propertyName
                && info.m_valueName == m_valueName
                && info.m_script == m_script
                && info.m_valueGroup == m_valueGroup);
    }
};
typedef QList<tagMemeryValueInfo> MemeryValueItems;
Q_DECLARE_METATYPE(MemeryValueItems)

#endif // QCOMMONSTRUCT_H
