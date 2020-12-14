#ifndef QPROJECTCORE_H
#define QPROJECTCORE_H

#include "sharedlibglobal.h"

#include <QObject>

class QProjectHost;
class QFormHost;

class QPageManager;
class QLanguageManager;
class QUserManager;
class QAbstractHost;
class QLanguage;
class QScriptValue;
class QScriptEngine;
class QDataManager;
class QResourceManager;
class QDriverManager;

struct tagUserInfo;

class SHAREDLIB_EXPORT QProjectCore : public QObject
{
    Q_OBJECT
public:
    explicit QProjectCore(QObject *parent = 0);
    ~QProjectCore();

    bool    open(const QString &fileName);

    void    close();
    bool    is_opened();

    bool    create_new(const QString &path,const QString &name);

    QPageManager* get_page_manager();
    QLanguageManager *get_language_manager();
    QUserManager    *get_user_manager();
    QDataManager    *get_data_manager();
    QResourceManager *get_resource_manager();
    QDriverManager  *get_driver_manager();
    QAbstractHost   *get_project_host();
    QAbstractHost   *get_host_by_uuid(const QString& uuid);

    void        init_script_engine();
protected:
    void        init_script_engine(QAbstractHost* host);
    QScriptValue        get_script_object(QAbstractHost* host,QScriptEngine *engine);
public slots:
    void    save();
signals:
    void    opened_signals();
    void    closed_signals();
public slots:
protected slots:
    void    user_refresh(tagUserInfo *info);
    void    language_refresh(QLanguage* language);
    void    form_refresh(QAbstractHost* form);
protected:
    void    copy_file(const QString &old_file,const QString &new_file);
protected:
    QProjectHost    *m_project_host;
    QString         m_project_path;
    QString         m_project_name;
    bool            m_is_open;
    QPageManager    *m_page_manager;
    QLanguageManager *m_language_manager;
    QUserManager    *m_user_manager;
    QDataManager    *m_data_manager;
    QResourceManager* m_resource_manager;
    QDriverManager  *m_driver_manager;
};

#endif // QPROJECTCORE_H
