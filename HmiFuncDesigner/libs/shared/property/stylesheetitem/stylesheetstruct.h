#ifndef STYLESHEETSTRUCT_H
#define STYLESHEETSTRUCT_H

#include <QVariant>
#include <QGradient>
#include <QMap>
#include <QVariant>
#include <QGradient>

class tagStylesheetItem
{
public:
    bool operator ==(const tagStylesheetItem &item)
    {
        return (m_attributes==item.m_attributes);
    }

    bool operator !=(const tagStylesheetItem &item)
    {
        return !(*this==item);
    }

public:
    QMap<QString, QVariant> m_attributes;
};

typedef QList<tagStylesheetItem>  tagStylesheetItems;

Q_DECLARE_METATYPE(tagStylesheetItem)
Q_DECLARE_METATYPE(tagStylesheetItems)
Q_DECLARE_METATYPE(QGradient)

#endif // STYLESHEETSTRUCT_H
