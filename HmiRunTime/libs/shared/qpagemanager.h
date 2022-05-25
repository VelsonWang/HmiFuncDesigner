#ifndef QPAGEMANAGER_H
#define QPAGEMANAGER_H

#include "sharedlibglobal.h"
#include <QObject>
#include <QWidget>
#include <QList>
#include <QMap>

class QAbstractHost;
class XMLObject;

class SHAREDLIB_EXPORT QPageManager: public QObject
{
    Q_OBJECT
public:
    QPageManager();

    void load(XMLObject *pXmlObj);
    void clear();

    QList<QWidget*> getPages();
    QWidget* getPage(int index);
    QWidget* getPage(const QString& uuid);
    QList<QWidget*> getPagesByTitle(const QString& title);

protected:
    QList<QWidget *> m_pages;
    QMap<QString, QWidget *> m_uuidToPage;
};

#endif // QPAGEMANAGER_H
