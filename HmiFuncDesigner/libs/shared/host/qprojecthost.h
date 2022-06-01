#ifndef QPROJECTHOST_H
#define QPROJECTHOST_H

#include "qabstracthost.h"

#define PROJECT_HOST_TITLE  "Project"

class SHAREDLIB_EXPORT QProjectHost : public QAbstractHost
{
    Q_OBJECT
public:
    QProjectHost(QAbstractHost *parent = 0);
    virtual void    show_form_by_id(const QString &id);

protected:
    void initProperty() override;

public slots:
    virtual void show_form(const QString &name);
    virtual void show_dialog(const QString &name);

protected:
    void createObject() override;

};

#endif // QPROJECTHOST_H
