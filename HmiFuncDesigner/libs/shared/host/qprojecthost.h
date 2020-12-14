#ifndef QPROJECTHOST_H
#define QPROJECTHOST_H

#include "qabstracthost.h"

#define PROJECT_HOST_TITLE  "Project"

class SHAREDLIB_EXPORT QProjectHost : public QAbstractHost
{
    Q_OBJECT
public:
    QProjectHost(QAbstractHost *parent = 0);
    virtual void    show_form_by_uuid(const QString &uuid);
protected:
    void init_property();
public slots:
    void    set_language(const QString &language);
    QString current_language();

    virtual void    show_form(const QString &name);
    virtual void    show_dialog(const QString &name);



protected:
    void    create_object();
};

#endif // QPROJECTHOST_H
