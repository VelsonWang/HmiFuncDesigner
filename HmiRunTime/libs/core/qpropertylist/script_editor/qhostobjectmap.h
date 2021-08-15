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

struct tagHostInfo
{
    QString m_name;
    QAbstractHost *m_host;
    QMap<QString, QMetaMethod> m_methods;
    QMap<QString, tagHostInfo*> m_children;
    QString m_type;
};

struct tagReturnInfo
{
    QString m_type;
    QString m_param;
    QMetaMethod m_method;
    bool m_is_last;
};

struct tagReturnList
{
    QList<tagReturnInfo> m_returns;
    QMap<QString, tagReturnInfo> m_name_to_return;
};

class QHostObjectMap
{
public:
    QHostObjectMap(QAbstractProperty *property, QAbstractHost *host);

    tagReturnList get_info(const QString &text);

protected:
    tagHostInfo* get_all_by_text(const QString &text);
    tagHostInfo* make_info(QAbstractHost *host);

protected:
    QMap<QString, tagHostInfo*> m_infos;
    QStringList m_param_list;
};

#endif // QHOSTOBJECTMAP_H
