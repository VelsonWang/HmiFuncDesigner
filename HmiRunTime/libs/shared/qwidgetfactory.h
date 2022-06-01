#ifndef QWIDGETFACTORY_H
#define QWIDGETFACTORY_H

#include "sharedlibglobal.h"
#include <QMetaObject>
#include <QMap>
#include <QString>
#include <QWidget>

class QAbstractHost;
class XMLObject;

struct tagWidgetInfo {
    const QMetaObject *m_hostObject;
    QString m_name;
};

class SHAREDLIB_EXPORT QWidgetFactory
{
protected:
    QWidgetFactory();

public:
    static void registerWidget(const QString name, const QMetaObject* host);
    static QWidget* createWidget(const QString& name);
    static QMap<QString, tagWidgetInfo*> getWidgetInfo();
    static QWidget* createWidget(XMLObject* xml);
    static tagWidgetInfo* getWidgetInfo(const QString &name);

protected:
    static QMap<QString, tagWidgetInfo*> m_metaMap;
};


#ifndef qRegisterWidget
    #define qRegisterWidget(name,class_name) QWidgetFactory::registerWidget(name, &class_name::staticMetaObject)
#endif

#endif // QWIDGETFACTORY_H
