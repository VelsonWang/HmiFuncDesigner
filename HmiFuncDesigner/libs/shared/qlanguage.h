#ifndef QLANGUAGE_H
#define QLANGUAGE_H

#include "sharedlibglobal.h"

#include <QString>
#include <QMap>

class XMLObject;

struct tagTranslateInfo
{
    QString     m_translate;
    QString     m_uuid;
};

class SHAREDLIB_EXPORT QLanguage
{
public:
    QLanguage();
    ~QLanguage();

    void toObject(XMLObject* xml);
    void fromObject(XMLObject* xml);

    void clear();

    QString get_language_name();
    void    set_language_name(const QString &name);
    tagTranslateInfo* get_translate(const QString &uuid);
    void    set_translate(const QString &uuid,const QString& translate);
    tagTranslateInfo*    add_translate(tagTranslateInfo& info);

    QMap<QString,tagTranslateInfo*> get_all_translate();

    void    remove_translate(const QString &uiid);

    void    set_file_name(const QString &fileName);
    QString get_file_name();

    QString get_information();
    void    set_information(const QString &information);

    void    setUuid(const QString &uuid);
    QString getUuid();

    QLanguage& operator =(const QLanguage &l);
protected:
    QString                 m_language_name;
    QMap<QString,tagTranslateInfo*>   m_translates;
    QString                 m_file_name;
    QString                 m_information;
    QString                 m_uuid;
};

#endif // QLANGUAGE_H
