#ifndef QRESOURCEMANAGER_H
#define QRESOURCEMANAGER_H

#include "../sharedlibglobal.h"

#include <QObject>
#include <QMap>

class QResourceFile;

class SHAREDLIB_EXPORT QResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit QResourceManager(QObject *parent = 0);
    ~QResourceManager();
    void    load(const QString &path);
    void    save(const QString &path);
    void    clear();

    void    addResource(const QString &name,const QByteArray &data);
    void    removeResource(const QString &name);

    QResourceFile*   get_file();
signals:
    
public slots:
protected:
    QResourceFile   *m_file;
    QMap<QString,int>           m_resourceNumber;
};

#endif // QRESOURCEMANAGER_H
