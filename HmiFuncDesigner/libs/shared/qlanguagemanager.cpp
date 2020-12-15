#include "qlanguagemanager.h"

#include "xmlobject.h"
#include "qlanguage.h"

#include <QDir>
#include <QFile>
#include <QUuid>

QLanguageManager::QLanguageManager(QObject *parent) :
    QObject(parent)
{
}

QLanguageManager::~QLanguageManager()
{
    clear();
}

void QLanguageManager::clear()
{
    qDeleteAll(m_languages);
    m_languages.clear();
    m_id_to_language.clear();
    m_current_language=NULL;
    m_path="";
}

void QLanguageManager::load(const QString &path)
{
    QString filePath=path+"/translates";

    m_path=filePath;

    QFileInfoList list=QDir(filePath).entryInfoList(QDir::Files);

    QFile f;
    QString str;
    foreach(QFileInfo info,list)
    {
        if(info.isFile() && info.filePath().endsWith(".xml"))
        {
            f.setFileName(info.filePath());
            if(f.open(QFile::ReadOnly))
            {
                str=f.readAll();
                XMLObject xml;
                if(xml.load(str,0))
                {
                    QLanguage *l=new QLanguage;
                    l->fromObject(&xml);
                    l->set_file_name(info.fileName());
                    m_languages.append(l);
                    m_id_to_language.insert(l->getUuid(),l);
                }
                f.close();
            }
        }
    }
}

void QLanguageManager::save(const QString &path)
{
    QString filePath=path+"/translates";
    QDir d(filePath);
    d.mkpath(filePath);

    foreach(QLanguage* l,m_languages)
    {
        XMLObject xml;
        l->toObject(&xml);
        QString str=xml.write();

        QFile f(filePath+"/"+l->get_file_name());
        if(f.open(QFile::WriteOnly))
        {
            f.write(str.toLocal8Bit());
            f.close();
        }
    }
}

QLanguage* QLanguageManager::get_current_language()
{
    return m_current_language;
}

QLanguage* QLanguageManager::get_language(const QString &uuid)
{
    return m_id_to_language.value(uuid);
}

QList<QLanguage*> QLanguageManager::get_all_languages()
{
    return m_languages;
}

QString QLanguageManager::get_text(const QString &translage_uuid, const QString &language_uuid)
{
    QLanguage *l=NULL;
    if(language_uuid=="")
    {
        l=m_current_language;
    }
    else
    {
        l=m_id_to_language.value(language_uuid);
    }
    if(l!=NULL)
    {
        tagTranslateInfo *info=l->get_translate(translage_uuid);
        if(info!=NULL)
        {
            return info->m_translate;
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}

void QLanguageManager::set_current_language(const QString &uuid)
{
    QLanguage *l=m_id_to_language.value(uuid);
    if(m_current_language!=l)
    {
        m_current_language=l;
        emit notifyCurLanguageChanged();
    }
}

void QLanguageManager::set_text(const QString &translate_uuid, const QString &text, const QString &language_uuid)
{
    QLanguage* l=m_id_to_language.value(language_uuid);
    if(l!=NULL)
    {
        l->set_translate(translate_uuid,text);
        if(l==m_current_language)
        {
            emit notifyCurTextChanged(translate_uuid);
        }
    }
}

void QLanguageManager::remove_text(const QString &translate_uuid)
{
    QMapIterator<QString,QLanguage*> it(m_id_to_language);

    while(it.hasNext())
    {
        it.next();
        it.value()->remove_translate(translate_uuid);
    }
}

void QLanguageManager::insert_language(QLanguage *l, int index)
{
    if(l!=NULL && l->get_language_name()!="" && l->getUuid()!="")
    {
        if(index<0 || index>m_languages.size())
        {
            index=m_languages.size();
        }
        m_languages.insert(index,l);
        m_id_to_language.insert(l->getUuid(),l);
        emit insert_language_signal(index,l);
    }
}

void QLanguageManager::remove_language(const QString &uuid)
{
    QLanguage *l=m_id_to_language.value(uuid);
    if(l!=NULL)
    {
        emit remove_language_signal(l);
        m_id_to_language.remove(uuid);
        m_languages.removeAll(l);
        delete l;
    }
}

void QLanguageManager::emit_refresh(QLanguage *language)
{
    emit refresh(language);
}

void QLanguageManager::add_translate(const QString &language_uuid, const QString &translate_uuid, const QString &text)
{
    tagTranslateInfo info;
    info.m_translate=text;
    info.m_uuid=translate_uuid;
    QLanguage* l=m_id_to_language.value(language_uuid);
    if(l!=NULL)
    {
        l->add_translate(info);
        if(l==m_current_language)
        {
            emit notifyCurTextChanged(translate_uuid);
        }
    }
}
