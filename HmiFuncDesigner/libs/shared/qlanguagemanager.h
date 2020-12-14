#ifndef QLANGUAGEMANAGER_H
#define QLANGUAGEMANAGER_H

#include "sharedlibglobal.h"

#include <QObject>
#include <QMap>

class QLanguage;

class SHAREDLIB_EXPORT QLanguageManager : public QObject
{
    Q_OBJECT
public:
    QLanguageManager(QObject* parent=0);
    ~QLanguageManager();

    void clear();
    void load(const QString &path);
    void save(const QString &path);

    QLanguage* get_current_language();
    QLanguage* get_language(const QString &uuid);

    QList<QLanguage*> get_all_languages();

    QString get_text(const QString &translage_uuid,const QString &language_uuid);

    void set_current_language(const QString &uuid);
    void set_text(const QString &translate_uuid, const QString &text, const QString &language_uuid);

    void remove_text(const QString &translate_uuid);

    void insert_language(QLanguage* l,int index=-1);
    void remove_language(const QString &uuid);
    void emit_refresh(QLanguage* language);
    void add_translate(const QString &language_uuid,const QString &translate_uuid,const QString &text);
signals:
    void current_language_changed();
    void current_text_changed(const QString &uuid);
    void refresh(QLanguage* language);
    void insert_language_signal(int index,QLanguage* l);
    void remove_language_signal(QLanguage* l);
protected:
    QList<QLanguage*>   m_languages;
    QMap<QString,QLanguage*>    m_id_to_language;
    QLanguage*          m_current_language;
    QString             m_path;
};

#endif // QLANGUAGEMANAGER_H
