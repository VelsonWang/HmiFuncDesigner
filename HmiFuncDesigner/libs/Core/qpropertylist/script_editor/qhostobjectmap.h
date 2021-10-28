#ifndef QHOSTOBJECTMAP_H
#define QHOSTOBJECTMAP_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QMetaMethod>

#define HAS_NEXT 1
#define FUNCTION_NO_PARAM 2
#define FUNCTION_PARAM  4
#define FUNCTION_NO_RETURN 8
#define FUNCTION_RETURN 16

#define TYPE_FORM       "form"
#define TYPE_FUNCTION   "function"
#define TYPE_PARAM      "param"
#define TYPE_DATA       "data"

class QAbstractHost;
class QAbstractProperty;

struct tagHostInfo {
    QString m_name;
    QAbstractHost *m_host;
    QMap<QString, QMetaMethod> m_methods;
    QMap<QString, tagHostInfo*> m_children;
    QString m_type;
};

struct tagReturnInfo {
    QString m_type;
    QString m_param;
    QMetaMethod m_method;
    bool m_is_last;
};

struct tagReturnList {
    QList<tagReturnInfo> m_returns;
    QMap<QString, tagReturnInfo> m_name_to_return;
};

class QHostObjectMap
{
public:
    QHostObjectMap(QAbstractProperty *property, QAbstractHost *host);

    tagReturnList getInfo(const QString &text);

protected:
    tagHostInfo* getAllByText(const QString &text);
    tagHostInfo* makeInfo(QAbstractHost *host);

protected:
    QMap<QString, tagHostInfo*> m_infos;
    QStringList m_paramList;
};

#endif // QHOSTOBJECTMAP_H
